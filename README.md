Motion
======

A 3d sample based on Frank Luna's DX11 book. This demo generates simple terrain that warps based on a sine function with vertex colors that interpolate based on height. This means tall terrain becomes white peaked mountains and medium terrain becomes green grass. This code differs from the book version since this code does not depend on the effects 11 framework for compiling and loading shaders and setting constant buffers. 


A note on implementation
========================
The way the terrain and colors are updated is most likely not optimal since it requires the vertex buffer be remapped every frame. I'd prefer to set the vertex buffer data once and then perform the updates in a vertex shader.
