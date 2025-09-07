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

file(GLOB ALAC_SOURCES ${alac_SOURCE_DIR}/codec/*.c)
add_library(alac STATIC ${ALAC_SOURCES})
target_include_directories(alac PUBLIC ${alac_SOURCE_DIR}/codec)

# --------------------

if (NOT CMAKE_USE_PTHREADS_INIT)
  add_library(pthread_wrapper STATIC
    src/pthread_wrapper/pthread.cpp
    src/pthread_wrapper/pthread.h
  )
  target_include_directories(pthread_wrapper PUBLIC src/pthread_wrapper)
else()
  add_library(pthread_wrapper INTERFACE)
  target_link_libraries(pthread_wrapper INTERFACE Threads::Threads)
endif()

# --------------------

FetchContent_Declare(
  crosstools
  GIT_REPOSITORY https://github.com/philippe44/crosstools.git
  GIT_TAG        810782f
)
FetchContent_MakeAvailable(crosstools)

add_library(crosstools STATIC
  ${crosstools_SOURCE_DIR}/src/platform.c
  ${crosstools_SOURCE_DIR}/src/platform.h
  ${crosstools_SOURCE_DIR}/src/cross_log.c
  ${crosstools_SOURCE_DIR}/src/cross_log.h
  ${crosstools_SOURCE_DIR}/src/cross_net.c
  ${crosstools_SOURCE_DIR}/src/cross_net.h
  ${crosstools_SOURCE_DIR}/src/cross_util.h
  ${crosstools_SOURCE_DIR}/src/cross_util.c
)

target_include_directories(crosstools PUBLIC
  ${crosstools_SOURCE_DIR}/src
)

target_link_libraries(crosstools PUBLIC pthread_wrapper)

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