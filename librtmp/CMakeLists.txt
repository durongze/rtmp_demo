cmake_minimum_required(VERSION 3.4.1)
# close openssl
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")

file(GLOB RTMP_SRC *.c)

include_directories( rtmp.lib E:/program/zlib/include/ E:/program/openssl/include/ )
add_library(rtmp.lib 
            STATIC
            ${RTMP_SRC}
)