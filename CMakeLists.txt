cmake_minimum_required(VERSION 4.0)
project(TrafficSimulator)

set(CMAKE_CXX_STANDARD 20)

#enable once ui is set and finished, it allows .ini to be copied from the main dir to the debug dir
configure_file(imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini COPYONLY)

# --- 1. AUTOMATICALLY DOWNLOAD GLFW ---
include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

# --- 3. IMGUI SETUP ---
# Point this to the parent folder of your imgui-docking folder
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking")

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(TrafficSimulator main.cpp
        vehicles.h
        vehicles.cpp
        traffic.h
        traffic.cpp
        SimulationUI.cpp
        SimulationUI.h
        ${IMGUI_SOURCES}
)

# --- 5. INCLUDE DIRECTORIES ---
target_include_directories(TrafficSimulator PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

# --- 6. LINKING ---
# Note: Use 'glfw' (lowercase) because that's the name from FetchContent
target_link_libraries(TrafficSimulator PRIVATE
        glfw
        OpenGL::GL
        gdi32
        shell32
)

