
add_executable(BulletECS_ECSDemo main.cpp "../DemoVsRawBullet/BulletECSSim.h")

target_link_libraries(BulletECS_ECSDemo
    PRIVATE
        BulletECS
)
