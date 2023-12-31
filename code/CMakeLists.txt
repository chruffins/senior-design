cmake_minimum_required(VERSION 3.0)
project(chrus_engine C)

# set C standard
set(CMAKE_C_STANDARD 11)

add_library(chrus_lib SHARED gameobjects/src/camera.c
gameobjects/src/node.c
gameobjects/src/scene.c
gameobjects/src/scenemanager.c
gameobjects/src/script.c
gameobjects/src/sound.c
gameobjects/src/sprite.c
gameobjects/src/chrus.c
utils/src/vector.c
utils/src/rbtree.c
utils/src/allocator.c)

# find the files
#file(GLOB_RECURSE SOURCES main/*.c main/*.h gameobjects/*.c gameobjects/*.h utils/*.h utils/*.c)

add_executable(${PROJECT_NAME} main/src/main.c)

target_compile_options(chrus_lib PRIVATE -g -Wall)
target_compile_options(chrus_engine PRIVATE -g -Wall)
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
find_library(LUAJIT luajit)

target_link_libraries(chrus_lib ${AL} ${AL_FONT} ${AL_AUDIO} ${AL_ACODEC} ${AL_PRIMITIVES} ${AL_IMAGE} ${LUAJIT})

target_link_libraries(${PROJECT_NAME} ${AL} ${AL_FONT} ${AL_PRIMITIVES} ${AL_IMAGE} ${AL_AUDIO} ${AL_ACODEC} ${LUAJIT} chrus_lib)