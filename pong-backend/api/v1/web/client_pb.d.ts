import * as jspb from 'google-protobuf'

import * as common_pb from './common_pb';


export class ClientResponse extends jspb.Message {
  getPing(): common_pb.Ping | undefined;
  setPing(value?: common_pb.Ping): ClientResponse;
  hasPing(): boolean;
  clearPing(): ClientResponse;

  getConnected(): common_pb.Connected | undefined;
  setConnected(value?: common_pb.Connected): ClientResponse;
  hasConnected(): boolean;
  clearConnected(): ClientResponse;

  getPosition(): common_pb.Position | undefined;
  setPosition(value?: common_pb.Position): ClientResponse;
  hasPosition(): boolean;
  clearPosition(): ClientResponse;

  getResponseCase(): ClientResponse.ResponseCase;

  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): ClientResponse.AsObject;
  static toObject(includeInstance: boolean, msg: ClientResponse): ClientResponse.AsObject;
  static serializeBinaryToWriter(message: ClientResponse, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): ClientResponse;
  static deserializeBinaryFromReader(message: ClientResponse, reader: jspb.BinaryReader): ClientResponse;
}

export namespace ClientResponse {
  export type AsObject = {
    ping?: common_pb.Ping.AsObject,
    connected?: common_pb.Connected.AsObject,
    position?: common_pb.Position.AsObject,
  }

  export enum ResponseCase { 
    RESPONSE_NOT_SET = 0,
    PING = 1,
    CONNECTED = 2,
    POSITION = 3,
  }
}

