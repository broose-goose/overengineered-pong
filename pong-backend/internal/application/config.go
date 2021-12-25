package application

import "time"

type Config struct {
	EmbeddedPongTimeout time.Duration
	WsPort int
}



func (conf *Config) GetEmbeddedPongTimeout() time.Duration {
	return conf.EmbeddedPongTimeout
}

func (conf *Config) GetWsPort() int {
	return conf.WsPort
}