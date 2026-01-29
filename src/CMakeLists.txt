include(FetchContent)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sdl)

set(SOURCES
    main.cpp
)

add_executable(testo ${SOURCES})
target_link_libraries(testo SDL3::SDL3)
