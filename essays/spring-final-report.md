# Chrus Engine
## Table of Contents

* [Chrus Engine](#chrus-engine)
  * [Table of Contents](#table-of-contents)
  * [Project Description](#project-description)
  * [User Interface Specification](#user-interface-specification)
  * [Test Plan and Results](#test-plan-and-results)
  * [User Manual](#user-manual)
  * [Spring Slidedeck](#spring-slidedeck)
  * [Final Expo Poster](#final-expo-poster)
  * [Assessments](#assessments)
    * [Fall Semester](#fall-assessment)
    * [Spring Semester](#spring-assessment)
  * [Summary of Hours and Justification](#summary-of-hours)
  * [Summary of Expenses](#summary-of-expenses)
  * [Appendix](#appendix)
    * [References](#references)

## Project Description

Chrus Engine is a game engine intended for the development of lightweight, cross-platform, and performant 2D games. For this, Chrus Engine provides memory management, an audio engine, a rendering engine, serialization and deserialization of games, and a Lua scripting interface. The rendering engine includes support for bitmaps, animations, primitives, and shaders. Lastly, Chrus Engine is multithreaded, which enables drawing and logic to be processed in parallel.

## User Interface Specification

As the users of Chrus Engine are intended to be game developers, 

## [Test Plan and Results](./test-plan.md)

The test plan, which includes the description of each test, can be viewed using the hyperlink in header.

### Allocation Test 1
Attempting to add assets to the red-black tree implementation, remove some assets, then add more assets to the red-black tree worked successfully. Inspecting the red-black tree in a debugger showed that the red-black tree was consistent and the pointers were correctly set.

### Allocation Test 2

Attempting to add invalid assets to my implementation of the red-black tree correctly returned null pointers, which indicated the rejection of invalid assets. Inspecting the red-black tree showed that the red-black tree was empty, which shows that no assets were added.

### Allocation Test 3

The red-black tree was used to add multiple assets, both valid and invalid. Inspection of the red-black tree showed that the red-black tree only contained assets that had valid links, which confirmed correctness.

### Serialization Test 1

Attempts to serialize a scene whose objects were created with a manually loaded in script resulted in a JSON file with correct syntax.

### Deserialization Test 1

The attempt to deserialize scenes from JSON files created in Serialization Test 1 succeeded. Inspection of the scene objects afterwards showed that the scenes were correctly reconstructed from a file.

### Deserialization Test 2

The attempt to deserialize scenes with missing information as well as corrupted keys succeeded. The function call that deserialized the faulty scene correctly returned a null pointer, indicating the presence of errors in the scene file.

### Deserialization Test 3

Scenes were procedurally created with varying numbers of text objects, up to 10,000 text objects. Loading each scene happened in less than a second.

### Drawing Test (Correctness)

A scene containing sprites, text, and primitives was created procedurally with a script. The script created each object in order, and the rendering showed that objects by default would be rendered back-to-front, where objects created first were rendered in the back.

### Drawing Test (Performance)

A scene containing 20,000 sprites was created procedurally with a script. The game engine was capable of rendering all of them rotating at various speeds at 60 frames per second on an NVIDIA GTX 1650 graphics card.

### User Input Test (Correctness)

A scene containing objects with set callbacks for keyboard and mouse input was created procedurally with a script. Attempts to interact with objects correctly showed that user input would be intercepted by order of callback creation. 

## [User Manual](./user-manual.md)

See the hyperlink in header.

## [Final Presentation](https://mailuc-my.sharepoint.com/:p:/g/personal/lee4cr_mail_uc_edu/EfFUymUihGxEsIx5ehUlDCMB4TGkaVDDMw7ZG9X9N1n9Mw?e=O8ah1R)

See the hyperlink in header.

## Assessments

### [Fall Assessment](./capstone-assessment.md)

### [Spring Assessment](./self-assessment.md)

## Summary of Hours and Justification

My total effort for this semester comes to approximately 65 hours of effort, which adds with last semester's 45 hours of effort to come to a total of 110 hours of effort expended on this project.

At least 60 hours of my time were spent on the design and development of Chrus Engine's core systems this semester, and 5 hours were spent designing and developing the technical demo presented at CEAS Expo. The time spent writing code can be seen through the commits, which are all credited to me. I also spent time outside of writing code doing research on shader programming, OpenGL, and LuaJIT.

## Summary of Expenses

There were no expenses incurred in the creation and development of Chrus Engine. However, Chrus Engine is dependent on these free and open source software libraries: pdjson, Allegro 5, and LuaJIT.

## Appendix

The final repository for Chrus Engine can be found [here](https://www.github.com/chruffins/senior-design).
