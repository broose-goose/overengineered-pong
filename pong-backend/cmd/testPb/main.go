package main

import (
	protoCommon "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoEmbedded "github.com/polis-interactive/Pong/pong-backend/api/v1/go/embedded"
	"google.golang.org/protobuf/proto"
	"log"
)

func main() {
	msg := &protoEmbedded.EmbeddedResponse{
		Response: &protoEmbedded.EmbeddedResponse_Ping{
			Ping: &protoCommon.Ping{},
		},
	}

	bMsg, err := proto.Marshal(msg)
	if err != nil {
		log.Fatalln("couldn't marshal")
		return
	}
	log.Println(bMsg)

	event := &protoEmbedded.EmbeddedResponse{}

	err = proto.Unmarshal(bMsg, event)
	if err != nil {
		log.Fatalln("couldn't unmarshal")
		return
	}
	log.Println(event)
}