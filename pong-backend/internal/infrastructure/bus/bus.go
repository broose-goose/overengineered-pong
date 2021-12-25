package bus

import "github.com/polis-interactive/Pong/pong-backend/internal/domain"

type bus struct {
	embeddedService domain.EmbeddedService
}

func NewBus() *bus {
	return &bus{}
}

func (b *bus) BindEmbeddedService(e domain.EmbeddedService) {
	b.embeddedService = e
}

func (b *bus) Shutdown() {
	b.embeddedService.Shutdown()
}