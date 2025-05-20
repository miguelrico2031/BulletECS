
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS CONFIGURE_DEPENDS include/BulletECS/*.h)

add_library(BulletECS STATIC ${LIB_SOURCES} ${LIB_HEADERS} "src/MotionStatePool.cpp")
target_include_directories(BulletECS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#include_directories(
#    ${bullet_SOURCE_DIR}/src
#    ${bullet_SOURCE_DIR}/src/bullet
#)

target_include_directories(BulletECS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${bullet_SOURCE_DIR}/src
        ${bullet_SOURCE_DIR}/src/bullet
)

target_link_libraries(BulletECS
    PUBLIC
        BulletDynamics
        BulletCollision
        LinearMath
)
