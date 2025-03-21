# Engine Design.

## multithreaded_job_system
Configurable job system that uses 100% of the CPU cores.

Every service in the engine will be a job source.

## ticking_world_simulation (service)
The crank for the whole entire actual simulating game.

Simulating logic and physics are executed here.

The ticking world simulation

## monolithic_renderer (service)
Queries for the data from ticking_world_simulation to create a visual representation of the game.

This can run variably or with a frame limit.

### Auxiliary functions.
Drives input querying since it (on Windows at least) also handles the window which receives keyboard, mouse, and gamepad input. However, monolithic_renderer sends the input data to input_handling.

Loads all 3D models, shaders, and textures (@TODO) into GPU memory. Though it's arguably what should already happen with a renderer.

## input_handling
Receives keyboard, mouse, and gamepad input to process and update a virtual gamepad.

This virtual gamepad is globally accessible for actions that use input.

## 
