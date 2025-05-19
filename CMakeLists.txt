cmake_minimum_required(VERSION 3.16)
project(BulletECS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# binaries (.exe) install location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# fetch external deps
# bullet
include(FetchContent)
FetchContent_Declare(
    bullet
    GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
    GIT_TAG 3.25
)

# disable Bullet tests, demos, and extras to avoid GTest and unnecessary targets
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)

# Disable SoftBody and InverseDynamics modules
set(BUILD_BULLET_SOFTBODY OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET_INVERSE_DYNAMICS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET_ROBOTICS OFF CACHE BOOL "" FORCE)
set(BUILD_MULTITHREADING OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_ENET OFF CACHE BOOL "" FORCE)
set(BUILD_GIMPACT OFF CACHE BOOL "" FORCE)
set(BUILD_HACD OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bullet)

# Subdirectorios
add_subdirectory(lib)
add_subdirectory(examples/helloworld)
add_subdirectory(examples/example2)