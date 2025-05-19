
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS CONFIGURE_DEPENDS include/BulletECS/*.h)

add_library(BulletECS STATIC ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(BulletECS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(BulletECS
    PUBLIC
        BulletDynamics
        BulletCollision
        LinearMath
)
