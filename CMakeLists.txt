cmake_minimum_required(VERSION 3.1.0)

#set variables
set (CMAKE_PROJECT_VERSION 0.0.9)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)
#set (CMAKE_BUILD_TYPE RelWithDebInfo)

#set project variables
project (sf)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CURSES_NEED_WIDE TRUE)

#external packages
if(MSVC)
  add_custom_target(
   PDCurses
   COMMAND nmake -f Makefile.vc WIDE=Y UTF8=Y
   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/PDCurses/Wincon"
  )
  set(CURSES_LIBRARIES "${CMAKE_SOURCE_DIR}/PDCurses/Wincon/pdcurses.lib")
  set(CURSES_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/PDCurses")
else()
  find_package(Curses REQUIRED)
endif()

find_package(Threads REQUIRED)
add_subdirectory(yaml-cpp buildycpp)
include_directories(
  ${CURSES_INCLUDE_DIR}
  "yaml-cpp/include"
)

#check ncursesw
message(STATUS "CURSES_LIBRARIES: ${CURSES_LIBRARIES}")
message(STATUS "CURSES_INCLUDE: ${CURSES_INCLUDE_DIR}")

#tests
ENABLE_TESTING()
ADD_SUBDIRECTORY( tests )

##############################
# SF
file(GLOB_RECURSE SF_SRC
    "src/sf/*.cpp"
)

add_executable(
    sf
    ${SF_SRC}
)

target_link_libraries(
    sf
    ${CURSES_LIBRARIES}
    yaml-cpp
    Threads::Threads
)

set_target_properties( sf PROPERTIES RUNTIME_OUTPUT_DIRECTORY . )
set_target_properties( sf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG . )
set_target_properties( sf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE . )

##############################
# SF_SERVER
file(GLOB_RECURSE SF_SERVER_SRC
    "src/sf_server/*.cpp"
)

add_executable(
    sf_server
    ${SF_SERVER_SRC}
    "src/sf/tools/log.cpp"
    "src/sf/tools/config.cpp"
    "src/sf/tools/clientsocket.cpp"
    "src/sf/tools/basesocket.cpp"
    "src/sf/tools/netmsg.cpp"
    "src/sf/tools/vec2.cpp"
    "src/sf/gameobject.cpp"
)

target_link_libraries(
    sf_server
    yaml-cpp
    Threads::Threads
)

if(MSVC)
  add_dependencies(sf_server PDCurses)
endif()

set_target_properties( sf_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY . )
set_target_properties( sf_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG . )
set_target_properties( sf_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE . )

##############################
# SF_CLIENT_TEST
file(GLOB_RECURSE SF_CLIENT_TEST_SRC
    "src/sf_client_test/*.cpp"
)

add_executable(
    sf_client_test
    ${SF_CLIENT_TEST_SRC}
    "src/sf/tools/log.cpp"
    "src/sf/tools/client.cpp"
    "src/sf/tools/clientsocket.cpp"
    "src/sf/tools/basesocket.cpp"
    "src/sf/tools/netmsg.cpp"
    "src/sf/tools/vec2.cpp"
    "src/sf/gameobject.cpp"
)

target_link_libraries(
    sf_client_test
    Threads::Threads
)

set_target_properties( sf_client_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY . )
set_target_properties( sf_client_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG . )
set_target_properties( sf_client_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE . )

#copy config and language files
#file(COPY lang DESTINATION .)
configure_file(lang/en.yaml lang/en.yaml COPYONLY)
configure_file(lang/de.yaml lang/de.yaml COPYONLY)
configure_file(config.yaml config.yaml COPYONLY)
configure_file(server.yaml server.yaml COPYONLY)
