# Self-Assessment
## Part A: Individual

For Chrus Engine, I worked as an individual. So, my contributions include:
- Design of Chrus Engine's various components
- Design of some assets used in demo
- Implementation of memory management of game assets
- Implementation of data structures used to represent game data
- Implementation of utility data structures such as red-black trees and vectors
- Implementation of multi-threaded engine design
- Implementation of Lua scripting APIs
- Implementation of game demo shown at CEAS Expo

In my initial assessment, I initially identified writing faster algorithms, creating efficient data structures, and avoiding scope creep as necessary skills. The final product was built off of these skills as a foundation-- my game engine can run at a high frame rate, avoids high usage of memory, and despite the scope of the project, I successfully finished a core feature set to use in time for the CEAS Expo.

This project taught me a vast amount about manual memory management, computer graphics, and multithreaded design. Because C places the responsibility of memory management on the programmer, I had to learn to explicitly design the lifetimes of every single allocation that I made. With graphics, I learned about how to write my own shaders, how shaders are just parallelized programs, and how graphics libraries also process arrays of vertices rather than just bitmaps. Lastly, I became a lot better at designing concurrent systems using multi-threading because I learned how to synchronize threads, and how to write code that runs synchronously and runs asynchronously.

My successes with the project were that I was even able to write a game engine, that I managed myself well enough to have time to write a demo, and that I learned so much about systems programming, both from successes and failures. On the other hand, I've also learned a lot from struggling with graph serialization, with graphics in a multi-threaded context, and with the general issues of multi-threaded debugging.
