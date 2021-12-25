package application

import (
	"github.com/polis-interactive/Pong/pong-backend/internal/domain/embedded"
	"github.com/polis-interactive/Pong/pong-backend/internal/infrastructure/bus"
	"github.com/polis-interactive/Pong/pong-backend/internal/infrastructure/ws"
	"log"
	"sync"
)

type Application struct {
	wsServer *ws.Server
	serviceBus applicationBus
	shutdown     bool
	shutdownLock sync.Mutex
}

func NewApplication(conf *Config) (*Application, error) {

	log.Println("Application, NewApplication: creating")

	/* create application instance */
	app := &Application{
		shutdown: true,
	}

	/* create bus */
	app.serviceBus = bus.NewBus()

	/* create services */
	embeddedService := embedded.NewService(conf)
	app.serviceBus.BindEmbeddedService(embeddedService)

	/* create servers */
	wsServer, err := ws.NewServer(conf, app.serviceBus)
	if err != nil {
		log.Fatalf("Application, NewApplication: failed to initialize WS server")
		return nil, err
	}
	app.wsServer = wsServer

	log.Println("Application, NewApplication: created")

	return app, err
}

func (app *Application) Startup() error {

	log.Println("Application, Startup: starting")

	app.shutdownLock.Lock()
	defer app.shutdownLock.Unlock()
	if app.shutdown == false {
		return nil
	}

	err := app.wsServer.Startup()
	if err != nil {
		log.Fatalf("Application, Startup: failed to startup WS server")
		return err
	}

	app.shutdown = false

	log.Println("Application, Startup: started")

	return nil
}

func (app *Application) Shutdown() error {

	log.Println("Application, Shutdown: shutting down")

	app.shutdownLock.Lock()
	defer app.shutdownLock.Unlock()
	if app.shutdown {
		return nil
	}
	app.shutdown = true

	app.wsServer.Shutdown()
	app.serviceBus.Shutdown()

	log.Println("Application, Shutdown: shut down")

	return nil
}