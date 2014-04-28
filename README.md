License
========
This program is free software distributed under the terms of the Apache License, Version 2.0.

Introduction
============
This program generates random synthetic data akin to Bongard problems for ILP learning. Each example is a "picture" composed of simple geometrical objects (elements). An object can be one of the three shapes: circle, square and triangle. A triangle may be pointing up or down. Each picture cannot have a pair of partially overlapped objects and all objects must be fully enclosed in the boundary of the picture.


Compilation
============
The program is built using CMake on all platforms.

On most systems you can compile the code as simple as

	$ mkdir build && cd build
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	$ make

Dependencies
------------
The C++ compiler must support c++11. The following two Boost libraries (1.50.0 or newer) are required:
* filesystem
* system

Command Line
=========
Two arguments must be provided. The first gives the number of pictures to be generated. The second gives the output directory.

For example, to generate 100 pictures in the current directory,
	
	$ ./bongard_generator 100 ./ 

We do not provide additional command line arguments to custom the generator. However, Changes to the generator parameters (e.g. the size, boundary of pictures, the size of objects) are very easy. You can find them in main.cpp and Configurations.hpp.

Output
=======
Since generating a large dataset takes very long time, the program periodically writes the data into a new directory under the input root path. In each output directory, ten files are generated. The output for each picture and spatial object is an encoded number (ID).
* element. This file gives the picture-object ID pair in the form of "picture_id|object_id".
* circle, triangle, triangle_up, triangle_down, rectangle. The four files store the object IDs for each shape.
* inside, north, east. The three files contain pairs of objects for three position relationships.
* summary. This file gives the configuration parameters and the maximum picture/object IDs in the current directory.

Given some target clauses, in order to generate a training set for ILP systems, one can first load the files into a RDBMS and execute queries for target clauses.
