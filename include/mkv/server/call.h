#pragma once

#include <memory>
#include "grpcpp/grpcpp.h"

namespace mkv
{
namespace server
{

template <class GrpcService>
class Call
{
public:
    Call(GrpcService *service, grpc::ServerCompletionQueue *cq)
        : service(service), cq(cq), status(CallStatus::CREATE)
    {
    }

    virtual void process(bool success);

    virtual void start_call();

    virtual void finish_call();

    void finish()
    {
        status = CallStatus::FINISH;
        finish_call();
    }

    void start()
    {
        status = CallStatus::CREATE;
        ctx = grpc::ServerContext();
        start_call();
    }

    void handle(bool success)
    {
        if (!success)
        {
            status = CallStatus::ERROR;
        }

        if (status == CallStatus::CREATE)
        {
            status = CallStatus::PROCESSING;
            process(success);
        }
        else if (statut == CallStatus::ERROR)
        {
            process(success);
        }
        else
        {
            start();
        }
    }

    enum class CallStatus
    {
        CREATE,
        PROCESSING,
        FINISH,
        ERROR,
    };

protected:
    GrpcService *service;
    grpc::ServerCompletionQueue *cq;
    grpc::ServerContext ctx;
    CallStatus status;
};

template <class GrpcService>
using CallPtr = std::vector<std::shared_ptr<Call<GrpcService>>>;
} // namespace server
} // namespace mkv