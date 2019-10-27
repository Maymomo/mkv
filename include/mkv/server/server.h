#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>

#include "grpcpp/grpcpp.h"

#include "mkv/server/call.h"

namespace mkv {
namespace server {
template <class GrpcService>
class Server {
public:
    Server(std::unique_ptr<grpc::Server> &&server, std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> &&cqs, std::unique_ptr<GrpcService> &&service)
        : server(std::move(server)), cqs(std::move(cqs)), service(std::move(service)) {
    }

    void register_calls(std::vector<CallPtr<GrpcService>> calls) {
        std::size_t cqs_size = cqs.size();
        std::size_t calls_size  = calls.size();
        for (std::size_t call_index = 0; call_index < calls_size; call_index++) {
            calls[call_index]->bind_complete_queue(cqs[call_index % cqs_size].get());
        }
        call_map.emplace_back(std::move(calls));
    }

    void start() {
        for (std::size_t index = 0;  index < cqs.size(); index++) {
            threads.emplace_back([this, index] {
                auto &cq = cqs[index];
                void *tag;
                bool ok;
                while (cq->Next(&tag, &ok)) {
                    static_cast<Call<GrpcService> *>(tag)->handle(ok);
                }
            });
        }
        for (auto &calls : call_map) {
            for (auto &call : calls) {
                call->start();
            }
        }
    }

    GrpcService *grpc_service() {
        return service.get();
    }

    ~Server() {
        shutdown();
    }

    void shutdown() {
        for (auto &cq : cqs) {
            cq->Shutdown();
        }
        for (auto &t : threads) {
            t.join();
        }
    }

private:
    std::unique_ptr<grpc::Server> server;
    std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> cqs;
    std::vector<std::vector<CallPtr<GrpcService>>> call_map;
    std::unique_ptr<GrpcService> service;
    std::vector<std::thread> threads;
};

} // namespace server
} // namespace mkv
