cmake_minimum_required(VERSION 3.7)
project(basestruct)

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5 COMPONENTS
    Core
    DBus
REQUIRED)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE BASESTRUCT_SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${BASESTRUCT_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Qt5Widgets_LIBRARIES}
    ${Qt5DBus_LIBRARIES}   
)

#set(CMAKE_INSTALL_PREFIX /usr)
## Install the executable
#install(TARGETS ${PROJECT_NAME} DESTINATION lib)

