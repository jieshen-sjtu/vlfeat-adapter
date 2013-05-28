vlfeat-adapter
==============

vlfeat adapter

Software Requirements:
1. a newest vlfeat code for the [head files] and the shared library [libvl.so].
   1.1 head files
       use the "ln" command to indicate the directory of head files: e.g. ln -s "path/to/vlfeat/vl" include/vl
   1.2 shared library
       use the "ln" command or just put the "libvl.so" into "lib" directory

2. the OpenCV liberary, should be version 2.4.3 or newer

3. fftw: www.fftw.org, this is required for extracting the GIST feature.
   NOTE: when run "configure" command, please add the parameter --enable-single so as to output the float version of static library.

4. scons software, to compile our code. It is based python, so be sure to have install python.

To use this code, follow the step behind:
1. type the command "scons" in the current location, and will get a program "vlfeat-adapter"
2. ./vlfeat-adapter test-img.jpg. By default, it will run an HOG demo. Be sure that the test-img.jpg is bigger than 64 * 128. The HOG demo will only run on the top-right region of the image with size 64 * 128 to give a clear visual effect, not on the whole image.
