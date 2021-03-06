libcollections
==============

A static C library that provides generic collections (inspired by Java collections).

How to build
------------

Use the makefile to build the lib (bin/libcollections.a):
``` bash
make # Build the lib and the samples, and run the unit tests
make clean # Clean the temporary and objects file.
```

Collections
-----------

libcollections provides different types of collections (see the samples folder) :

- Linked list
- Array list
- Stack *(using an array list)*
- Deque *(Double-ended linked queue)*
- Queue *(linked queue)*

Have a look to the include file ("include/collections/name_of_collection.h") to see the available functions.

Tools
-----

- Written with Emacs 24
- Built with gcc 4.8, tests run by CUnit 2.1-0
- Tested on GNU/Linux (Linux Mint 16)
