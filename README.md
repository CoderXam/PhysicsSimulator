# Physics Simulator

This is a little project I'm working on to learn c++, physics and in general to expand my brain and make something cool on the way.

This repository was created using the [CMake SFML Project Template](https://github.com/SFML/cmake-sfml-project).

## How to build the project

1. You probably need to be able to compile c++ source code.
1. If you want to add or remove any .cpp files, open [CMakeLists.txt](CMakeLists.txt) and change the source files listed in the [`add_executable`](CMakeLists.txt#L10) call in CMakeLists.txt to match the source files your project requires.
1. Configure and build the project. Most popular IDEs support CMake projects with very little effort on your part.
    - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
    - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
    - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
    - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

    Using CMake from the command line is straightforward as well.

    For a single-configuration generator (typically the case on Linux and macOS):
    ```
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

    For a multi-configuration generator (typically the case on Windows):
    ```
    cmake -S . -B build
    cmake --build build --config Release
    ```
1. Enjoy!

## License

The source code is dual licensed under Public Domain and MIT -- choose whichever you prefer.
