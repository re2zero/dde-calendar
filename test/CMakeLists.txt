cmake_minimum_required(VERSION 3.7)
project(dde-calendar-test)

ADD_SUBDIRECTORY(dde-calendar-client-test)

ADD_SUBDIRECTORY(dde-calendar-service-test)

include_directories(third-party_stub)
