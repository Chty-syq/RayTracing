cmake_minimum_required(VERSION 3.25)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third-party/tbb)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME}
        tbb
        )
target_include_directories(${PROJECT_NAME} PUBLIC  # header-only package
        src
        third-party/stb_image
        third-party/glm
        )
