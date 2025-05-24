
add_executable(BulletECS_ECSDemoVsRawBullet main.cpp "BulletECSSim.cpp" "RawBulletSim.h" "RawBulletSim.cpp")

target_link_libraries(BulletECS_ECSDemoVsRawBullet
    PRIVATE
        BulletECS
)
