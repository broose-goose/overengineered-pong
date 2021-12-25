package bus

import (
	"fmt"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"log"
)

func (b *bus) HandleEmbeddedConnectionOpen() (ch chan *protoEmbedded.EmbeddedResponse, err error) {
	ch, err = b.embeddedService.StartEmbeddedConnection()
	if err != nil {
		return nil, err
	}
	// send message to client saying we connected
	return ch, nil
}

func (b *bus) HandleEmbeddedConnectionClose() {
	b.embeddedService.CloseEmbeddedConnection()
	// send message to client saying we out this bitch
}

func (b *bus) HandleEmbeddedEvent(event *protoEmbedded.EmbeddedRequest) {
	log.Println("bus, HandleEmbeddedEvent: Handling")
	e := event.Request
	switch e.(type) {
	case *protoEmbedded.EmbeddedRequest_Position:
		eCast := e.(*protoEmbedded.EmbeddedRequest_Position)
		b.handleEmbeddedPositionEvent(eCast.Position.Position)
	case *protoEmbedded.EmbeddedRequest_Pong:
		b.handleEmbeddedPongEvent()
	}
}

func (b *bus) handleEmbeddedPositionEvent(position uint32) {
	log.Println(fmt.Sprintf("bus, handleEmbeddedPositionEvent; position %d", position))
	// send position to the client
}

func (b *bus) handleEmbeddedPongEvent() {
	log.Println("bus, handleEmbeddedPongEvent")
	b.embeddedService.PongEmbedded()
}