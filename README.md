# my-proglang-virtual-machine

A simple spec for a simple programming language. Ideally the VM would be written in C but it's easier to do that in JS so I'll do that as a proof-of-concept.

Planned features:
- Interpreted
- Strong typing
- Functional (no hidden side effects)
- Abstraction
- function overloading is a thing

At some point it'd be nice for the lang to be used for making desktop apps with minimal boilerplate but we'll cross that bridge

# Ideas idk

```
package Math

// "real" is the implementation-agnostic equivalent of float/double.
real :PI = 3.141592;

fn :abs(mut int x) {
  if (x < 0) {
    x = -x;
  }
}
```

```
// The main function is your program's entrypoint. because it isn't preceded by ":",
// it has internal linkage.
fn main() {
  mut int my_num = -10;
  Math:abs(mut my_num);
  print(my_num); // 10
}
```

In the above example code (two files), the global-scope constant `:PI` and the function `:abs` are preceded by a `:` to indicate that they have external linkage. You access these through their package, which is demonstrated in the second file. By default, everything has internal linkage; only **functions** and **global-scope constants** can have external linkage (i.e. a variable in the global scope flagged with `mut` cannot also be flagged with `:`).

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
