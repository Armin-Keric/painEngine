cmake_minimum_required(VERSION 3.20)
project(vulkan)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-g")

# ── Source files ──────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/pain_app.cpp
    src/pain_window.cpp
    src/pain_pipeline.cpp
    src/pain_device.cpp
)
 
add_executable(${PROJECT_NAME} ${SOURCES})
 
# ── Include directories ───────────────────────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    Libs/include
)
 
# ── Find packages ─────────────────────────────────────────────────────────────
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
 
# ── Link libraries ────────────────────────────────────────────────────────────
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    Vulkan::Vulkan
)
