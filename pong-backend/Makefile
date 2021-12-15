
.PHONY: compile
compile:
	protoc ./api/v1/common.proto \
		--proto_path=./api/v1 \
		--go_out=./api/v1/go/common \
		--go_opt=paths=source_relative \

	protoc ./api/v1/client.proto \
		--proto_path=./api/v1 \
		--go_out=./api/v1/go/client \
		--go-grpc_out=./api/v1/go/client \
		--go_opt=paths=source_relative \
        --go-grpc_opt=paths=source_relative

	protoc ./api/v1/embeded.proto \
		--proto_path=./api/v1 \
		--go_out=./api/v1/go/embeded \
		--go_opt=paths=source_relative \

	protoc ./api/v1/common.proto \
		--proto_path=./api/v1 \
		--js_out=import_style=commonjs:./api/v1/web  \
		--grpc-web_out=import_style=typescript,mode=grpcwebtext:./api/v1/web

	protoc ./api/v1/client.proto \
		--proto_path=./api/v1 \
		--js_out=import_style=commonjs:./api/v1/web  \
		--grpc-web_out=import_style=typescript,mode=grpcwebtext:./api/v1/web

	protoc --plugin=protoc-gen-nanopb="/c/Program Files (x86)/nanopb/generator/protoc-gen-nanopb.bat" \
		--proto_path=./api/v1 \
		--nanopb_out=./api/v1/embeded \
		./api/v1/common.proto

	protoc --plugin=protoc-gen-nanopb="/c/Program Files (x86)/nanopb/generator/protoc-gen-nanopb.bat" \
		--proto_path=./api/v1 \
		--nanopb_out=./api/v1/embeded \
		./api/v1/embeded.proto

