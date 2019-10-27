#include <unistd.h>
#include <iostream>
#include "mkv/service/mkv_service/mkv_service.h"

using namespace mkv::service;

int main(void) {
    mkv_service::MkvServiceConfig config;
    config.server_address = "127.0.0.1:8888";
    config.per_call_num = 1024;
    config.concurrent_num = 4;
    auto mkv_service = mkv_service::MkvService::build_service(config);
    mkv_service->start();
    while (true) {
        ::sleep(1);
    }
    return 0;
}
