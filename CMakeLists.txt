cmake_minimum_required(VERSION 3.25.1)
project(clipboardy)

if(WIN32)
    message(STATUS "Building on Windows")
    add_executable(${PROJECT_NAME} src/windows/main.c src/windows/server.c src/windows/clipboard.c)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
    target_include_directories(${PROJECT_NAME} PRIVATE "C:\\msys64\\ucrt64\\include")
elseif(APPLE)
    message(STATUS "Building on macOS")
    add_executable(${PROJECT_NAME} src/posix/clipboard.c src/posix/server.c src/posix/main.c)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
elseif(UNIX)
    message(STATUS "Building on Linux")
    
    #check if xclip is installed
    find_program(XCLIP_PATH xclip)

    if(NOT XCLIP_PATH)
        message(FATAL_ERROR "xclip not found! Please install it using your package manager.")
    endif()

    add_executable(${PROJECT_NAME} src/posix/clipboard.c src/posix/server.c src/posix/main.c)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} X11)
endif()
