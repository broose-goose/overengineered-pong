// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.26.0
// 	protoc        v3.19.1
// source: embedded.proto

package protoEmbedded

import (
	common "github.com/polis-interactive/Pong/pong-backend/api/v1/go/common"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type EmbeddedRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Types that are assignable to Request:
	//	*EmbeddedRequest_Pong
	//	*EmbeddedRequest_Position
	Request isEmbeddedRequest_Request `protobuf_oneof:"request"`
}

func (x *EmbeddedRequest) Reset() {
	*x = EmbeddedRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_embedded_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *EmbeddedRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*EmbeddedRequest) ProtoMessage() {}

func (x *EmbeddedRequest) ProtoReflect() protoreflect.Message {
	mi := &file_embedded_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use EmbeddedRequest.ProtoReflect.Descriptor instead.
func (*EmbeddedRequest) Descriptor() ([]byte, []int) {
	return file_embedded_proto_rawDescGZIP(), []int{0}
}

func (m *EmbeddedRequest) GetRequest() isEmbeddedRequest_Request {
	if m != nil {
		return m.Request
	}
	return nil
}

func (x *EmbeddedRequest) GetPong() *common.Pong {
	if x, ok := x.GetRequest().(*EmbeddedRequest_Pong); ok {
		return x.Pong
	}
	return nil
}

func (x *EmbeddedRequest) GetPosition() *common.Position {
	if x, ok := x.GetRequest().(*EmbeddedRequest_Position); ok {
		return x.Position
	}
	return nil
}

type isEmbeddedRequest_Request interface {
	isEmbeddedRequest_Request()
}

type EmbeddedRequest_Pong struct {
	Pong *common.Pong `protobuf:"bytes,1,opt,name=pong,proto3,oneof"`
}

type EmbeddedRequest_Position struct {
	Position *common.Position `protobuf:"bytes,2,opt,name=position,proto3,oneof"`
}

func (*EmbeddedRequest_Pong) isEmbeddedRequest_Request() {}

func (*EmbeddedRequest_Position) isEmbeddedRequest_Request() {}

type EmbeddedResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Types that are assignable to Response:
	//	*EmbeddedResponse_Ping
	//	*EmbeddedResponse_Connected
	Response isEmbeddedResponse_Response `protobuf_oneof:"response"`
}

func (x *EmbeddedResponse) Reset() {
	*x = EmbeddedResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_embedded_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *EmbeddedResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*EmbeddedResponse) ProtoMessage() {}

