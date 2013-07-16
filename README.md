vlfeat-adapter
==============

Requirements:
1. scons software
2. OpenCV library, 2.4 or more recent version

Building steps:

---build gist library---

1. cd 3rdparty/lear_gist-1.2/
2. follow the README file in that directory to build the libgist.a library
3. back to the directory where this file is
4. cp 3rdparty/lear_gist-1.2/src/gist.h include/
   cp 3rdparty/lear_gist-1.2/src/fftw3.h include/
   cp 3rdparty/lear_gist-1.2/src/libfftw3f.a lib/
   cp 3rdparty/lear_gist-1.2/src/libgist.a lib/

---build vlfeat library---

1. cd 3rdparty/vlfeat/
2. make
3. back to the place where this file is
4. cp -r 3rdparty/vlfeat/vl include/
5. cp 3rdparty/vlfeat/bin/glnx64a/libvl.so lib/
6. add the lib/ path to LD_LIBRARY_PATH: export LD_LIBRARY_PATH=lib/:$LD_LIBRARY_PATH

---build other source---
1. scons
2. this will generate one exe program vlfeat-adapter for testing, and will copy all the head files and libraries to the include/ and lib/ directory respectively

---test---
1. ./vlfeat-adapter test.jpg

