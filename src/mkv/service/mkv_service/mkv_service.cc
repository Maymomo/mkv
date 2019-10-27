#include <memory>

#include "mkv/service/mkv_service/mkv_service.h"
#include "mkv/service/mkv_service/get_call.h"

namespace mkv {

namespace service {

namespace mkv_service {


void MkvService::start() {
    server->start();
}

void MkvService::shutdown() {
    if (server) {
        server->shutdown();
    }
}


void MkvService::register_call(std::vector<mkv::server::CallPtr<mkvproto::Mkv::AsyncService>> &&calls) {
    server->register_calls(std::move(calls));
}

std::unique_ptr<MkvService> MkvService::build_service(MkvServiceConfig config) {
    mkv::server::ServerBuilder<mkvproto::Mkv::AsyncService> server_builder;
    server_builder.listen_on(config.server_address);
    server_builder.set_concurrent(config.concurrent_num);
    auto server = server_builder.build_server();
    if (!server) {
        return {};
    }
    std::vector<mkv::server::CallPtr<mkvproto::Mkv::AsyncService>> calls;
    for (std::size_t i = 0; i < config.per_call_num; i++) {
        calls.emplace_back(std::make_shared<GetCall>(server->grpc_service()));
    }
    auto service = std::make_unique<MkvService>(std::move(server));
    if (!service) {
        return {};
    }
    service->register_call(std::move(calls));
    return std::move(service);
}

}

}

}
