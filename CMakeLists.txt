cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)
set(CMAKE_BUILD_TYPE "Debug")
if(MSVC OR XCODE)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# set the project name
project(Test)

set(EXEC Test)
# this function searches for all .cpp files in the source code
file(GLOB SRC src/*.cpp)
# add all the .cpp file to the executable project
add_executable(${EXEC} ${SRC})
# include the header files to the project
include_directories(PUBLIC src/include)

# find the OpenGL package and other components
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)

# download and compile dlib
include(FetchContent)
FetchContent_Declare(dlib
    GIT_REPOSITORY https://github.com/davisking/dlib.git
    GIT_TAG        v19.22
)
FetchContent_MakeAvailable(dlib)

# point OpenCV_DIR to the build of opencv
SET("OpenCV_DIR" CACHE PATH "opencv/build")
find_package(OpenCV REQUIRED)

# link the libraries to the main project
target_link_libraries(${EXEC} PUBLIC ${OpenCV_LIBS} dlib::dlib OpenGL::GL glew_s glfw glm)