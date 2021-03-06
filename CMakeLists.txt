cmake_minimum_required(VERSION 3.11.0)
project(airsim_standalone)

set(AIRSIM_ROOT /home/madratman/projects/AirSim)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

add_subdirectory("${AIRSIM_ROOT}/cmake/rpclib_wrapper" rpclib_wrapper)
add_subdirectory("${AIRSIM_ROOT}/cmake/AirLib" AirLib)
add_subdirectory("${AIRSIM_ROOT}/cmake/MavLinkCom" MavLinkCom)

set(CMAKE_CXX_STANDARD 11) 
set(RPC_LIB_INCLUDES " ${AIRSIM_ROOT}/external/rpclib/rpclib-2.2.1/include")
set(RPC_LIB rpc)
message(STATUS "found RPC_LIB_INCLUDES=${RPC_LIB_INCLUDES}")

include_directories(${AIRSIM_ROOT}/AirLib/deps/eigen3)

include_directories(
  include
  ${AIRSIM_ROOT}/AirLib/deps/eigen3
  ${AIRSIM_ROOT}/AirLib/include
  ${RPC_LIB_INCLUDES}
  ${AIRSIM_ROOT}/MavLinkCom/include
  ${AIRSIM_ROOT}/MavLinkCom/common_utils
)

# link_directories(${AIRSIM_ROOT}/build_debug/output/lib/)
add_executable(hello_drone "src/hello_drone.cpp")
target_link_libraries(hello_drone AirLib)

add_executable(benchmark_simgetimages "src/benchmark_simgetimages.cpp")
target_link_libraries(benchmark_simgetimages AirLib)

add_executable(get_settings "src/get_settings.cpp")
target_link_libraries(get_settings AirLib)