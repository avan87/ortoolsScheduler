// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: scheduler.proto

#include "scheduler.pb.h"
#include "scheduler.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>

static const char* Schedule_method_names[] = {
  "/Schedule/GetSchedule",
};

std::unique_ptr< Schedule::Stub> Schedule::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Schedule::Stub> stub(new Schedule::Stub(channel));
  return stub;
}

Schedule::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_GetSchedule_(Schedule_method_names[0], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::ClientReader< ::Pair>* Schedule::Stub::GetScheduleRaw(::grpc::ClientContext* context, const ::Data& request) {
  return new ::grpc::ClientReader< ::Pair>(channel_.get(), rpcmethod_GetSchedule_, context, request);
}

::grpc::ClientAsyncReader< ::Pair>* Schedule::Stub::AsyncGetScheduleRaw(::grpc::ClientContext* context, const ::Data& request, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncReader< ::Pair>(channel_.get(), cq, rpcmethod_GetSchedule_, context, request, tag);
}

Schedule::Service::Service() {
  (void)Schedule_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      Schedule_method_names[0],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< Schedule::Service, ::Data, ::Pair>(
          std::mem_fn(&Schedule::Service::GetSchedule), this)));
}

Schedule::Service::~Service() {
}

::grpc::Status Schedule::Service::GetSchedule(::grpc::ServerContext* context, const ::Data* request, ::grpc::ServerWriter< ::Pair>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


