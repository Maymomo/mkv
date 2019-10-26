#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>

#include "grpcpp/grpcpp.h"

#include "mkv/server/call.h"

namespace mkv
{
namespace server
{
template <class GrpcService>
class Server
{
public:
    Server(std::unique_ptr<grpc::Server> &&server, std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> &&cqs, std::unique_ptr<GrpcService> &&service)
        : server(std::move(server)), cqs(std::move(cqs)), service(std::move(service))
    {
    }

    void register_calls(std::vector<std::shared_ptr<Call<GrpcService>>> calls) {}

    void start()
    {
        for (auto &cq : cqs)
        {
            threads.emplace_back([this] {
                void *tag;
                bool ok;
                while (cq->Next(&tag, &ok))
                {
                    static_cast<Call<GrpcService> *>(tag)->handle(ok);
                }
            });
        }
        for (auto &calls : call_map)
        {
            for (auto &call : calls)
            {
                call->start();
            }
        }
    }
    
    GrpcService *grpc_service() {
        return service.get();
    }

    ~Server()
    {
        shutdown();
    }

    void shutdown()
    {
        for (auto &cq : cqs)
        {
            cq->Shutdown();
        }
        for (auto &t : threads)
        {
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
