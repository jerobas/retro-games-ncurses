# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(SnakeGame VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Specify build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# I DONT KNOW WHAT THIS IS MAKING
if(CMAKE_BUILD_TYPE STREQUAL "ncurses")
add_definitions(${CMAKE_C_FLAGS_NCURSES})
endif()

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Custom flags for different build types
set(CMAKE_C_FLAGS_NCURSES "-DCMAKE_BUILD_TYPE=ncurses")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Add subdirectories
# add_subdirectory(src)

# Add the source files
set(SOURCES
    src/snake/logic.c
    src/arena.c
    src/snake/client.c
    src/snake/cJSON.c
)

# Link the ncurses library
if(CMAKE_BUILD_TYPE STREQUAL "ncurses")
  list(APPEND SOURCES
      src/snake/ncurses/graphics.c
      src/snake/ncurses/client.c
      src/snake/ncurses/main.c
      src/ncurses-handler.c
  )

  # Add the ncurses library
  find_package(Curses REQUIRED)
  include_directories(${CURSES_INCLUDE_DIR})
endif()

# Define the executable
add_executable(snake ${SOURCES})

# Link the ncurses library  
if(CMAKE_BUILD_TYPE STREQUAL "ncurses")
  target_link_libraries(snake ${CURSES_LIBRARIES})
endif()

