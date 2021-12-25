package main

import (
	"github.com/polis-interactive/Pong/pong-backend/internal/application"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"
)

func main() {
	conf := &application.Config{
		EmbeddedPongTimeout: 15 * time.Second,
		WsPort: 9001,
	}

	app, err := application.NewApplication(conf)
	if err != nil {
		panic(err)
	}

	err = app.Startup()
	if err != nil {
		log.Println("Main: failed to startup, shutting down")
		err2 := app.Shutdown()
		if err2 != nil {
			log.Println("Main: issue shutting down; ", err2)
		}
		panic(err)
	}

	c := make(chan os.Signal)
	signal.Notify(c, os.Interrupt, syscall.SIGTERM)
	<-c

	err = app.Shutdown()
	if err != nil {
		log.Println("Main: issue shutting down; ", err)
	}
}
