cmake_minimum_required(VERSION 3.0)
project(chrus_engine C)

# set C standard
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

add_library(chrus_lib SHARED gameobjects/src/camera.c
gameobjects/src/node.c
gameobjects/src/scene.c
gameobjects/src/scenemanager.c
gameobjects/src/script.c
gameobjects/src/sound.c
gameobjects/src/sprite.c
gameobjects/src/chrus.c
gameobjects/src/text.c
gameobjects/src/primitive.c
utils/src/vector.c
utils/src/rbtree.c
utils/src/serializer.c
utils/src/deserializer.c
utils/src/allocator.c
utils/src/globals.c
utils/src/drawing_thread.c
utils/src/bitmap_converter.c
utils/src/pdjson.c)

# find the files
#file(GLOB_RECURSE SOURCES main/*.c main/*.h gameobjects/*.c gameobjects/*.h utils/*.h utils/*.c)

add_executable(${PROJECT_NAME} main/src/main.c)

target_compile_options(chrus_lib PRIVATE)
target_compile_options(chrus_engine PRIVATE)
#target_compile_options(chrus_engine -g)

# set some compiler flags
#target_compile_options(example PUBLIC -std=c++1y -Wall -Wfloat-conversion)
#target_compile_options(${PROJECT_NAME} -O2)

find_library(AL allegro)
find_library(AL_FONT allegro_font)
find_library(AL_AUDIO allegro_audio)
find_library(AL_ACODEC allegro_acodec)
find_library(AL_PRIMITIVES allegro_primitives)
find_library(AL_IMAGE allegro_image)
find_library(LUAJIT luajit-5.1)

target_link_libraries(chrus_lib ${AL} ${AL_FONT} ${AL_AUDIO} ${AL_ACODEC} ${AL_PRIMITIVES} ${AL_IMAGE} ${LUAJIT})

target_link_libraries(${PROJECT_NAME} ${AL} ${AL_FONT} ${AL_PRIMITIVES} ${AL_IMAGE} ${AL_AUDIO} ${AL_ACODEC} ${LUAJIT} chrus_lib)