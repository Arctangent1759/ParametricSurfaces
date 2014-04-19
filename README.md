Parametric Surfaces
===================

Creators:
---------
Alexander Chu ID#23460953
Leo Wu ID#23661771

Submitter: Alexander Chu (cs184-ay)
Website: http://http://inst.cs.berkeley.edu/~cs184-ay/as2.html

Platform:
---------

Linux (Hive Machines)

Code:
-----

Header files are located in include/
Source files are located in src/

Features:
---------

We implemented the following features:
- Render Bez Files
- Render Obj Files
- Output bez file given an obj file input
- Output obj file given an bez file input

How To Use:
-----------

*Setup*

To compile our code, run the following in a shell:

make clean && make

This will create an executable located at:

bin/runner


*Usage*

w toggles wireframe mode, and s toggles smooth shading mode.
+ and - zoom in and out.
The arrow keys rotate the object, and shift+arrow keys translate the object.

_bez Files_

To render a bez file, run:

bin/runner <file> <parameter> [-a]

<file> is the path to the bez file to be rendered. It must end in the .bez extension.
<parameter> is the parameter of the adaptive or uniform tesselation.
If the [-a] parameter is present, adaptive tesselation will be performed. Otherwise uniform tesselation will be performed.

_obj files_

To render an obj file, run:

bin/runner <file>

<file> is the path to the obj file to be rendered. It must end in the .obj extension.

_File Output and Conversion_

To output a file, append '-o <outfile>' to the above commands, where <outfile> is the path to the output file.
If the input file is a bez, an obj file will be written.
If the input file is an obj, a bez file will be written.

Examples:
bin/runner bez/teapot.bez 0.01 -a -o teapot.obj
bin/runner obj/dodecahedron.obj -o dodecahedron.bez

When converting an obj file to a bez, the normals are lost, so smooth shading will return the same results as flat shading.
