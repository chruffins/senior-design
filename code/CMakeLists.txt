cmake_minimum_required(VERSION 3.0)
project(chrus_engine C)

# set C standard
set(CMAKE_C_STANDARD 11)

# find the files
file(GLOB_RECURSE SOURCES main/*.c main/*.h gameobjects/*.c gameobjects/*.h utils/*.h utils/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(chrus_engine PRIVATE -g -Wall)
#target_compile_options(chrus_engine -g)

# set some compiler flags
#target_compile_options(example PUBLIC -std=c++1y -Wall -Wfloat-conversion)
#target_compile_options(${PROJECT_NAME} -O2)

find_library(AL allegro)
find_library(AL_FONT allegro_font)
find_library(AL_AUDIO allegro_audio)
find_library(AL_PRIMITIVES allegro_primitives)
find_library(AL_IMAGE allegro_image)
find_library(LUAJIT luajit)

target_link_libraries(${PROJECT_NAME} ${AL} ${AL_FONT} ${AL_AUDIO} ${AL_PRIMITIVES} ${AL_IMAGE} ${LUAJIT})