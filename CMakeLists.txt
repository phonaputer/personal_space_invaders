cmake_minimum_required(VERSION 3.24.0)
project(personal_space_invaders VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

if(EMSCRIPTEN)
  target_link_options(${PROJECT_NAME} PRIVATE "SHELL:--embed-file ${CMAKE_BINARY_DIR}/assets@/assets")
endif()