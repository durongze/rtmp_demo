cmake_minimum_required(VERSION 3.4.1)
add_subdirectory(librtmp)
project(rtmpsvr)

add_compile_options(-DNO_CRYPTO)

include_directories( rtmppublish ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/ E:/program/zlib/include/ E:/program/openssl/include/ )
link_directories(rtmppublish ${CMAKE_BINARY_DIR}/librtmp/Debug/ E:/program/zlib/lib/ E:/program/openssl/lib/)
add_executable(rtmppublish rtmppublish.c thread.c getopt.c )
target_link_libraries(rtmppublish ws2_32 gdi32 rtmp libssl libcrypto)

include_directories( rtmpdump ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/ E:/program/zlib/include/ E:/program/openssl/include/ )
link_directories(rtmpdump ${CMAKE_BINARY_DIR}/librtmp/Debug/ E:/program/zlib/lib/ E:/program/openssl/lib/)
add_executable(rtmpdump rtmpdump.c thread.c getopt.c )
target_link_libraries(rtmpdump ws2_32 gdi32 rtmp libssl libcrypto)

include_directories( rtmpgw ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/ E:/program/zlib/include/  E:/program/openssl/include/ )
link_directories(rtmpgw ${CMAKE_BINARY_DIR}/librtmp/Debug/ E:/program/zlib/lib/ E:/program/openssl/lib/)
add_executable(rtmpgw rtmpgw.c thread.c getopt.c )
target_link_libraries(rtmpgw ws2_32 gdi32 rtmp libssl libcrypto)

include_directories( rtmpsrv ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/ E:/program/zlib/include/  E:/program/openssl/include/ )
link_directories(rtmpsrv ${CMAKE_BINARY_DIR}/librtmp/Debug/ E:/program/zlib/lib/ E:/program/openssl/lib/)
add_executable(rtmpsrv rtmpsrv.c thread.c getopt.c )
target_link_libraries(rtmpsrv ws2_32 gdi32 rtmp libssl libcrypto)

include_directories( rtmpsuck ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/ E:/program/zlib/include/  E:/program/openssl/include/ )
link_directories(rtmpsuck ${CMAKE_BINARY_DIR}/librtmp/Debug/ E:/program/zlib/lib/ E:/program/openssl/lib/)
add_executable(rtmpsuck rtmpsuck.c thread.c getopt.c )
target_link_libraries(rtmpsuck ws2_32 gdi32 rtmp libssl libcrypto)