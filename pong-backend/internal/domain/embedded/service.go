package embedded

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"github.com/polis-interactive/Pong/pong-backend/internal/domain"
	"log"
	"sync"
	"time"
)

type Service struct {
	mu *sync.RWMutex
	embedded *embedded
	isConnected bool
	pongTimeout time.Duration
}

var _ domain.EmbeddedService = (*Service)(nil)

func NewService(cfg Config) *Service {
	log.Println("embedded, NewService: creating service")
	return &Service{
		mu: &sync.RWMutex{},
		embedded: nil,
		isConnected: false,
		pongTimeout: cfg.GetEmbeddedPongTimeout(),
	}
}

func (s *Service) StartEmbeddedConnection() (chan *protoEmbedded.EmbeddedResponse, error) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.embedded != nil || s.isConnected == true {
		s.doCloseEmbeddedConnection()
	}
	s.embedded = NewEmbedded(s.pongTimeout)
	s.isConnected = true
	return s.embedded.ch, nil
}

func (s *Service) CloseEmbeddedConnection() {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.doCloseEmbeddedConnection()
}

func (s *Service) doCloseEmbeddedConnection() {
	if s.embedded != nil {
		s.embedded.Disconnect()
		s.embedded = nil
	}
	s.isConnected = false
}

func (s *Service) GetEmbeddedIsConnected() bool {
	s.mu.RLock()
	defer s.mu.RUnlock()
	return s.isConnected
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

func (s *Service) PongEmbedded() {
	s.mu.RLock()
	defer s.mu.RUnlock()
	if s.embedded != nil {
		s.embedded.AcknowledgePong()
	}
}

func (s *Service) Shutdown() {
	log.Println("embedded, service, shutdown: shutting down service")
	s.CloseEmbeddedConnection()
}