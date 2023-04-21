cmake_minimum_required(VERSION 3.16)
project(vulkan-model-viewer)

set(CMAKE_CXX_STANDARD 20)
find_package(Vulkan REQUIRED)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLM)

add_executable(${PROJECT_NAME} 
     src/main.cpp
     src/lve_window.hpp
     src/lve_window.cpp
     src/lve_pipeline.hpp
     src/lve_pipeline.cpp
     src/lve_device.hpp
     src/lve_device.cpp
     src/app.hpp
     src/app.cpp
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan ${GLM_LIBRARIES} glm::glm)