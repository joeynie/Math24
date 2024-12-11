cmake_minimum_required(VERSION 3.10)
project(Math24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(fltk-1.4.0rc3)

include_directories(fltk-1.4.0rc3)

file(GLOB SRC
    src/*.h
    src/*.cpp
)
add_executable(${PROJECT_NAME} ${SRC})

# both win and linux use the same lib name
target_link_libraries(${PROJECT_NAME}
    fltk
    fltk_images
)