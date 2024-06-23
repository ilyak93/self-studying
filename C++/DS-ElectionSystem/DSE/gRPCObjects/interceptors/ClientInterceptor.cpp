#include "ClientInterceptor.hpp"

ClientInterceptorImpl::ClientInterceptorImpl(int channel) : channel_(channel) {}

void ClientInterceptorImpl::Intercept(grpc::experimental::InterceptorBatchMethods* methods) {
    if (methods->QueryInterceptionHookPoint(
            grpc::experimental::InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
        auto metadata = methods->GetSendInitialMetadata();
        if (metadata != nullptr) {
            metadata->insert(std::make_pair("channel", std::to_string(channel_)));
        }
    }
    methods->Proceed();
}

ClientInterceptorFactory::ClientInterceptorFactory(int channel) : channel_(channel) {}

grpc::experimental::Interceptor* ClientInterceptorFactory::CreateClientInterceptor(grpc::experimental::ClientRpcInfo* info) {
    return new ClientInterceptorImpl(channel_);
}