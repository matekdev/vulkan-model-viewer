cmake_minimum_required(VERSION 3.16)
project(vulkan-model-viewer)

set(CMAKE_CXX_STANDARD 17)
find_package(Vulkan REQUIRED)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLM)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan ${GLM_LIBRARIES} glm::glm)