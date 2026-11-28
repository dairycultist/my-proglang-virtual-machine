# my-proglang-virtual-machine

Ideally the VM would be written in C but it's easier to do that in JS so I'll do that as a proof-of-concept.

# Spec

The virtual machine will come with built-in libraries (boilerplate!). Some ideas:
- `GL` (exposes helpers for opening windows, rendering things, and overall just making desktop apps)
- `Math` (standard math functions, e.g. `:sin(float rad, mut float out)`)
- `CLI` (`:print(char[] string)`, `:readline(mut char[] string)`, etc)
- `Client` and `Server` (for netcode! can open TCP, UDP, HTTP connections, whatever!)

Features
- Interpreted
- Strong typing
- Functional (no hidden side effects)
- Abstraction

### Types

The primitives are:
- `int`; default value `0`
- `real` (for real numbers); default value `0.0`
- `bool`; default value `false`
- `char`; default value `\0`

Arrays of a type behave as their own type (e.g. `int[]` can't be passed as a parameter of type `int`). Arrays are homogeneous. The size of an array is fixed at time of declaration; however, this size can be determined at run-time (see below). The size of an array is stored by the array and can be accessed through the `sizeof` keyword (see below).

```
int x = 10;
mut int[x] my_array; // an array of length 10 filled with 0
int size = sizeof my_array; // 10
```

Strings are represented as char arrays (see below). Strings are NOT null-terminated.

```
char[] my_string = "hello";
CLI:print(sizeof my_string); // "5"
```

I have nooo idea how I'm gonna implement abstract data types.

### Variables

Variables are constant (immutable) unless declared with the `mut` keyword, in which case they are mutable. Variables in the global scope cannot be flagged with the `mut` keyword and so are always constant (otherwise, a function would be able to change this variable, violating 'no hidden side effects').

There is no means for a variable to store a reference to another variable (which would allow a function to change a variable locally that affects another variable non-locally, violating 'no hidden side effects').

### Functions

Function parameters are constant (immutable) unless declared with the `mut` keyword, in which case they are mutable. When passing a variable as a mutable argument to a function, you must explicitly label it with the `mut` keyword, *even though* the function's prototype already labels it as mutable (see below `foo(mut my_num, 5);`); this helps with readability as well as facilitates function overloading. 

Function parameters are *always* pass-by-reference. This includes arguments passed as inline declarations, in which case a temporary variable is created to reference (if the parameter is mutable, the function can freely modify this value, but it will disappear once the function returns).

Array parameters to functions needn't specify a size, in which case an array of any size may be passed. Otherwise, the array being passed must have a size equal to or greater than the parameter's size.

Functions do not return values; the sole output of functions is their mutable parameters (recall that mutable global variables don't exist).

Functions can be overloaded.

```
fn foo(mut int x, int y) {
  x = 20;
  // y = 30; This would be a runtime error (assignment to parameter y of function foo which is not mutable).
}

fn main() {
  mut int my_num = 10;
  foo(mut my_num, 5);
  CLI:print(my_num); // "20"
}
```

### Linkage

By default, everything has internal linkage.

You can specify a symbol have external linkage with `:` (see below `:PI` and `:abs(...)`). Only symbols in the global scope of a file labeled with a package name can have external linkage.

To label a file with a package name, the first line of the file must be the keyword `package` followed by the namespace of the package (convention is to use UpperCamelCase; see below `package Math`).

Symbols with external linkage are accessed through their package (see below `Math:abs(...);`). There is no explicit importing of packages (all are linked automatically).

```
package Math

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
  mut int my_num = -20;
  Math:abs(mut my_num);
  CLI:print(my_num); // "20"
}
```
