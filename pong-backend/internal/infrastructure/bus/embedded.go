package bus

import (
	"fmt"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"log"
)

func (b *bus) HandleEmbeddedConnectionOpen() (id uint32, ch chan *protoEmbedded.EmbeddedResponse, err error) {
	id, ch, err = b.embeddedService.StartEmbeddedConnection()
	if err != nil {
		return 0, nil, err
	}
	// send message to client saying we connected
	return id, ch, nil
}

func (b *bus) HandleEmbeddedConnectionClose(id uint32) {
	b.embeddedService.CloseEmbeddedConnection(id)
	// send message to client saying we out this bitch
}

func (b *bus) HandleEmbeddedEvent(event *protoEmbedded.EmbeddedRequest) {
	log.Println("bus, HandleEmbeddedEvent: Handling")
	e := event.Request
	switch e.(type) {
	case *protoEmbedded.EmbeddedRequest_Position:
		eCast := e.(*protoEmbedded.EmbeddedRequest_Position)
		b.handleEmbeddedPositionEvent(eCast.Position.Position)
	}
}

func (b *bus) handleEmbeddedPositionEvent(position uint32) {
	log.Println(fmt.Sprintf("bus, handleEmbeddedPositionEvent; position %d", position))
	// send position to the client
}