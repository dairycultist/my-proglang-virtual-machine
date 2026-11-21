# my-proglang-virtual-machine

Virtual machine written in C that runs a custom programming language, oriented towards minimal boilerplate for making desktop apps (because C is the only programming language I know how to make a window in and I am not doing that for every project).

the programming language specs
- interpreted (not with bytecode)
- strong typing
- function overloading is a thing
- functional; functions have no hidden side effects (i.e. 'imperative' functions mutate the state through their output parameters)



functions are always pass-by-reference (even for inline declarations, which just create a temporary variable to reference). parameters are constant/immutable unless specified as mutable. returning isn't a thing (just use a mutable output variable)

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

Libraries are how you share functions (and constants?) between files. Functions that belong to a library have external linkage. Otherwise, all functions (and global variables) have internal linkage.

There is no explicit importing of libraries; all libraries that can be available to your program, are (built-in libraries aren't bundled with your program so there's no bloat, and libraries you define as part of your program are, well, part of your program).

The virtual machine comes with built-in libraries. These include:
- `GL` (implements sdl2+glew+opengl abstractly for you)
  - exposes helpers for drawing sprites/text of different rotations, scales, and tints
  - exposes helpers for loading objs, creating meshes manually, drawing those, also loading custom shaders
- `Math` (your bog-standard math library, stuff like trig)

```
fn LibraryName:function_in_library() {
  ...
}

LibraryName:function_in_library();
```