func (x *EmbeddedResponse) ProtoReflect() protoreflect.Message {
	mi := &file_embedded_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use EmbeddedResponse.ProtoReflect.Descriptor instead.
func (*EmbeddedResponse) Descriptor() ([]byte, []int) {
	return file_embedded_proto_rawDescGZIP(), []int{1}
}

func (m *EmbeddedResponse) GetResponse() isEmbeddedResponse_Response {
	if m != nil {
		return m.Response
	}
	return nil
}

func (x *EmbeddedResponse) GetPing() *common.Ping {
	if x, ok := x.GetResponse().(*EmbeddedResponse_Ping); ok {
		return x.Ping
	}
	return nil
}

func (x *EmbeddedResponse) GetConnected() *common.Connected {
	if x, ok := x.GetResponse().(*EmbeddedResponse_Connected); ok {
		return x.Connected
	}
	return nil
}

type isEmbeddedResponse_Response interface {
	isEmbeddedResponse_Response()
}

type EmbeddedResponse_Ping struct {
	Ping *common.Ping `protobuf:"bytes,1,opt,name=ping,proto3,oneof"`
}

type EmbeddedResponse_Connected struct {
	Connected *common.Connected `protobuf:"bytes,2,opt,name=connected,proto3,oneof"`
}

func (*EmbeddedResponse_Ping) isEmbeddedResponse_Response() {}

func (*EmbeddedResponse_Connected) isEmbeddedResponse_Response() {}

var File_embedded_proto protoreflect.FileDescriptor

var file_embedded_proto_rawDesc = []byte{
	0x0a, 0x0e, 0x65, 0x6d, 0x62, 0x65, 0x64, 0x64, 0x65, 0x64, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x12, 0x17, 0x50, 0x6f, 0x6e, 0x67, 0x42, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64, 0x2e, 0x76, 0x31,
	0x2e, 0x65, 0x6d, 0x62, 0x65, 0x64, 0x64, 0x65, 0x64, 0x1a, 0x0c, 0x63, 0x6f, 0x6d, 0x6d, 0x6f,
	0x6e, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22, 0x8e, 0x01, 0x0a, 0x0f, 0x45, 0x6d, 0x62, 0x65,
	0x64, 0x64, 0x65, 0x64, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x31, 0x0a, 0x04, 0x70,
	0x6f, 0x6e, 0x67, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x1b, 0x2e, 0x50, 0x6f, 0x6e, 0x67,
	0x42, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64, 0x2e, 0x76, 0x31, 0x2e, 0x63, 0x6f, 0x6d, 0x6d, 0x6f,
	0x6e, 0x2e, 0x50, 0x6f, 0x6e, 0x67, 0x48, 0x00, 0x52, 0x04, 0x70, 0x6f, 0x6e, 0x67, 0x12, 0x3d,
	0x0a, 0x08, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b,
	0x32, 0x1f, 0x2e, 0x50, 0x6f, 0x6e, 0x67, 0x42, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64, 0x2e, 0x76,
	0x31, 0x2e, 0x63, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2e, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f,
	0x6e, 0x48, 0x00, 0x52, 0x08, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x42, 0x09, 0x0a,
	0x07, 0x72, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x22, 0x93, 0x01, 0x0a, 0x10, 0x45, 0x6d, 0x62,
	0x65, 0x64, 0x64, 0x65, 0x64, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x12, 0x31, 0x0a,
	0x04, 0x70, 0x69, 0x6e, 0x67, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x1b, 0x2e, 0x50, 0x6f,
	0x6e, 0x67, 0x42, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64, 0x2e, 0x76, 0x31, 0x2e, 0x63, 0x6f, 0x6d,
	0x6d, 0x6f, 0x6e, 0x2e, 0x50, 0x69, 0x6e, 0x67, 0x48, 0x00, 0x52, 0x04, 0x70, 0x69, 0x6e, 0x67,
	0x12, 0x40, 0x0a, 0x09, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x65, 0x64, 0x18, 0x02, 0x20,
	0x01, 0x28, 0x0b, 0x32, 0x20, 0x2e, 0x50, 0x6f, 0x6e, 0x67, 0x42, 0x61, 0x63, 0x6b, 0x65, 0x6e,
	0x64, 0x2e, 0x76, 0x31, 0x2e, 0x63, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2e, 0x43, 0x6f, 0x6e, 0x6e,
	0x65, 0x63, 0x74, 0x65, 0x64, 0x48, 0x00, 0x52, 0x09, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74,
	0x65, 0x64, 0x42, 0x0a, 0x0a, 0x08, 0x72, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x42, 0x51,
	0x5a, 0x4f, 0x67, 0x69, 0x74, 0x68, 0x75, 0x62, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x70, 0x6f, 0x6c,
	0x69, 0x73, 0x2d, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x2f, 0x50,
	0x6f, 0x6e, 0x67, 0x2f, 0x70, 0x6f, 0x6e, 0x67, 0x2d, 0x62, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64,
	0x2f, 0x61, 0x70, 0x69, 0x2f, 0x76, 0x31, 0x2f, 0x67, 0x6f, 0x2f, 0x65, 0x6d, 0x62, 0x65, 0x64,
	0x64, 0x65, 0x64, 0x3b, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x45, 0x6d, 0x62, 0x65, 0x64, 0x64, 0x65,
	0x64, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_embedded_proto_rawDescOnce sync.Once
	file_embedded_proto_rawDescData = file_embedded_proto_rawDesc
)

func file_embedded_proto_rawDescGZIP() []byte {
	file_embedded_proto_rawDescOnce.Do(func() {
		file_embedded_proto_rawDescData = protoimpl.X.CompressGZIP(file_embedded_proto_rawDescData)
	})
	return file_embedded_proto_rawDescData
}

var file_embedded_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_embedded_proto_goTypes = []interface{}{
	(*EmbeddedRequest)(nil),  // 0: PongBackend.v1.embedded.EmbeddedRequest
	(*EmbeddedResponse)(nil), // 1: PongBackend.v1.embedded.EmbeddedResponse
	(*common.Pong)(nil),      // 2: PongBackend.v1.common.Pong
	(*common.Position)(nil),  // 3: PongBackend.v1.common.Position
	(*common.Ping)(nil),      // 4: PongBackend.v1.common.Ping
	(*common.Connected)(nil), // 5: PongBackend.v1.common.Connected
}
var file_embedded_proto_depIdxs = []int32{
	2, // 0: PongBackend.v1.embedded.EmbeddedRequest.pong:type_name -> PongBackend.v1.common.Pong
	3, // 1: PongBackend.v1.embedded.EmbeddedRequest.position:type_name -> PongBackend.v1.common.Position
	4, // 2: PongBackend.v1.embedded.EmbeddedResponse.ping:type_name -> PongBackend.v1.common.Ping
	5, // 3: PongBackend.v1.embedded.EmbeddedResponse.connected:type_name -> PongBackend.v1.common.Connected
	4, // [4:4] is the sub-list for method output_type
	4, // [4:4] is the sub-list for method input_type
	4, // [4:4] is the sub-list for extension type_name
	4, // [4:4] is the sub-list for extension extendee
	0, // [0:4] is the sub-list for field type_name
}

func init() { file_embedded_proto_init() }
func file_embedded_proto_init() {
	if File_embedded_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_embedded_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*EmbeddedRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_embedded_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*EmbeddedResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	file_embedded_proto_msgTypes[0].OneofWrappers = []interface{}{
		(*EmbeddedRequest_Pong)(nil),
		(*EmbeddedRequest_Position)(nil),
	}
	file_embedded_proto_msgTypes[1].OneofWrappers = []interface{}{
		(*EmbeddedResponse_Ping)(nil),
		(*EmbeddedResponse_Connected)(nil),
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_embedded_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_embedded_proto_goTypes,
		DependencyIndexes: file_embedded_proto_depIdxs,
		MessageInfos:      file_embedded_proto_msgTypes,
	}.Build()
	File_embedded_proto = out.File
	file_embedded_proto_rawDesc = nil
	file_embedded_proto_goTypes = nil
	file_embedded_proto_depIdxs = nil
}