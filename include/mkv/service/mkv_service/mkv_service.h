#pragma once

#include <memory>

#include "mkvproto/mkv.grpc.pb.h"
#include "mkv/server/server.h"
#include "mkv/server/server_builder.h"

namespace mkv {

namespace service {

struct MkvServiceConfig {
    std::string server_address;
    std::size_t per_call_num;
    std::size_t concurrent_num;
};

class MkvService {
public:
    MkvService(MkvServiceConfig config) : config(config) {}

    bool start() {
        mkv::server::ServerBuilder<mkvproto::Mkv::AsyncService> server_builder;
        server_builder.listen_on(config.server_address);
        server_builder.set_concurrent(config.concurrent_num);
        server = server_builder.build_server();
        if (!server) {
            return false;
        }
        // call all request
        /*
         * for () {
         *  calls = build_calls();
         *  server->register_calls(calls);
         * }
         */
        server->start();
    }

    void shutdown() {
        if (server) {
            server->shutdown();
        }
    }

private:
    MkvServiceConfig config;
    std::unique_ptr<mkv::server::Server<mkvproto::Mkv::AsyncService>> server;
};
}
}
