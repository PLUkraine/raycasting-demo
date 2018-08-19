cmake_minimum_required (VERSION 3.1)
project (Raycaster)

# use GLFW
add_subdirectory("glfw")
include_directories("glfw/include")

# use OpenGL
find_package(OpenGL REQUIRED)

# include all other libs (imaging, glad, logging...)
include_directories ("${PROJECT_SOURCE_DIR}/include")

# all source files
set (PROJECT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Window.h "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ImageRenderer.cpp "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RaycasterEngine.cpp "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.h "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ImageRenderer.h "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RaycasterEngine.h "
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp "
)

# create target
add_executable (Raycaster ${PROJECT_SRC})
target_link_libraries (Raycaster glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# copy resources to the project root
if (MSVC)
    message("Resources will be put in ${PROJECT_BINARY_DIR}")
    add_custom_command(TARGET Raycaster POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources "${PROJECT_BINARY_DIR}/resources")
endif (MSVC)
# copy resources to the target build folder
add_custom_command(TARGET Raycaster POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources "$<TARGET_FILE_DIR:Raycaster>/resources")