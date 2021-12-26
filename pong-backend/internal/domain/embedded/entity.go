package embedded

import (
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"sync"
)

var lastId uint32 = 0

type embedded struct {
	ch chan *protoEmbedded.EmbeddedResponse
	mu *sync.RWMutex
	id uint32
}

func NewEmbedded() *embedded {
	lastId += 1
	newEmbedded := &embedded {
		ch: make(chan *protoEmbedded.EmbeddedResponse, 10),
		mu: &sync.RWMutex{},
		id: lastId,
	}
	return newEmbedded
}

func (e *embedded) Disconnect() {
	e.mu.Lock()
	defer e.mu.Unlock()
	if e.ch != nil {
		close(e.ch)
		e.ch = nil
	}
}