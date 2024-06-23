// ServerInterceptor.hpp
#pragma once
#include <grpcpp/grpcpp.h>
#include "SharedRestrictions.hpp"

class ServerInterceptorImpl : public grpc::experimental::Interceptor {
public:
    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override;
};

class ServerInterceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface {
public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override;
    void AddRestrict(const std::string& client, const std::string& procedure);
};