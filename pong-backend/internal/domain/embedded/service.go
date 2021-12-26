package embedded

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"github.com/polis-interactive/Pong/pong-backend/internal/domain"
	"log"
	"sync"
)

type Service struct {
	mu *sync.RWMutex
	embedded *embedded
}

var _ domain.EmbeddedService = (*Service)(nil)

func NewService(cfg Config) *Service {
	log.Println("embedded, NewService: creating service")
	return &Service{
		mu: &sync.RWMutex{},
		embedded: nil,
	}
}

func (s *Service) StartEmbeddedConnection() (uint32, chan *protoEmbedded.EmbeddedResponse, error) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.embedded != nil {
		s.doCloseEmbeddedConnection(s.embedded.id)
	}
	s.embedded = NewEmbedded()
	return s.embedded.id, s.embedded.ch, nil
}

func (s *Service) CloseEmbeddedConnection(id uint32) {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.doCloseEmbeddedConnection(id)
}

func (s *Service) doCloseEmbeddedConnection(id uint32) {
	if s.embedded != nil && s.embedded.id == id{
		s.embedded.Disconnect()
		s.embedded = nil
	}
}

func (s *Service) GetEmbeddedIsConnected() bool {
	s.mu.RLock()
	defer s.mu.RUnlock()
	return s.embedded != nil
}

func (s *Service) SendClientConnected(connected *protoCommon.Connected) {
	s.mu.RLock()
	defer s.mu.RUnlock()
	if s.embedded != nil {
		s.embedded.ch<-&protoEmbedded.EmbeddedResponse{
			Response: &protoEmbedded.EmbeddedResponse_Connected{
				Connected: connected,
			},
		}
	}
}

func (s *Service) Shutdown() {
	log.Println("embedded, service, shutdown: shutting down service")
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.embedded != nil  {
		s.doCloseEmbeddedConnection(s.embedded.id)
	}
}