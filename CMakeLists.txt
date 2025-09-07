cmake_minimum_required (VERSION 3.30)

project(libraop)
include(FetchContent)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# --------------------

FetchContent_Declare(
  alac
  GIT_REPOSITORY https://github.com/macosforge/alac.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(alac)

file(
  GLOB ALAC_SOURCES
  ${alac_SOURCE_DIR}/codec/*.c
  ${alac_SOURCE_DIR}/codec/*.cpp
  ${alac_SOURCE_DIR}/codec/*.h
)

add_library(
  alac OBJECT
  ${ALAC_SOURCES}
  src/alac_wrapper/alac_wrapper.h
  src/alac_wrapper/alac_wrapper.cpp
)

target_include_directories(
  alac PUBLIC
  ${alac_SOURCE_DIR}/codec
  src/alac_wrapper
)

# --------------------

FetchContent_Declare(
  crosstools
  GIT_REPOSITORY https://github.com/philippe44/crosstools.git
  GIT_TAG        810782f
)
FetchContent_MakeAvailable(crosstools)

add_library(
  crosstools OBJECT
  ${crosstools_SOURCE_DIR}/src/platform.c
  ${crosstools_SOURCE_DIR}/src/platform.h
  ${crosstools_SOURCE_DIR}/src/cross_log.c
  ${crosstools_SOURCE_DIR}/src/cross_log.h
  ${crosstools_SOURCE_DIR}/src/cross_net.c
  ${crosstools_SOURCE_DIR}/src/cross_net.h
  ${crosstools_SOURCE_DIR}/src/cross_util.h
  ${crosstools_SOURCE_DIR}/src/cross_util.c
)

target_include_directories(
  crosstools PUBLIC
  ${crosstools_SOURCE_DIR}/src
)

target_link_libraries(
  crosstools PUBLIC
  Threads::Threads
)

if (NOT CMAKE_USE_PTHREADS_INIT)
  target_sources(
    crosstools PUBLIC
    src/pthread_wrapper/pthread.cpp
    src/pthread_wrapper/pthread.h
  )
  
  target_include_directories(
    crosstools PUBLIC
    src/pthread_wrapper
  )
endif()

# --------------------

add_library(libraop STATIC
  src/aes.c
  src/aes.h
  src/aes_ctr.c
  src/aes_ctr.h
  src/raop_client.c
  src/raop_client.h
  src/rtsp_client.c
  src/rtsp_client.h
)

target_link_libraries(libraop PUBLIC
  crosstools
  alac
  OpenSSL::Crypto
  OpenSSL::SSL
  wsock32
  ws2_32
)

add_executable(cliraop src/cliraop.c)
target_link_libraries(cliraop libraop)