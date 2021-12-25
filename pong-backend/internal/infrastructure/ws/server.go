package ws

import (
	"errors"
	"fmt"
	"log"
	"net/http"
	"sync"
)

type Server struct {
	srv *http.Server
	embeddedServer *embeddedServer
	port int
	shutdown bool
	shutdownLock sync.Mutex
	shutdowns chan struct{}
	wg *sync.WaitGroup
}

func NewServer(
	config Config,
	bus Bus,
) (*Server, error) {

	log.Println("wsServer, NewServer: creating")

	shutdowns := make(chan struct{})
	wg := &sync.WaitGroup{}

	srv, err := newEmbeddedServer(bus, shutdowns, wg)
	if err != nil {
		return nil, err
	}

	log.Println("wsServer, NewServer: created successfully")

	return &Server{
		embeddedServer: srv,
		port:   config.GetWsPort(),
		shutdown: true,
		shutdowns: shutdowns,
		wg: wg,
	}, nil
}

func (s *Server) Startup() error {

	log.Println("wsServer, Startup: starting")

	s.shutdownLock.Lock()
	defer s.shutdownLock.Unlock()

	if s.shutdown == false {
		return errors.New("wsServer, Startup: Tried to startup server twice")
	}

	addr := fmt.Sprintf("0.0.0.0:%d", s.port)
	log.Println(fmt.Sprintf("wsServer, Startup: listening at %s", addr))

	s.srv = &http.Server{
		Addr: addr,
		Handler: s.embeddedServer.mux,
	}

	go func (srv *http.Server) {
		err := srv.ListenAndServe()
		if err != nil {
			log.Println(fmt.Sprintf("wsServer: reported err %s", err))
			s.Shutdown()
		}
	}(s.srv)

	s.shutdown = false

	log.Println("wsServer, Startup: started")

	return nil
}

func (s *Server) Shutdown() {

	log.Printf("wsServer, Shutdown: Shutting down")

	s.shutdownLock.Lock()
	defer s.shutdownLock.Unlock()
	if s.shutdown {
		return
	}
	s.shutdown = true

	close(s.shutdowns)
	s.wg.Wait()
	err := s.srv.Close()
	if err != nil {
		log.Println(fmt.Sprintf("wsServer, Shutdown: error closing server, %s", err.Error()))
	}

	log.Printf("wsServer, Shutdown: probably done")
}
