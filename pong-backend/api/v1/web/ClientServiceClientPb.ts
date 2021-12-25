/**
 * @fileoverview gRPC-Web generated client stub for PongBackend.v1.client
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck


import * as grpcWeb from 'grpc-web';

import * as common_pb from './common_pb';
import * as client_pb from './client_pb';


export class ClientClient {
  client_: grpcWeb.AbstractClientBase;
  hostname_: string;
  credentials_: null | { [index: string]: string; };
  options_: null | { [index: string]: any; };

  constructor (hostname: string,
               credentials?: null | { [index: string]: string; },
               options?: null | { [index: string]: any; }) {
    if (!options) options = {};
    if (!credentials) credentials = {};
    options['format'] = 'text';

    this.client_ = new grpcWeb.GrpcWebClientBase(options);
    this.hostname_ = hostname;
    this.credentials_ = credentials;
    this.options_ = options;
  }

  methodInfoClientStream = new grpcWeb.AbstractClientBase.MethodInfo(
    client_pb.ClientResponse,
    (request: common_pb.EmptyRequest) => {
      return request.serializeBinary();
    },
    client_pb.ClientResponse.deserializeBinary
  );

  clientStream(
    request: common_pb.EmptyRequest,
    metadata?: grpcWeb.Metadata) {
    return this.client_.serverStreaming(
      this.hostname_ +
        '/PongBackend.v1.client.Client/ClientStream',
      request,
      metadata || {},
      this.methodInfoClientStream);
  }

  methodInfoClientPong = new grpcWeb.AbstractClientBase.MethodInfo(
    common_pb.EmptyResponse,
    (request: common_pb.Pong) => {
      return request.serializeBinary();
    },
    common_pb.EmptyResponse.deserializeBinary
  );

  clientPong(
    request: common_pb.Pong,
    metadata: grpcWeb.Metadata | null): Promise<common_pb.EmptyResponse>;

  clientPong(
    request: common_pb.Pong,
    metadata: grpcWeb.Metadata | null,
    callback: (err: grpcWeb.Error,
               response: common_pb.EmptyResponse) => void): grpcWeb.ClientReadableStream<common_pb.EmptyResponse>;

  clientPong(
    request: common_pb.Pong,
    metadata: grpcWeb.Metadata | null,
    callback?: (err: grpcWeb.Error,
               response: common_pb.EmptyResponse) => void) {
    if (callback !== undefined) {
      return this.client_.rpcCall(
        this.hostname_ +
          '/PongBackend.v1.client.Client/ClientPong',
        request,
        metadata || {},
        this.methodInfoClientPong,
        callback);
    }
    return this.client_.unaryCall(
    this.hostname_ +
      '/PongBackend.v1.client.Client/ClientPong',
    request,
    metadata || {},
    this.methodInfoClientPong);
  }

}

