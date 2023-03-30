# POSIX++

C++ bindings for select POSIX things.

This library seeks to provide a pleasant C++20 interface to certain nasty POSIX functions. Which ones? Well, whatever I feel like implementing. I expect them to fall into two categories, for the most part:

- Stuff for which there is no C++ standard library replacement (e.g. socket stuff)
- Stuff for which there is a C++ standard library replacement, but I think I can do better (e.g. condition variables)

On occasion, I expect I'll implement something that doesn't fit into either of those categories. Something that provides new functionality rather than just wrapping existing POSIX functionality. It'll always make use of other parts of this library. I have something in mind already.

Everything is unstable and subject to change.
