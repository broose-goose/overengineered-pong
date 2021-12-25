package ws

import (
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
)

type Bus interface {
	HandleEmbeddedConnectionOpen() (ch chan *protoEmbedded.EmbeddedResponse, err error)
	HandleEmbeddedConnectionClose()
	HandleEmbeddedEvent(event *protoEmbedded.EmbeddedRequest)
}

