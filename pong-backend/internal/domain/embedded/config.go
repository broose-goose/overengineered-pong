package embedded

import "time"

type Config interface {
	GetEmbeddedPongTimeout() time.Duration
}
