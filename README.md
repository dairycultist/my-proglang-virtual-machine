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

**All variables/function parameters** are constant (immutable) unless flagged with the `mut` keyword, in which case they are mutable. When passing a variable as a mutable argument to a function, you must explicitly label it with the `mut` keyword (*even though* the function's prototype already labels it as mutable; see above `Math:abs(mut my_num);`).

Function parameters are always pass-by-reference. This includes arguments passed as inline declarations, in which case a temporary variable is created to reference (if the parameter is mutable, the function can modify this value, but it will disappear once the function returns).

The sole output of functions is their mutable parameters (functions don't return values, and variables in the global scope must be constant).

By default, everything has internal linkage; only things in the global scope can have external linkage (i.e. functions and global-scope constants). This is indicated by `:` preceding their keyword (see above `:PI` and `:abs(...)`). You access these through their package (see above `Math:abs(...);`).

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

The virtual machine will come with built-in libraries. Some ideas:
- `GL` (exposes helpers for opening windows and rendering things)
- `Math` (standard math functions, e.g. `:sin(float rad, mut float out)`)
- `CLI` (`:print(idkhowtorepresentstrings)`, `:readline(mut idkhowtorepresentstrings)`, etc)
- `Client` and `Server` (for netcode! can open TCP, UDP, HTTP connections, whatever!)
