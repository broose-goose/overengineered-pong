import * as jspb from 'google-protobuf'



export class EmptyRequest extends jspb.Message {
  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): EmptyRequest.AsObject;
  static toObject(includeInstance: boolean, msg: EmptyRequest): EmptyRequest.AsObject;
  static serializeBinaryToWriter(message: EmptyRequest, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): EmptyRequest;
  static deserializeBinaryFromReader(message: EmptyRequest, reader: jspb.BinaryReader): EmptyRequest;
}

export namespace EmptyRequest {
  export type AsObject = {
  }
}

export class EmptyResponse extends jspb.Message {
  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): EmptyResponse.AsObject;
  static toObject(includeInstance: boolean, msg: EmptyResponse): EmptyResponse.AsObject;
  static serializeBinaryToWriter(message: EmptyResponse, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): EmptyResponse;
  static deserializeBinaryFromReader(message: EmptyResponse, reader: jspb.BinaryReader): EmptyResponse;
}

export namespace EmptyResponse {
  export type AsObject = {
  }
}

export class Ping extends jspb.Message {
  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): Ping.AsObject;
  static toObject(includeInstance: boolean, msg: Ping): Ping.AsObject;
  static serializeBinaryToWriter(message: Ping, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): Ping;
  static deserializeBinaryFromReader(message: Ping, reader: jspb.BinaryReader): Ping;
}

export namespace Ping {
  export type AsObject = {
  }
}

export class Pong extends jspb.Message {
  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): Pong.AsObject;
  static toObject(includeInstance: boolean, msg: Pong): Pong.AsObject;
  static serializeBinaryToWriter(message: Pong, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): Pong;
  static deserializeBinaryFromReader(message: Pong, reader: jspb.BinaryReader): Pong;
}

export namespace Pong {
  export type AsObject = {
  }
}

export class Connected extends jspb.Message {
  getConnected(): boolean;
  setConnected(value: boolean): Connected;

  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): Connected.AsObject;
  static toObject(includeInstance: boolean, msg: Connected): Connected.AsObject;
  static serializeBinaryToWriter(message: Connected, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): Connected;
  static deserializeBinaryFromReader(message: Connected, reader: jspb.BinaryReader): Connected;
}

export namespace Connected {
  export type AsObject = {
    connected: boolean,
  }
}

export class Position extends jspb.Message {
  getPosition(): number;
  setPosition(value: number): Position;

  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): Position.AsObject;
  static toObject(includeInstance: boolean, msg: Position): Position.AsObject;
  static serializeBinaryToWriter(message: Position, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): Position;
  static deserializeBinaryFromReader(message: Position, reader: jspb.BinaryReader): Position;
}

export namespace Position {
  export type AsObject = {
    position: number,
  }
}

