cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_definitions(-DUNICODE -D_UNICODE)


target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil)
