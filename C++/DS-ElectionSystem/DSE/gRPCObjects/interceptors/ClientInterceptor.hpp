#ifndef CLIENT_INTERCEPTOR_HPP
#define CLIENT_INTERCEPTOR_HPP

#include <grpcpp/grpcpp.h>
#include <string>

class ClientInterceptorImpl : public grpc::experimental::Interceptor {
public:
    ClientInterceptorImpl(int channel);
    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override;

private:
    int channel_;
};

class ClientInterceptorFactory : public grpc::experimental::ClientInterceptorFactoryInterface {
public:
    ClientInterceptorFactory(int channel);
    grpc::experimental::Interceptor* CreateClientInterceptor(grpc::experimental::ClientRpcInfo* info) override;

private:
    int channel_;
};

#endif // CLIENT_INTERCEPTOR_HPP