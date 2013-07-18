vlfeat-adapter
==============

Requirements:
1. scons software
2. OpenCV library, 2.4 or more recent version

---Building---

./build-all

It will build all static libraries and place them into the "lib" directory.
All associated head files are placed in the "include" directory

*|--- Imoprtant ---|*
Actually, after your first calling for "build-all", if you want to modify the test program "src/test.cpp",
you just need to type "scons" to rebuild the program rather than calling "build-all" again.
In short, you are recommended to call "build-all" only once. 

---test---
1. ./vlfeat-adapter test.jpg

Then some demos will run as you input your selection.

---clean---

./clean-all

It will clean all the files during the building process, including the static libraries and head files.

