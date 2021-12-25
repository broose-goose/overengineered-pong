package domain

import (
	"sync"
	"time"
)

type ConnectionKeepAlive struct {
	Wg *sync.WaitGroup
	PongChannel chan bool
	Shutdown chan struct{}
	pingFunc func ()()
	keepAliveTimeout time.Duration
	closeConnection func()()
}

func NewConnectionKeepAlive(
	sendPing func()(),
	closeConnection func()(),
	keepAliveTimeout time.Duration,
) *ConnectionKeepAlive {
	newConnectionKeepAlive := &ConnectionKeepAlive{
		Wg: &sync.WaitGroup{},
		PongChannel: make(chan bool, 10),
		Shutdown: make(chan struct{}),
		pingFunc: sendPing,
		keepAliveTimeout: keepAliveTimeout,
		closeConnection: closeConnection,
	}
	newConnectionKeepAlive.Wg.Add(1)
	go newConnectionKeepAlive.RunConnectionKeepAlive()
	return newConnectionKeepAlive
}

func (c *ConnectionKeepAlive) RunConnectionKeepAlive() {
	for {
		c.pingFunc()
		// wait at least keepAliveTimeout for pong
		select {
		case _, ok := <-c.PongChannel:
			if !ok {
				goto StopKeepAlive
			}
		case _, ok := <-c.Shutdown:
			if !ok {
				goto StopKeepAlive
			}
		case <-time.After(c.keepAliveTimeout):
			go c.closeConnection()
			goto StopKeepAlive
		}
		// don't ping again till after the timeout
		select {
		case _, ok := <-c.Shutdown:
			if !ok {
				goto StopKeepAlive
			}
		case <-time.After(c.keepAliveTimeout):
		}
		// pump the channel for ass hats
		for len(c.PongChannel) > 0 {
			<-c.PongChannel
		}
	}
StopKeepAlive:
	c.Wg.Done()
}