# User Manual
Welcome to Chrus Engine! Chrus Engine is a 2D game engine written in C, using Lua as its scripting language.

## Quickstart
Here's how to get Chrus Engine running quickly. Currently, you will need to build from source, so you will need to have CMake, Make, and a C compiler installed. On Windows, you will probably need to use MinGW/MSYS2 in order to install Chrus Engine's dependencies. Make sure that you are in the base directory before running these commands:
1. ``cd code && mkdir build && cd build``
2. ``cmake ..``
3. ``make``

An executable and a shared library will be created, and these are the necessary files for Chrus Engine to function. Also, Chrus Engine requires Allegro 5.2 and LuaJIT 2.1 as dependencies. Once you've installed Chrus Engine, you can start creating games using the IDE (that currently does not exist).

## FAQ
``TODO: Think of some frequently asked questions.``
Q. Will Chrus Engine support any other scripting languages?
A. Not for the expo! However, FFI bindings can theoretically be written for any language that supports calling C functions from shared libraries.

Q. Can I use Chrus Engine directly from C?
A. With some minor modifications, it's possible to do so by replacing the Lua calls from C with your own code. However, some events' triggers were coded in Lua specifically.

Q. Where's the physics engine?
A. I have no idea if I have enough time to complete a good physics engine for the expo!

Q. How does multi-threading work?
A. The Lua VM for each scene runs on its own thread, because the Lua VM is not thread-safe. Drawing happens in a completely separate thread, so the FPS will not be tied to the TPS at all.

Q. Is this secure?
A. No. I haven't done enough testing to make sure that the engine is resistant to common exploits such as buffer overflows. In addition, Lua sandboxing is not in the scope for the expo. If the engine was released to the public after the expo, I would not advise you to run a game from an untrusted source!

## IDE
``TODO: Finish serialization so that it's possible to create a scene and script editor.`` 

## Scripting
Chrus Engine has an event-driven Lua scripting interface that presents game objects as classes by wrapping the basic function operations on Chrus Engine's game objects. See (put something here) for the API documentation.

## Game Objects
Chrus Engine provides a number of objects and functions usable for the creation of games. See the library documentation for an exhaustive listing of functions and their uses.

### Scenes
The highest-level unit in the game that you are given full control of is the scene. The scene encapsulates the links to assets and scripts in a tree, and is editable from the IDE. In Chrus Engine, you can have a maximum of 16 scenes running simultaneously, although cross-scene communication is limited.

### Sprites
The sprite encapsulates a bitmap, or a 2D image. The sprite can have its image data either created through scripting and loading from disk. The sprite has capabilities such as rotation, tinting, and scaling. In addition, it can be translated and flipped.

### Text
Text can be displayed by Chrus Engine with TrueType fonts. Chrus Engine supports UTF-8 encoding for text.

### Primitives
As of writing, primitives are not yet implemented. However, Chrus Engine will have the ability to directly draw primitives onto the display.

### Sounds
Chrus Engine is capable of loading and playing both short-form and long-form audio in many different codecs (with the exception of MP3). The "sound" game object is suited for short-form audio, and the "audiostream" game object is suited for long-form audio. With both the sound and audiostream objects, it's possible to control gain, pan, and speed of the underlying audio, and the audio can be played once or even looped. Lastly, the sound and audiostream will eventually have the capability for real-time postprocessing. It's up to the developer to correctly differentiate the two objects, as using the sound object for long-form audio such as in-game music is extremely memory-inefficient compared to the audiostream.

### Scripts
The script is the unit of the scripting interface, and encapsulates the path for the source code intended for execution. In order to run a script, it must be instantiated as "enabled" and then parented into the scene graph.

