cmake_minimum_required(VERSION 3.4.1)

project(rtmpsvr)

add_compile_options(-DNO_CRYPTO)

include(cmake/dep_zlib.cmake)
include(cmake/dep_openssl.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")

file(GLOB_RECURSE RTMP_HDRS_LIST  ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/*.h)
file(GLOB_RECURSE RTMP_SRCS_LIST  ${CMAKE_CURRENT_SOURCE_DIR}/librtmp/*.c)

#list(REMOVE_ITEM  RTMP_SRCS_LIST src/xxx.c )

source_group(TREE ${PROJECT_SOURCE_DIR} FILES  ${LUA_SRCS_LIST} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/librtmp)

add_library(rtmp_static STATIC ${RTMP_SRCS_LIST})
add_library(rtmp_shared SHARED ${RTMP_SRCS_LIST})
target_link_libraries(rtmp_shared    ws2_32 gdi32 ${SSL_LIBRARIES})

set_target_properties(rtmp_shared PROPERTIES PUBLIC_HEADER "${RTMP_HDRS_LIST}")

SET_TARGET_PROPERTIES(rtmp_static PROPERTIES  CLEAN_DIRECT_OUTPUT 1  OUTPUT_NAME "librtmp"  )
SET_TARGET_PROPERTIES(rtmp_shared PROPERTIES  CLEAN_DIRECT_OUTPUT 1  OUTPUT_NAME "librtmp"  )

file(GLOB RTMP_COMMON_SRCS_LIST ${RTMP_SRCS_LIST}  thread*.c*  getopt*.c*)

add_executable(rtmppublish rtmppublish.c ${RTMP_COMMON_SRCS_LIST} )
target_link_libraries(rtmppublish ws2_32 gdi32 ${SSL_LIBRARIES})

add_executable(rtmpdump rtmpdump.c       ${RTMP_COMMON_SRCS_LIST} )
target_link_libraries(rtmpdump    ws2_32 gdi32 ${SSL_LIBRARIES})

add_executable(rtmpgw rtmpgw.c           ${RTMP_COMMON_SRCS_LIST} )
target_link_libraries(rtmpgw      ws2_32 gdi32 ${SSL_LIBRARIES})

add_executable(rtmpsrv rtmpsrv.c         ${RTMP_COMMON_SRCS_LIST} )
target_link_libraries(rtmpsrv     ws2_32 gdi32 ${SSL_LIBRARIES})

add_executable(rtmpsuck rtmpsuck.c       ${RTMP_COMMON_SRCS_LIST} )
target_link_libraries(rtmpsuck    ws2_32 gdi32 ${SSL_LIBRARIES})