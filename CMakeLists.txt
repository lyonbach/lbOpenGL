cmake_minimum_required(VERSION 3.0)

project(lbOpenGL)

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(GLM_INCLUDE_DIRS ./Vendor/GLM/include)

# Add the include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ./Source)

# Add the source files
file(GLOB_RECURSE SOURCES ./Source/*.cpp ./Source/*.h)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the required libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${GLM_LIBRARIES})