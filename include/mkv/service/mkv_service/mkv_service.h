#pragma once

#include <memory>

#include "mkvproto/mkv.grpc.pb.h"
#include "mkv/server/server.h"
#include "mkv/server/server_builder.h"

namespace mkv {

namespace service {

namespace mkv_service {

struct MkvServiceConfig {
    std::string server_address;
    std::size_t per_call_num;
    std::size_t concurrent_num;
};

class MkvService {
public:
    MkvService(std::unique_ptr<mkv::server::Server<mkvproto::Mkv::AsyncService>> &&server) : server(std::move(server)) {}

    void start();

    void shutdown();
private:
    void register_call(std::vector<mkv::server::CallPtr<mkvproto::Mkv::AsyncService>> &&calls);
private:
    std::unique_ptr<mkv::server::Server<mkvproto::Mkv::AsyncService>> server;
public:
    static std::unique_ptr<MkvService> build_service(MkvServiceConfig config);
};
}
}
}
