package application

import (
	"github.com/polis-interactive/Pong/pong-backend/internal/domain"
	"github.com/polis-interactive/Pong/pong-backend/internal/infrastructure/ws"
)

type applicationBus interface {
	BindEmbeddedService(e domain.EmbeddedService)
	Shutdown()
	ws.Bus
}