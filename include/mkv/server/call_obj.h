#pragma once

#include "grpcpp/grpcpp.h"

#include "mkv/server/call.h"

namespace mkv {

namespace server {

template <class GrpcService, class Request, class Response>
class CallObj : public mkv::server::Call<GrpcService> {
public:
    CallObj(GrpcService *service) : service(service) {}

    void start_async_call() final override {
        ctx = std::make_unique<grpc::ServerContext>();
        responder = std::make_unique<grpc::ServerAsyncResponseWriter<Response>>(ctx.get());
        request.Clear();
        response.Clear();
    }
    
    virtual void start_call() = 0;

    void finish_async_call() final override {
        finish_call();
    }

    virtual void finish_call() = 0;

protected:
    Request request;
    Response response;
    std::unique_ptr<grpc::ServerAsyncResponseWriter<Response>> responder;
    std::unique_ptr<grpc::ServerContext> ctx;
    GrpcService *service;
};

}

}
