cmake_minimum_required(VERSION 3.25.1)
project(clipboardy)
set(BUILD_MODE "server" CACHE STRING "Build mode: 'server' or 'client'")

if (WIN32) 

    message(STATUS "Building on Windows")
    if (BUILD_MODE STREQUAL "main")
        message(STATUS "Build Server")
        add_executable(${PROJECT_NAME} src/server_wind.c)
    elseif (BUILD_MODE STREQUAL "client")
        message(STATUS "Build Client")
        add_executable(${PROJECT_NAME} src/client_wind.c)
    endif()
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
    target_include_directories(${PROJECT_NAME} PRIVATE "C:\\msys64\\ucrt64\\include")

elseif (UNIX AND NOT APPLE) 

    message(STATUS "Building on Linux")
    add_executable(${PROJECT_NAME} src/socket_linux.c) 
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} X11)

endif()

