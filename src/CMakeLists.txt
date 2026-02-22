include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
  SDL3_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG        prerelease-3.1.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)

add_executable(
  ${PROJECT_NAME} 
  main.cpp
)

add_subdirectory(engine)
add_subdirectory(game)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer)
target_include_directories(
  ${PROJECT_NAME} 
  PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/engine
  ${CMAKE_CURRENT_SOURCE_DIR}/game/scenes/invasion
)
