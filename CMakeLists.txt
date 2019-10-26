cmake_minimum_required(VERSION 3.5.1)
project(mkv CXX C)
include(cmake/grpc.cmake)
include(cmake/mkvproto.cmake)


set(CMAKE_CXX_STANDARD 17)

set(mkv_server_src
  include/mkv/server/server.h
  include/mkv/server/server_builder.h
  include/mkv/server/call.h
)

set(mkv_service_src
  include/mkv/service/mkv_service/mkv_service.h
  src/mkv/service/mkv_service/mkv_service.cc
)


include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(mkv_service STATIC ${mkv_server_src} ${mkv_service_src})
target_link_libraries(mkv_service ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF})
