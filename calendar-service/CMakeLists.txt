cmake_minimum_required(VERSION 3.7)
project(dde-calendar-service)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(APP_RES_DIR "assets")
set(APP_SERVICE "${APP_RES_DIR}/data/com.deepin.dataserver.calendar.service")
set(APP_CONFIG "${APP_RES_DIR}/data/com.deepin.dataserver.calendar.conf")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(DtkGui REQUIRED)
find_package(Qt5 COMPONENTS
    Core
    Widgets
    DBus
REQUIRED)

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    ${DtkCore_LIBRARIES}
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

SUBDIRLIST(all_src ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Include all app own subdirectorys
foreach(subdir ${all_src})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir})
endforeach()
file(GLOB_RECURSE CALENDARSERVICE_SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

include_directories(../calendar-basicstruct)
link_directories(${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${CALENDARSERVICE_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)
#link_libraries(../basestruct/basestruct)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} basestruct)


set(CMAKE_INSTALL_PREFIX /usr)

# Install files
install(TARGETS ${PROJECT_NAME} DESTINATION lib/deepin-daemon/)
install(FILES ${APP_SERVICE} DESTINATION share/dbus-1/services/)
install(FILES ${APP_CONFIG} DESTINATION share/dbus-1/system.d/)
