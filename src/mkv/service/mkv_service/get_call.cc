#include "mkv/service/mkv_service/get_call.h"

namespace mkv {

namespace service {

namespace mkv_service {

void GetCall::start_call() {
   service->Requestget(ctx.get(), &request, responder.get(), cq, cq,
                                  this);
}

void GetCall::process(bool success) {
    response.set_key(request.key());
    response.set_value(request.key());
    finish();
}



void GetCall::finish_call() {
    responder->Finish(response, grpc::Status::OK, this);
}

}

}

}
