package embedded

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"github.com/polis-interactive/Pong/pong-backend/internal/domain"
	"log"
	"sync"
	"time"
)

type embedded struct {
	ch chan *protoEmbedded.EmbeddedResponse
	keepAlive *domain.ConnectionKeepAlive
	mu *sync.RWMutex
}

func NewEmbedded(connectionTimeout time.Duration) *embedded {
	newEmbedded := &embedded {
		ch: make(chan *protoEmbedded.EmbeddedResponse, 10),
		mu: &sync.RWMutex{},
	}
	keepAlive := domain.NewConnectionKeepAlive(
		newEmbedded.sendPing,
		newEmbedded.Disconnect,
		connectionTimeout,
	)
	newEmbedded.keepAlive = keepAlive
	return newEmbedded
}

func (e *embedded) sendPing() {
	e.mu.RLock()
	defer e.mu.RUnlock()
	if e.ch != nil {
		log.Println("Embedded: Pinging the big fkr")
		e.ch<-&protoEmbedded.EmbeddedResponse{
			Response: &protoEmbedded.EmbeddedResponse_Ping{
				Ping: &protoCommon.Ping{},
			},
		}
	}
}

func (e *embedded) AcknowledgePong() {
	e.mu.RLock()
	defer e.mu.RUnlock()
	if e.keepAlive != nil {
		e.keepAlive.PongChannel<-true
	}
}

func (e *embedded) Disconnect() {
	e.mu.Lock()
	defer e.mu.Unlock()
	if e.keepAlive != nil {
		close(e.keepAlive.Shutdown)
		e.keepAlive.Wg.Wait()
		e.keepAlive = nil
	}
	if e.ch != nil {
		close(e.ch)
		e.ch = nil
	}
}