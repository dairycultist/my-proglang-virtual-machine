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
package MyPkg

// "real" is the implementation-agnostic equivalent of float/double.
real :PI = 3.141592;

fn :foo(mut int x, int y) {
  x = 20;
  // y = 30; This would be a runtime error (assignment to parameter y of function foo which is not mutable).
}
```

```
// The main function is your program's entrypoint. because it isn't preceded by ":",
// it has internal linkage.
fn main() {
  mut int my_num = 10;
  MyPkg:foo(mut my_num);
  CLI:print(my_num); // "20"
}
```

The virtual machine will come with built-in libraries. Some ideas:
- `GL` (exposes helpers for opening windows and rendering things)
- `Math` (standard math functions, e.g. `:sin(float rad, mut float out)`)
- `CLI` (`:print(idkhowtorepresentstrings)`, `:readline(mut idkhowtorepresentstrings)`, etc)
- `Client` and `Server` (for netcode! can open TCP, UDP, HTTP connections, whatever!)

### Types

The primitives are `int`, `real` (for real numbers), and idk what I want for the rest.

Arrays of a type count as their own type (e.g. `int[5]`). Arrays are homogeneous. The size of an array is fixed at time of declaration; however, this size can be determined at run-time (see below).

```
int x = 10;
int[x] y; // filled with 0
```

I have nooo idea how I'm gonna implement abstract data types.

### Variables

Variables are constant (immutable) unless declared with the `mut` keyword, in which case they are mutable. Variables in the global scope cannot be flagged with the `mut` keyword and so are always constant.

Variables cannot store a reference to another variable (which would allow a function to change a variable locally that affects another variable non-locally, violating 'no hidden side effects').

### Functions

Function parameters are constant (immutable) unless declared with the `mut` keyword, in which case they are mutable. When passing a variable as a mutable argument to a function, you must explicitly label it with the `mut` keyword (*even though* the function's prototype already labels it as mutable; see above `MyPkg:foo(mut my_num);`).

Function parameters are *always* pass-by-reference. This includes arguments passed as inline declarations, in which case a temporary variable is created to reference (if the parameter is mutable, the function can freely modify this value, but it will disappear once the function returns).

Functions do not return values; the sole output of functions is their mutable parameters (recall that mutable global variables don't exist).

### Linkage

By default, everything has internal linkage.

You can specify a symbol have external linkage with `:` (see above `:PI` and `:foo(...)`). Only symbols in the global scope can have external linkage.

Symbols with external linkage are accessed through their package (see above `MyPkg:foo(...);`). There is no explicit importing of packages (all are linked automatically).
