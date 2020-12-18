cmake_minimum_required(VERSION 3.7)
#common resource names
project(dde-calendar-client-test)

ADD_COMPILE_OPTIONS(-fno-access-control)

set(APP_CLIENT_DIR "${CMAKE_SOURCE_DIR}/calendar-client")
set(APP_RES_DIR "${APP_CLIENT_DIR}/assets")
set(APP_BIN_NAME "dde-calendar-test")
set(PROJECT_NAME_TEST ${APP_BIN_NAME})

find_package(PkgConfig REQUIRED)
find_package(DtkCore REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DFrameworkdbus REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-Wl,--as-needed -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "-pie")

include_directories(${APP_CLIENT_DIR}/src)

SUBDIRLIST(all_src ${APP_CLIENT_DIR}/src)

#Include all app own subdirectorys
foreach(subdir ${all_src})
    include_directories(${APP_CLIENT_DIR}/src/${subdir})
endforeach()

file(GLOB_RECURSE Calendar_SRC ${APP_CLIENT_DIR}/src/*.cpp)
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})
list(REMOVE_ITEM Calendar_SRC ${APP_CLIENT_DIR}/src/main.cpp)

#test SRC
SUBDIRLIST(TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
foreach(subdir ${TEST_SRC})
    include_directories(${subdir})
endforeach()

file(GLOB_RECURSE Calendar_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(../../calendar-basicstruct/)

# Tell CMake to create the executable
add_executable(${APP_BIN_NAME} ${Calendar_TEST_SRC} ${Calendar_SRC})

target_include_directories(${APP_BIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS} ${OBJECT_BINARY_DIR})

target_link_libraries(${APP_BIN_NAME}
    ${DtkWidget_LIBRARIES}
    ${DtkCore_LIBRARIES}
    ${DtkGui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Svg_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
    ${DFrameworkdbus_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    pthread
    basestruct
)

# 设置添加gocv相关信息的输出
set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage")


##------------------------------ 创建'make client test'指令---------------------------------------

add_custom_command(TARGET ${APP_BIN_NAME} PRE_LINK
    COMMAND echo " =================== SERVICE START ==================== "
    #COMMAND ls ${CMAKE_BINARY_DIR}/calendar-service/
    #COMMAND ${CMAKE_BINARY_DIR}/calendar-service/dde-calendar-service &
    )

add_custom_command(TARGET ${APP_BIN_NAME} POST_BUILD
    COMMAND echo " =================== CREAT LCOV REPROT BEGIN ==================== "
    COMMAND ${CMAKE_BINARY_DIR}/test/dde-calendar-client-test/${APP_BIN_NAME}
    COMMAND echo " =================== TEST END ==================== "
    )
