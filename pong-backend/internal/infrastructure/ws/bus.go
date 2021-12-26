package ws

import (
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
)

type Bus interface {
	HandleEmbeddedConnectionOpen() (id uint32, ch chan *protoEmbedded.EmbeddedResponse, err error)
	HandleEmbeddedConnectionClose(id uint32)
	HandleEmbeddedEvent(event *protoEmbedded.EmbeddedRequest)
}

