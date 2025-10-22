cmake_minimum_required(VERSION 3.15)
project(Chip8Emulator)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to use vcpkg
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Add executable
add_executable(chip8
    main.cpp
    CPU.cpp
    Memory.cpp
    Display.cpp
    Keyboard.cpp
)

# Link SDL2
target_link_libraries(chip8 ${SDL2_LIBRARIES})