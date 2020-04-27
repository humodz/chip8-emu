add_executable(hello-boost
  hello-boost.cpp
)

target_link_libraries(hello-boost ${BOOST_LIBRARIES})

add_executable(hello-sdl
  hello-sdl.cpp
)

target_link_libraries(hello-sdl gfx)
target_include_directories(hello-sdl PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(hello-sdl ${SDL2_LIBRARIES})
target_link_libraries(hello-sdl ${SDL2EXTRA_LIBRARIES})

add_subdirectory(disassembler)
add_subdirectory(emulator)