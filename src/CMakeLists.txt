include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

add_executable(
  ${PROJECT_NAME} 
  main.cpp
)

add_subdirectory(util)
add_subdirectory(app)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_include_directories(
  ${PROJECT_NAME} 
  PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/app
  ${CMAKE_CURRENT_SOURCE_DIR}/util
)
