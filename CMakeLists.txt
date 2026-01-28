cmake_minimum_required(VERSION 3.20)
project(TrafficSimulator)

set(CMAKE_CXX_STANDARD 20)

#allows .ini to be copied from the main dir to the debug dir
configure_file(imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini COPYONLY)

#GLFW
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

#IMGUI PATHS
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking")

set(IMGUI_SOURCES
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

set(PROJECT_SOURCES
        src/main.cpp
        src/vehicles.h
        src/vehicles.cpp
        src/traffic.h
        src/traffic.cpp
        src/SimulationUI.cpp
        src/SimulationUI.h
)

# BUILD
add_executable(TrafficSimulator ${PROJECT_SOURCES} ${IMGUI_SOURCES})


target_include_directories(TrafficSimulator PRIVATE
        src                        # Look for your headers in src/
        ${IMGUI_DIR}               # Look for imgui.h
        ${IMGUI_DIR}/backends      # Look for imgui_impl_xxxx.h
)

#LINKING
target_link_libraries(TrafficSimulator PRIVATE
        glfw
        OpenGL::GL
        gdi32
        shell32
)