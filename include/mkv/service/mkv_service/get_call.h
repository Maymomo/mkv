#pragma once

#include "mkvproto/mkv.grpc.pb.h"

#include "mkv/server/call_obj.h"

namespace mkv {

namespace service {

namespace mkv_service {

class GetCall : public mkv::server::CallObj<mkvproto::Mkv::AsyncService, mkvproto::RequestGet, mkvproto::ResponseGet> {
public:
    GetCall(mkvproto::Mkv::AsyncService *service) : mkv::server::CallObj<mkvproto::Mkv::AsyncService, mkvproto::RequestGet, mkvproto::ResponseGet>(service) {}
    void start_call() override;

    void process(bool success) override;

    void finish_call() override;

private:
};

}

}

}
