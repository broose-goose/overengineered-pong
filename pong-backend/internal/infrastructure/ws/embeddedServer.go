package ws

import (
	"errors"
	"fmt"
	"github.com/gorilla/websocket"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"google.golang.org/protobuf/proto"
	"log"
	"net/http"
	"sync"
)

type embeddedServer struct {
	mux *http.ServeMux
	b Bus
	shutdowns chan struct{}
	wg *sync.WaitGroup
}

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
}

func newEmbeddedServer(bus Bus, shutdowns chan struct{}, wg *sync.WaitGroup) (*embeddedServer, error) {

	log.Println("embeddedServer, newServer: creating")

	srv := &embeddedServer{
		mux: http.NewServeMux(),
		b: bus,
		shutdowns: shutdowns,
		wg: wg,
	}

	srv.mux.HandleFunc("/", srv.embeddedHandler)

	return srv, nil
}


func (e *embeddedServer) embeddedHandler(w http.ResponseWriter, r *http.Request) {

	log.Println("embeddedServer, embeddedHandler: creating connection")

	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println(fmt.Sprintf("embeddedServer, embeddedHandler: couldn't create connection; %s", err.Error()))
		return
	}

	log.Println("embeddedServer, embeddedHandler: running")

	embeddedOutChan, err := e.b.HandleEmbeddedConnectionOpen()

	if err != nil {
		log.Println(fmt.Sprintf("embeddedServer, embeddedHandler: couldn't handle connection open; %s", err.Error()))
		return
	}

	embeddedInChan := make(chan *protoEmbedded.EmbeddedRequest, 10)

	log.Println("embeddedServer, embeddedHandler: handling messages")

	e.wg.Add(2)

	// only reads on connection here
	go func(stream *websocket.Conn, ch chan *protoEmbedded.EmbeddedRequest, wg *sync.WaitGroup) {
		event := &protoEmbedded.EmbeddedRequest{}
		for {
			_, bEvent, err := stream.ReadMessage()
			if err != nil {
				if websocket.IsUnexpectedCloseError(err, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
					log.Printf("error: %v", err)
				}
				goto StopStream
			}
			err = proto.Unmarshal(bEvent, event)
			if err != nil {
				log.Printf(fmt.Sprintf("embeddedServer, embeddedHandler: failed to unmarshal proto; %s", err))
				continue
			}
			ch<-event
		}
	StopStream:
		log.Println("embeddedServer, embeddedHandler, in chan: closing")
		close(ch)
		wg.Done()
	}(conn, embeddedInChan, e.wg)

	err = func(
		stream *websocket.Conn,
		outChannel chan *protoEmbedded.EmbeddedResponse,
		inChannel chan *protoEmbedded.EmbeddedRequest,
		bus Bus,
		shutdowns chan struct{},
	) error {
		for {
			select {
			case _, ok := <-shutdowns:
				if !ok {
					return nil
				}
			case msg, ok := <-inChannel:
				if !ok {
					return errors.New("in channel closed unexpectedly")
				}
				bus.HandleEmbeddedEvent(msg)
			case msg, ok := <-outChannel:
				if !ok {
					return errors.New("out channel closed unexpectedly")
				}
				log.Println(msg)
				bMsg, err := proto.Marshal(msg)
				log.Println(bMsg)
				if err != nil {
					return errors.New(fmt.Sprintf("couldn't marshal protobuf; %s", err))
				}
				err = conn.WriteMessage(websocket.BinaryMessage, bMsg)
				if err != nil {
					return errors.New(fmt.Sprintf("failed to send msg; %s", err))
				}
			}
		}
	}(conn, embeddedOutChan, embeddedInChan, e.b, e.shutdowns)

	if err != nil {
		log.Println(fmt.Sprintf("embeddedServer, embeddedHandler: out chan error; %s", err))
	}

	err = conn.WriteMessage(websocket.CloseMessage, []byte{})
	if err != nil {
		log.Println(fmt.Sprintf("embeddedServer, embeddedHandler: couldn't send close event; %s", err.Error()))
	}
	err = conn.Close()
	if err != nil {
		log.Println(fmt.Sprintf("embeddedServer, embeddedHandler: couldn't close connection; %s", err.Error()))
	}

	e.b.HandleEmbeddedConnectionClose()

	e.wg.Done()

}