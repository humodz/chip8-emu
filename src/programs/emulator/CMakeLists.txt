add_executable(emulator
  emulator.cpp
)

target_link_libraries(emulator chip8 gfx)
target_include_directories(emulator PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(emulator ${SDL2_LIBRARIES})
target_link_libraries(emulator ${SDL2EXTRA_LIBRARIES})