// ServerInterceptor.cpp
#include "ServerInterceptor.hpp"

void ServerInterceptorImpl::Intercept(grpc::experimental::InterceptorBatchMethods* methods) {
    if (methods->QueryInterceptionHookPoint(
            grpc::experimental::InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
        
        auto* initial_metadata = methods->GetSendInitialMetadata();
        if (initial_metadata == nullptr) {
            methods->Proceed();
            return;
        }
        
        std::string method_name;
        auto it = initial_metadata->find(":path");
        if (it != initial_metadata->end()) {
            method_name = it->second;
        }

        std::string peer;
        it = initial_metadata->find(":authority");
        if (it != initial_metadata->end()) {
            peer = it->second;
        }

        std::string clientIp = peer;
        size_t colonPos = clientIp.find_last_of(':');
        if (colonPos != std::string::npos) {
            clientIp = clientIp.substr(0, colonPos);
        }
        if (clientIp.front() == '/') {
            clientIp = clientIp.substr(1);
        }

        if (SharedRestrictions::getInstance().isRestricted(clientIp, method_name)) {
            // Deliver an empty request by not calling Proceed()
            return;
        }
    }
    methods->Proceed();
}

grpc::experimental::Interceptor* ServerInterceptorFactory::CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) {
    return new ServerInterceptorImpl();
}

void ServerInterceptorFactory::AddRestrict(const std::string& client, const std::string& procedure) {
    SharedRestrictions::getInstance().addRestrict(client, procedure);
}