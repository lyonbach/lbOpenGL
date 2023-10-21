cmake_minimum_required(VERSION 3.0)

project(lbOpenGL)

set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLM/include)
set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLEW/include)
set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/include)

set(GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLEW/lib64/libGLEW.a)
set(GLFW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/lib/libglfw3.a)

# Add the include directories
include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ./Source)


# Add the source files
file(GLOB_RECURSE SOURCES ./Source/*.cpp ./Source/*.h)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Link the required libraries
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARY} X11 GL)