# Chrus Engine Overview
Welcome to Chrus Engine! Chrus Engine is my senior capstone, a game engine designed for 2D games.

# Quickstart (sort of)
How to use the engine (in its current state):

You'll need to install the Allegro 5 and the LuaJIT dev libraries. On Debian (or anything with apt):
- `sudo apt install liballegro5-dev`
- `sudo apt install libluajit-5.1-dev`

Then, you'll need to build the engine from source. Do this:
- `cd code/build`
- `cmake ..`
- `make`

The executable and shared library get put into `code/build`. Now, take a look at `code/main/src/main.c`. This is where the main loop is. Line 96 loads in `testscene.json` stored in `code/build`, which loads `code/build/data/helloworld.lua`, the script that the demo runs in. If you check out the JSON file itself, then the serialization and deserialization files in `code/utils`, you'll see how to tweak values in the JSON! Line 149 in `main.c` is commented out, but it contains a function call that can save a scene into a file. However, I never wrote an API for traversing the scene, so the scene files are basically useless!

Lastly, you'll want to check out `code/build/data/chrus_ffi.lua`, and `code/build/data/events.lua`. These are the Lua files that contain the scripting APIs you use. When you edit the scripts, you can check the changes by just re-running the executable! No need for recompiling.
