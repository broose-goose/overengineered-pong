package domain

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
)

type EmbeddedService interface {
	StartEmbeddedConnection()(id uint32, ch chan *protoEmbedded.EmbeddedResponse, err error)
	CloseEmbeddedConnection(id uint32)
	GetEmbeddedIsConnected() (isConnected bool)
	SendClientConnected(connected *protoCommon.Connected)
	Shutdown()
}