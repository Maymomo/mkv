#pragma once

#include <memory>
#include <string>

#include "mkv/server/server.h"

namespace mkv
{
namespace server
{
template <class GrpcService>
class ServerBuilder
{
public:
    ServerBuilder() : service(new GrpcService()) {}
    std::unique_ptr<Server<GrpcService>> build_server()
    {
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(service.get());
        if (concurrent <= 0)
        {
            return {};
        }
        std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> cqs;
        for (std::size_t i = 0; i < concurrent; i++)
        {
            cqs.emplace_back(builder.AddCompletionQueue());
        }

        server = builder.BuildAndStart();
        if (!server)
            return {};
        return {std::move(server), std::move(cqs), std::move(service)};
    }

    void listen_on(std::string address)
    {
        server_address = std::move(address);
    }

    void set_concurrent(std::size_t num)
    {
        concurrent = num;
    }

private:
    std::size_t concurrent;
    std::string server_address;
    grpc::ServerBuilder builder;
    std::unique_ptr<GrpcService> service;
};
} // namespace server
} // namespace mkv
