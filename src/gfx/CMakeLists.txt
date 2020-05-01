add_library(gfx STATIC
  audio-chunk.cpp
  font.cpp
  renderer.cpp
  sdl-init.cpp
  surface.cpp
  texture.cpp
  util.cpp
  window.cpp
)

target_include_directories(gfx PRIVATE ${SDL_INCLUDE_DIRS})
