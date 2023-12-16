# Physics Simulator

This is a little project I'm working on to learn c++, physics and in general to expand my brain and make something cool on the way.

This repository was created using the [CMake SFML Project Template](https://github.com/SFML/cmake-sfml-project).

## Images

![Image 1](images/image_1.png)

![Image 3](images/image_3.png)

## Explanation and controls

Current version simulates gravitational force exerted from a single point.

Click or drag to create particles. Right click to remove them.

The red lines show the acceleration of the particles and the yellow lines show the velocity.

Note that the text showing fps and particle count only works when running the program from the main project source directory (run ``` ./build/PhysicsSimulator.exe ```)

## How to download

Go to [Releases](https://github.com/CoderXam/PhysicsSimulator/releases/) and download any zipped file from the list.
 
## Building the project yourself using cmake

1. You will need:
- CMake
- Something to build executables (example: GNU Make or MinGW Make)
- Something to compile c++ code (example: gcc/g++)

1. If you want to add or remove any .cpp files, open [CMakeLists.txt](CMakeLists.txt) and change the source files listed in the [`add_executable`](CMakeLists.txt#L10) call in CMakeLists.txt to match the source files your project requires.
1. To generate the build system:
    ```
    cmake -S <source-directory> -B <build-directory> -G <generator-name>
    ```
    If no generator is specified cmake will choose a default. If you are on windows this will likely be a visual studio solution generator which I personally tend to avoid. For the build directory I recommend creating a directory called "build" in the main project directory.

1. Once the files have been generated you can then run Make or use the command
    ```
    cmake --build <build-directory>
    ```

    For help using cmake run
    ```
    cmake --help
    ```
1. If you want to make any changes to existing source files, save the files then run make or enter the cmake --build command again to rebuild the project. No need to recreate the makefiles (or equivalent).
1. The executable will be created somewhere in the build directory.
1. Not sure how to properly set up for debug and release modes yet. I think it builds in release by default.

## Resources

Some resources that helped me out for this project:

Low Level Game Dev: https://www.youtube.com/@lowlevelgamedev9330 (c++, game dev, inspiration)

The Cherno: https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb (c++ content and tutorials)

Code, Tech, and Tutorials: https://www.youtube.com/@CodeTechandTutorials (c++ and cmake tutorials)

SFML: https://www.sfml-dev.org/learn.php (sfml help and tutorials)

This guy is also cool (javidx9/One Lone Coder): https://www.youtube.com/@javidx9

Desmos is always good for general math and finding the right math functions https://www.desmos.com/calculator

Of course Stack Overflow and ChatGPT have also been good help for this project as well as other random forums and stack exchanges. 

## License

The source code is dual licensed under Public Domain and MIT -- choose whichever you prefer.