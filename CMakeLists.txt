cmake_minimum_required(VERSION 3.16)
project(vulkan-model-viewer)

set(CMAKE_CXX_STANDARD 20)
find_package(Vulkan REQUIRED)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLM)

set(GLSLC_EXECUTABLE "${Vulkan_INCLUDE_DIR}/../bin/glslc")
file(GLOB_RECURSE SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert" "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag")
add_custom_target(
    compile_shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/shaders/spv"
)

foreach(SHADER_FILE IN LISTS SHADER_FILES)
    get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME_WE)
    get_filename_component(SHADER_EXTENSION ${SHADER_FILE} EXT)

    add_custom_command(
        TARGET compile_shaders
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER_FILE} -o "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER_FILENAME}${SHADER_EXTENSION}.spv"
        COMMENT "Compiling shader ${SHADER_FILE}"
    )
endforeach()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/lve_window.hpp
    src/lve_window.cpp
    src/lve_pipeline.hpp
    src/lve_pipeline.cpp
    src/lve_device.hpp
    src/lve_device.cpp
    src/lve_swap_chain.hpp
    src/lve_swap_chain.cpp
    src/lve_model.hpp
    src/lve_model.cpp
    src/lve_object.hpp
    src/lve_renderer.hpp
    src/lve_renderer.cpp
    src/simple_render_system.hpp
    src/simple_render_system.cpp
    src/app.hpp
    src/app.cpp
)

add_dependencies(${PROJECT_NAME} compile_shaders)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan ${GLM_LIBRARIES} glm::glm)