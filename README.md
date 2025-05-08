# hawsoo_sentou_goraku_engine
Game engine made for 3D combat adventure games. Yes this is what HSGE stands for.


## @NOTE: Canning this project.

This is seen as an interesting experiment on a decentralized job system (or at least the best attempt I could muster for getting this). Unfortunately, since so many parts are so incompatible, the friction developed on this software development and architecting paradigm has screeched this project to a halt. It is unfortunately too much for a single individual to pioneer thru.

Some things that were important to note:
- The job system halved the framerate (likely due to all of the context switching from the jobs).
- All of the atomic operations were a headache to manage and keep visibility.
- It was really interesting to see that despite the all-core job system, there wasn't enough throughput in the job system to exercise all of the cores.
- GLFW needed to be run single core.
    - The input reading and window operations were VERY thread-unsafe.
- Vulkan was able to be run on multiple cores.
    - It would have been better if it were more organized, such as being single threaded for setup and teardown, and multithreaded for descriptor set and command buffer sub-pass writing.
- There definitely would've been benefit in a "god job" which kept track of the timing of all the workings of the different things that the game engine needed.

### So for the future, the software process should be:

1. Starting out with an easy to build application.
Building and reducing friction until all features are built is necessary. So this means:
- Singlethreaded.
- One cmake package.
- (Possibly) bundling the game engine and the game together, and then separating the two later?
    - This may need to be pre-planned.
- Pre-planning the software architecture before building software.
    - So that interface marriage and consumption doesn't need to be a huge part of planning each individual package.


1. Moving from a feature-complete slow application to a feature-complete fast application.
Frictionless dev is sacrificed a little bit as the application works towards creating speed increases where needed. Friction is still a huge danger, and tech debt is looming here now too.
- Multithreaded.
    - Looking into different job system paradigms and incorporating these.
- 1-3 cmake packages.
    - Perhaps one for the job system as a subdirectory.
- Separate game engine from game.
    - Asset loading must not be hardcoded into the game/game engine binary.
        - Use json/yaml for loading these! (Or pak files).
- A loooooot of profiling.
    - Improvement of assets at the same time as improvement of speed.
- Removing tech debt.
    - This is a very built piece of software at this point so it will need to be cleaned and organized a few times.


1. Optional: Moving to a componentized game engine that is fast.
This is really only for organizational practices. This is likely where the real game developer would rather go in and make their game instead of focusing on the game engine.
- Componentized and pretty code.
