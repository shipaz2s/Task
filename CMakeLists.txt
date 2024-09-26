cmake_minimum_required(VERSION 3.5.0)

project("Calculator" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(Qt6 COMPONENTS Core Widgets Gui Network REQUIRED)

if(WIN32)
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		set(PLUGINS_FILES ${Qt6_DIR}/../../../plugins/platforms/qwindows.dll)
	else()
		set(PLUGINS_FILES ${Qt6_DIR}/../../../plugins/platforms/qwindowsd.dll)
	endif()
endif()

message(STATUS "PLUGINS_FILES ${PLUGINS_FILES}")

message("-- Configuring " ${PROJECT_NAME})


set(lib_headers include/calculator.h)

set(lib_src src/calculator.c)

set(ui_headers include/mainwnd.hpp)

set(ui_src	src/gui.cpp
			src/mainwnd.cpp
			)

set(test_src src/test.cpp)

if (WIN32 AND (CMAKE_BUILD_TYPE STREQUAL "Release") )
	add_library(Calculator SHARED ${lib_src} ${lib_headers})
else()
	add_library(Calculator SHARED ${lib_src} ${lib_headers})
endif()

set_target_properties(Calculator PROPERTIES LINKER_LANGUAGE C)

if (WIN32 AND (CMAKE_BUILD_TYPE STREQUAL "Release") )
	add_executable(CalculatorUi WIN32 ${ui_src} ${ui_headers})
else()
	add_executable(CalculatorUi ${ui_src} ${ui_headers})
endif()

set_target_properties(CalculatorUi PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test ${test_src})
set_target_properties(test PROPERTIES LINKER_LANGUAGE CXX)

message("-- " ${PROJECT_NAME} " BUILD_TYPE " ${CMAKE_BUILD_TYPE})

#lib-----------------------------------------------------------------------lib
target_include_directories(Calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#UI------------------------------------------------------------------------UI
target_include_directories(CalculatorUi PUBLIC	${Qt6Widgets_INCLUDE_DIRS}
												${Qt6Core_INCLUDE_DIRS}
												${Qt6Gui_INCLUDE_DIRS}
												${CMAKE_CURRENT_SOURCE_DIR}/include
												)


target_link_libraries(CalculatorUi ${Qt6Widgets_LIBRARIES}
									${Qt6Gui_LIBRARIES}
									${Qt6Core_LIBRARIES}
									Calculator
									)

#test----------------------------------------------------------------------test

target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(test Calculator)

# install(TARGETS CalculatorUi
# 	RUNTIME_DEPENDENCIES
# 	PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
# 	POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
# 	DIRECTORIES  ${Qt6_DIR}/bin ${Qt6_DIR}/../../../bin
# 	RUNTIME DESTINATION .
# 	LIBRARY DESTINATION lib
# 	ARCHIVE DESTINATION lib/static)

# install(FILES ${PLUGINS_FILES} DESTINATION plugins/platforms)
# install(DIRECTORY ${RESOURCE_DIR} DESTINATION .)

# set(CPACK_PACKAGE_NAME ${exec_name})
# set(CPACK_PACKAGE_CONTACT "me <me@example.com>")
# set(CPACK_GENERATOR ZIP DEB NSIS)
# include(CPack)