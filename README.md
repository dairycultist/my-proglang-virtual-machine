# my-proglang-virtual-machine

Virtual machine written in C that runs a custom programming language, oriented towards minimal boilerplate for making desktop apps (because C is the only programming language I know how to make a window in and I am not doing that for every project).

the programming language specs
- interpreted (not with bytecode)
- strong typing
- function overloading is a thing
- functional; functions have no hidden side effects (i.e. 'imperative' functions mutate the state through their output parameters)

the virtual machine has built-in libraries like
- `GL` (implements sdl2+glew+opengl abstractly for you)
  - exposes helpers for drawing sprites/text of different rotations, scales, and tints
  - exposes helpers for loading objs, creating meshes manually, drawing those, also loading custom shaders
- `Math` (your bog-standard math library, stuff like trig)

since it's a virtual machine with namespaces for libraries there's no explicit importing of libraries, you just have all of them available to you immediately. the syntax is

```
LibraryName:function_in_library();
```

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
