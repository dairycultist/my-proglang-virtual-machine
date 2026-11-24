# my-proglang-virtual-machine

Virtual machine written in C that runs a custom programming language, oriented towards minimal boilerplate for making desktop apps (because C is the only programming language I know how to make a window in and I am not doing that for every project).

the programming language specs
- interpreted (not with bytecode)
- strong typing
- function overloading is a thing

## Functional programming

By principle, functions have no hidden side effects. Functions mutate the state solely through their output parameters; mutable global variables aren't permitted (i.e. all global variables are constant).

All variables/function parameters are constant (immutable) unless flagged with the `mut` keyword, in which case they are mutable.

Function parameters are always pass-by-reference. This includes arguments passed as inline declarations, in which case a temporary variable is created to reference (if the parameter is mutable, the function can modify this value, but it will disappear once the function returns).

Functions do not return values; to replicate this functionality, use output parameters.

```
fn foo(mut int x, int y) {
  x = 20;
  y = 30; // runtime error: assignment to parameter y of function foo which is not mutable
}

mut int my_var = 10;
foo(mut my_var);
print(my_var); // "20"
```

## Libraries

Libraries are how you share functions and global-scope constants between files. By default, functions and global-scope constants have internal linkage. Declaring them as part of a library switches them to external linkage.

There is no explicit importing of libraries; all libraries that can be available to your program, are (built-in libraries aren't bundled with your program so there's no bloat, and libraries you define as part of your program are, well, part of your program).

The virtual machine comes with built-in libraries. These include:
- `GL` (implements sdl2+glew+opengl abstractly for you)
  - exposes helpers for drawing sprites/text of different rotations, scales, and tints
  - exposes helpers for loading objs, creating meshes manually, drawing those, also loading custom shaders
- `Math`
  - `:sin(float rad)`
  - `:cos(float rad)`
  - `:abs(int x)`
  - `:abs(float x)`
  - etc
- `CLI` stuff like reading and writing to the command line
- `Client` and `Server` are for netcode

```
fn LibraryName:function_in_library() {
  ...
}

LibraryName:function_in_library();
```
