# my-proglang-virtual-machine

virtual machine in C that runs a custom programming language, just so I can have a programming language easier than C that can create a window

the programming language specs
- interpreted (no bytecode)
- strong typing
- function overloading is a thing

the virtual machine has built-in libraries like
- EZGL2D (implements sdl2+glew+opengl abstractly for you and reveals helpers for drawing sprites/text of different rotations, scales, and tints)
- EZGL3D (implements sdl2+glew+opengl abstractly for you and reveals helpers for loading objs, creating meshes manually, drawing those, also loading custom shaders)
