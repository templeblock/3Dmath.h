## 3Dmath.h
A tiny public domain 3D math library in C.

My intentions with this project are to make a simple 3D math library 
that can be used in 3D games as well as visualization applications. At 
this juncture, it is definitely better suited for the former.

This library isn't fully developed or optimized, so I cannot recommend 
its use in anything mission critical. It's also kind of messy--the 
namespacing could be made to look much neater, for instance.

#### Features
* Basic vector and matrix math (currently only with 3D vectors and 4x4 
  matrices).
* Support for creating perspective projection and viewspace 
  transformation matrices.
* Works with OpenGL: in calls to glUniformMatrix4fv and similar, just
  pass the matrix held in seMat4 and GL_TRUE to transpose.
* Only one file to include.

#### Usage
`#define SE_OPENGL` if you are using OpenGL, then 
`#include "3Dmath.h"`.
