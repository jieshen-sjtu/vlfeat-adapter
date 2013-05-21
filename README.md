vlfeat-adapter
==============

vlfeat adapter

Requirements:
1. a newest vlfeat code, we didn't include it here, you should put it somewhere then use the "ln" command to indicate the location: ln -s [path/to/vlfeat/vl] include/vl
2. the OpenCV liberary, should be version 2.4.3 or newer
3. scons software, to compile our code

To use this code, follow the step behind:
1. type the command "scons" in the current location, and will get a program "vlfeat-adapter"
2. ./vlfeat-adapter test-img.jpg. By default, it will run an HOG demo. Be sure that the test-img.jpg is bigger than 64 * 128. The HOG demo will only run on the top-right region of the image with size 64 * 128 to give a clear visual effect, not on the whole image.
