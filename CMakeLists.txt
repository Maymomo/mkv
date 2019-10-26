cmake_minimum_required(VERSION 3.5.1)
project(mkv CXX C)
include(cmake/grpc.cmake)
include(cmake/mkvproto.cmake)


set(mkv_server_src
  include/mkv/server/server.h
  include/mkv/server/server_builder.h
  include/mkv/server/call.h
)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(mkv_server STATIC ${mkv_server_src})
target_link_libraries(mkv_server ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF})
