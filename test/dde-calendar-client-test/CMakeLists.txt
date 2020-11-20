cmake_minimum_required(VERSION 3.7)
#common resource names

set(APP_CLIENT_DIR "${PROJECT_SOURCE_DIR}/calendar-client")
set(APP_RES_DIR "${APP_CLIENT_DIR}/assets")
set(APP_BIN_NAME "dde-calendar-test")
set(APP_TRANS_SCRIPT "${APP_RES_DIR}/translate_generation.sh")
set(APP_DESKTOP "${APP_RES_DIR}/dde-calendar.desktop")
set(APP_QRC "${APP_RES_DIR}/resources.qrc")
set(APP_SERVICE "${APP_RES_DIR}/dbus/com.deepin.Calendar.service")

project(${APP_BIN_NAME})

find_package(PkgConfig REQUIRED)
find_package(DtkCore REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
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

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi -Wl,--as-needed")
endif ()

#compile flags
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else()
  # -Wl, -O2 Enable linker optimizations
  # -Wl, --gc-sections Remove unused code resulting from -fdsta-sections and
  # -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
endif()

#Generate the qm file
execute_process(COMMAND bash ${APP_TRANS_SCRIPT}
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

configure_file(${APP_RES_DIR}/environments.h.in environments.h @ONLY)

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

# Tell CMake to create the executable
add_executable(${PROJECT_NAME} ${Calendar_TEST_SRC} ${Calendar_SRC} ${APP_QRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS} ${OBJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
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
)
enable_testing()
add_test(
  NAME ${PROJECT_NAME}
  COMMAND $<TARGET_FILE:${PROJECT_NAME}>
  )
