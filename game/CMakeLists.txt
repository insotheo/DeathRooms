project(DeathRoomsTheGame)

file(GLOB GAME_SOURCES
src/main.cpp
src/engine/*.cpp
src/game/*.cpp
)

set(INCLUDE_DIRS
src/
vendor/SFML/include
)

add_executable(Death_Rooms ${GAME_SOURCES})

target_include_directories(Death_Rooms PRIVATE ${INCLUDE_DIRS})

target_link_libraries(Death_Rooms PRIVATE
FLAC
freetype
ogg
sfml-audio
sfml-graphics
sfml-main
sfml-system
sfml-window
vorbis
vorbisenc
vorbisfile
)

add_custom_command(
    TARGET Death_Rooms
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/bin/assets"
    COMMENT "Copying assets directory to build directory..."
)