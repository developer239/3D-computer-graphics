![master](https://github.com/developer239/game-programming/actions/workflows/ci.yml/badge.svg)

# Game Programming

Monorepository for graphics programming and game engine related projects.

### Running the Project on Mac

1) Install dependencies:

```bash
$ brew install cmake ninja sdl2 sdl2_ttf sdl2_image sdl2_mixer
```

2) Build:

```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=$(brew --prefix)/bin/ninja -G Ninja -S . -B build
$ cd build
$ ninja
$ ctest
```

3) Run:

```bash
$ cd build/src/apps/3d-graphics-programming
$ ./3DGraphicsProgramming
```

## 3D Computer Graphics Programming

- [Link To App](/src/apps/3d-graphics-programming)
- [Link To Original](https://pikuma.com/courses/learn-3d-computer-graphics-programming)

Personal take on the course. The original course is in C. I'm using C++ with custom CMake configuration and somewhat clean code architecture.

No GPU, no OpenGL, no DirectX. If you like what you see go checkout the original course it is well worth the price.

![preview-1](/docs/preview-1.gif)
