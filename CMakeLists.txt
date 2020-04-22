cmake_minimum_required(VERSION 3.5)
project(move-transition)

include(external/FindLibobs.cmake)
include(ObsHelpers)
find_package(LibObs REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH "${QTDIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(move-transition_HEADERS
	move-transition.h
	easing.h)
set(move-transition_SOURCES
	move-transition.c
	move-transition-override-filter.c
	easing.c)

include_directories(
        "${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api"
        "${LIBOBS_INCLUDE_DIR}/../plugins")

if(WIN32)
	list(APPEND move-transition_SOURCES
		move-transition.rc)
endif()

add_library(move-transition MODULE
	${move-transition_HEADERS}
	${move-transition_SOURCES})
target_link_libraries(move-transition
	obs-frontend-api
	libobs)

install_obs_plugin_with_data(move-transition data)
