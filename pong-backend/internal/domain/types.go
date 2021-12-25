package domain

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
)

type EmbeddedService interface {
	StartEmbeddedConnection()(chan *protoEmbedded.EmbeddedResponse, error)
	CloseEmbeddedConnection()
	GetEmbeddedIsConnected() (isConnected bool)
	SendClientConnected(connected *protoCommon.Connected)
	PongEmbedded()
	Shutdown()
}