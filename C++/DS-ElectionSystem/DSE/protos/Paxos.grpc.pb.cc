// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: Paxos.proto

#include "Paxos.pb.h"
#include "Paxos.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace protos {

static const char* PaxosGreeter_method_names[] = {
  "/protos.PaxosGreeter/UponReceivingInit",
  "/protos.PaxosGreeter/UponReceivingPrepare",
  "/protos.PaxosGreeter/UponReceivingAccept",
  "/protos.PaxosGreeter/UponReceivingCommit",
};

std::unique_ptr< PaxosGreeter::Stub> PaxosGreeter::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< PaxosGreeter::Stub> stub(new PaxosGreeter::Stub(channel, options));
  return stub;
}

PaxosGreeter::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_UponReceivingInit_(PaxosGreeter_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UponReceivingPrepare_(PaxosGreeter_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UponReceivingAccept_(PaxosGreeter_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UponReceivingCommit_(PaxosGreeter_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status PaxosGreeter::Stub::UponReceivingInit(::grpc::ClientContext* context, const ::protos::Init& request, ::protos::Init* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protos::Init, ::protos::Init, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UponReceivingInit_, context, request, response);
}

void PaxosGreeter::Stub::async::UponReceivingInit(::grpc::ClientContext* context, const ::protos::Init* request, ::protos::Init* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protos::Init, ::protos::Init, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingInit_, context, request, response, std::move(f));
}

void PaxosGreeter::Stub::async::UponReceivingInit(::grpc::ClientContext* context, const ::protos::Init* request, ::protos::Init* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingInit_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protos::Init>* PaxosGreeter::Stub::PrepareAsyncUponReceivingInitRaw(::grpc::ClientContext* context, const ::protos::Init& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protos::Init, ::protos::Init, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UponReceivingInit_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protos::Init>* PaxosGreeter::Stub::AsyncUponReceivingInitRaw(::grpc::ClientContext* context, const ::protos::Init& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUponReceivingInitRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PaxosGreeter::Stub::UponReceivingPrepare(::grpc::ClientContext* context, const ::protos::Prepare& request, ::protos::Promise* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protos::Prepare, ::protos::Promise, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UponReceivingPrepare_, context, request, response);
}

void PaxosGreeter::Stub::async::UponReceivingPrepare(::grpc::ClientContext* context, const ::protos::Prepare* request, ::protos::Promise* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protos::Prepare, ::protos::Promise, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingPrepare_, context, request, response, std::move(f));
}

void PaxosGreeter::Stub::async::UponReceivingPrepare(::grpc::ClientContext* context, const ::protos::Prepare* request, ::protos::Promise* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingPrepare_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protos::Promise>* PaxosGreeter::Stub::PrepareAsyncUponReceivingPrepareRaw(::grpc::ClientContext* context, const ::protos::Prepare& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protos::Promise, ::protos::Prepare, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UponReceivingPrepare_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protos::Promise>* PaxosGreeter::Stub::AsyncUponReceivingPrepareRaw(::grpc::ClientContext* context, const ::protos::Prepare& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUponReceivingPrepareRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PaxosGreeter::Stub::UponReceivingAccept(::grpc::ClientContext* context, const ::protos::Accept& request, ::protos::Accepted* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protos::Accept, ::protos::Accepted, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UponReceivingAccept_, context, request, response);
}

void PaxosGreeter::Stub::async::UponReceivingAccept(::grpc::ClientContext* context, const ::protos::Accept* request, ::protos::Accepted* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protos::Accept, ::protos::Accepted, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingAccept_, context, request, response, std::move(f));
}

void PaxosGreeter::Stub::async::UponReceivingAccept(::grpc::ClientContext* context, const ::protos::Accept* request, ::protos::Accepted* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingAccept_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protos::Accepted>* PaxosGreeter::Stub::PrepareAsyncUponReceivingAcceptRaw(::grpc::ClientContext* context, const ::protos::Accept& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protos::Accepted, ::protos::Accept, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UponReceivingAccept_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protos::Accepted>* PaxosGreeter::Stub::AsyncUponReceivingAcceptRaw(::grpc::ClientContext* context, const ::protos::Accept& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUponReceivingAcceptRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PaxosGreeter::Stub::UponReceivingCommit(::grpc::ClientContext* context, const ::protos::Commit& request, ::protos::Finish* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protos::Commit, ::protos::Finish, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UponReceivingCommit_, context, request, response);
}

void PaxosGreeter::Stub::async::UponReceivingCommit(::grpc::ClientContext* context, const ::protos::Commit* request, ::protos::Finish* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protos::Commit, ::protos::Finish, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingCommit_, context, request, response, std::move(f));
}

void PaxosGreeter::Stub::async::UponReceivingCommit(::grpc::ClientContext* context, const ::protos::Commit* request, ::protos::Finish* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UponReceivingCommit_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protos::Finish>* PaxosGreeter::Stub::PrepareAsyncUponReceivingCommitRaw(::grpc::ClientContext* context, const ::protos::Commit& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protos::Finish, ::protos::Commit, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UponReceivingCommit_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protos::Finish>* PaxosGreeter::Stub::AsyncUponReceivingCommitRaw(::grpc::ClientContext* context, const ::protos::Commit& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUponReceivingCommitRaw(context, request, cq);
  result->StartCall();
  return result;
}

PaxosGreeter::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PaxosGreeter_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PaxosGreeter::Service, ::protos::Init, ::protos::Init, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PaxosGreeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protos::Init* req,
             ::protos::Init* resp) {
               return service->UponReceivingInit(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PaxosGreeter_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PaxosGreeter::Service, ::protos::Prepare, ::protos::Promise, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PaxosGreeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protos::Prepare* req,
             ::protos::Promise* resp) {
               return service->UponReceivingPrepare(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PaxosGreeter_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PaxosGreeter::Service, ::protos::Accept, ::protos::Accepted, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PaxosGreeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protos::Accept* req,
             ::protos::Accepted* resp) {
               return service->UponReceivingAccept(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PaxosGreeter_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PaxosGreeter::Service, ::protos::Commit, ::protos::Finish, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PaxosGreeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protos::Commit* req,
             ::protos::Finish* resp) {
               return service->UponReceivingCommit(ctx, req, resp);
             }, this)));
}

PaxosGreeter::Service::~Service() {
}

::grpc::Status PaxosGreeter::Service::UponReceivingInit(::grpc::ServerContext* context, const ::protos::Init* request, ::protos::Init* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PaxosGreeter::Service::UponReceivingPrepare(::grpc::ServerContext* context, const ::protos::Prepare* request, ::protos::Promise* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PaxosGreeter::Service::UponReceivingAccept(::grpc::ServerContext* context, const ::protos::Accept* request, ::protos::Accepted* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PaxosGreeter::Service::UponReceivingCommit(::grpc::ServerContext* context, const ::protos::Commit* request, ::protos::Finish* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace protos

