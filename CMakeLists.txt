cmake_minimum_required(VERSION 3.10)
project(ProjectMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Where to look for headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources
file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(ProjectMaker ${SRC_FILES})
target_link_libraries(ProjectMaker ${wxWidgets_LIBRARIES})
