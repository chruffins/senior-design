# Test Plan
## Overall Test Plan
Testing for Chrus Engine will involve testing individual system components as well as testing the entire system. Individual system components will be tested through the use of sequences of functions and sample data, which will cover both valid actions and data as well as invalid actions and data.

## Test Case Descriptions
### Allocation Test 1
- This test will ensure that the internal red-black tree used for memory allocation of assets works correctly.
- This test will attempt to add a number of assets to the red-black tree, remove some, and then attempt to add more assets.
- Inputs: A list of valid asset links. 
- Outputs: The valid assets will be correctly added to the red-black tree. Redundant assets will return pointers to the original asset instead of allocating separately.
- Normal
- Functional
- Unit

### Allocation Test 2
- This test will ensure that the internal red-black tree used for memory allocation of assets correctly rejects invalid assets.
- This test will attempt to add invalid assets to the red-black tree.
- Inputs: A list of invalid links and assets.
- Outputs: The tree will remain empty.
- Abnormal
- Functional
- Unit

### Allocation Test 3
- This test will ensure that the internal red-black tree used for asset allocation correctly rejects invalid actions.
- This test will feed invalid data to the outward facing functions of the allocator.
- Inputs: A list of invalid data and functions.
- Outputs: The tree will not change.
- Abnormal
- Functional
- Unit

### Serialization Test 1
- This test will ensure that serialization of scenes occurs correctly.
- This test will attempt to serialize scenes.
- Inputs: A list of populated scenes
- Outputs: A list of files storing the scenes
- Normal
- Functional
- Unit

### Deserialization Test 1
- This test will ensure that loading of scenes occurs correctly.
- This test will attempt to deserialize scenes.
- Inputs: A list of files containing valid scenes
- Outputs: Scenes contained in memory
- Normal
- Functional
- Unit

### Deserialization Test 2
- This test will ensure that loading of invalid scenes fails.
- This test will attempt to deserialize invalid files.
- Inputs: A list of files containing invalid scenes.
- Outputs: No scenes loaded into memory and an acknowledgement of errors
- Abnormal
- Functional
- Unit

### Deserialization Test 3
- This test will ensure that loading of scenes is timely.
- This test will load scenes with data sizes varying in orders of magnitude.
- Inputs: A list of scenes, ranging from small to very large.
- Outputs: Scenes loaded into memory, in a timely manner.
- Normal
- Performance
- Unit

### Drawing Test (Correctness)
- This test will ensure that rendering occurs without runtime errors as well as correctly.
- This test will load a scene with various renderables and render.
- Inputs: A scene utilizing multiple sprites
- Outputs: Screen renders these sprites in-order.
- Normal
- Functional
- Integration

### Drawing Test (Performance)
- This test will ensure that rendering occurs in a timely manner.
- This test will load a scene with a huge number of sprites.
- Inputs: A scene utilizing a huge number of sprites.
- Outputs: Screen renders scene at an acceptable framerate.
- Normal
- Performance
- Integration

### User Input Test (Correctness)
- This test will ensure that user input is handled correctly.
- This test will load a interactable scene.
- Inputs: A scene with multiple interactable objects and a list of user interactions.
- Outputs: The game correctly handles the user interactions.
- Normal
- Functional
- Integration
