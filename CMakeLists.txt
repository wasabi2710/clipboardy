cmake_minimum_required(VERSION 3.25.1)
project(clipboardy)

if (WIN32) 
    message(STATUS "Building on Windows")
    add_executable(${PROJECT_NAME} src/wndow.c) 
elseif(UNIX AND NOT APPLE) 
    message(STATUS "Building on Linux")
    add_executable(${PROJECT_NAME} src/linux.c)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} X11)
endif()

# if (WIN32)
#     message(STATUS "Building on Windows")
#     # Add Windows-specific options or libraries
# elseif (UNIX AND NOT APPLE)
#     message(STATUS "Building on Linux or other Unix-like OS")
#     # Add Linux-specific options or libraries
# elseif (APPLE)
#     message(STATUS "Building on macOS")
#     # Add macOS-specific options or libraries
# else()
#     message(WARNING "Unknown operating system")
# endif()
