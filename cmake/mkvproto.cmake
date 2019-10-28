set(MKVPROTO_ROOT_DIR "third_party/mkvproto")
set(MKVPROTO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/mkvproto")
get_filename_component(mkv_proto "${MKVPROTO_ROOT_DIR}/proto/mkv.proto" ABSOLUTE)
get_filename_component(mkv_proto_path "${mkv_proto}" PATH)


# Generated sources
set(mkv_proto_srcs "${MKVPROTO_OUTPUT_DIR}/mkv.pb.cc")
set(mkv_proto_hdrs "${MKVPROTO_OUTPUT_DIR}/mkv.pb.h")
set(mkv_grpc_srcs "${MKVPROTO_OUTPUT_DIR}/mkv.grpc.pb.cc")
set(mkv_grpc_hdrs "${MKVPROTO_OUTPUT_DIR}/mkv.grpc.pb.h")


set (mkv_proto_all "${mkv_proto_srcs}" "${mkv_proto_hdrs}" "${mkv_grpc_srcs}" "${mkv_grpc_hdrs}")

add_custom_command(
      OUTPUT "${mkv_proto_srcs}" "${mkv_proto_hdrs}" "${mkv_grpc_srcs}" "${mkv_grpc_hdrs}"
      COMMAND mkdir -p ${MKVPROTO_OUTPUT_DIR}
      COMMAND ${_PROTOBUF_PROTOC}
                    --grpc_out "${MKVPROTO_OUTPUT_DIR}"
                    --cpp_out "${MKVPROTO_OUTPUT_DIR}"
                    -I "${mkv_proto_path}"
                    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
                    "${mkv_proto}"
		    DEPENDS "${mkv_proto}" "${_PROTOBUF_PROTOC}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(mkvproto STATIC "${mkv_proto_all}")
target_link_libraries(mkvproto ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF})
