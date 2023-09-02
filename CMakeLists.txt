cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)

project(RayTracing LANGUAGES CXX CUDA)

add_subdirectory(third-party/tbb)
add_subdirectory(third-party/assimp)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME}
        tbb
        assimp
        )
target_include_directories(${PROJECT_NAME} PUBLIC  # header-only package
        src
        third-party/stb_image
        third-party/glm
        )
