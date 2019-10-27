#pragma once

#include <memory>
#include "grpcpp/grpcpp.h"

namespace mkv {

namespace server {

template <class GrpcService>
class Server;

template <class GrpcService>
class Call {
public:
    Call() : status(CallStatus::CREATE) {}

    virtual void process(bool success) = 0;

    virtual void start_async_call() = 0 ;

    virtual void finish_async_call() = 0;

    void finish() {
        status = CallStatus::FINISH;
        finish_async_call();
    }

    void start() {
        status = CallStatus::CREATE;
        start_async_call();
    }

    void handle(bool success) {
        if (!success) {
            status = CallStatus::ERROR;
        }

        if (status == CallStatus::CREATE) {
            status = CallStatus::PROCESSING;
            process(success);
        } else if (status == CallStatus::ERROR) {
            process(success);
        } else {
            start();
        }
    }

    enum class CallStatus {
        CREATE,
        PROCESSING,
        FINISH,
        ERROR,
    };
private:
    friend class Server<GrpcService>;
    void bind_complete_queue(grpc::ServerCompletionQueue *complete_queue) {
        cq = complete_queue;
    }

protected:
    grpc::ServerCompletionQueue *cq;
    CallStatus status;
};

template <class GrpcService>
using CallPtr = std::shared_ptr<Call<GrpcService>>;
}

}
