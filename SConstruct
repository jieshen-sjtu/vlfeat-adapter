import os
import sys

SRC = Split('''src/main.cpp
               src/test.cpp
               src/utils.cpp
               src/adapter/basic-adapter.cpp
               src/adapter/hog-adapter.cpp
               src/adapter/sift-adapter.cpp
               src/adapter/gist-adapter.cpp
               src/adapter/dsift-adapter.cpp
               src/adapter/lbp-adapter.cpp
               src/adapter/kmeans-adapter.cpp
                              ''')

INCLUDE_PATH = Split('''include
                        /usr/include
                        ''')

LIB_PATH = Split('''lib
                    /usr/local/lib
                    /usr/lib
                    ''')

LIB_NAME = Split('''
                    libvl
                    libopencv_core
                    libopencv_highgui
                    libopencv_imgproc
                    libimg-coding
                    libgist
                    libfftw3f
                    libm
                    ''')

env = Environment(LIBPATH=LIB_PATH, LIBS=LIB_NAME, CPPPATH=INCLUDE_PATH, LINKFLAGS='-fopenmp',
                  CFLAGS='-O3', CXXFLAGS='-O3', CXX='g++');
#env.ParseConfig('pkg-config --cflags --libs opencv');

env.Program(target="vlfeat-adapter", source=SRC)

libname = 'libhog-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/basic-adapter.cpp','src/adapter/hog-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))

libname = 'libsift-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/basic-adapter.cpp','src/adapter/sift-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))

libname = 'libdsift-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/basic-adapter.cpp', 'src/adapter/dsift-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))

libname = 'libgist-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/basic-adapter.cpp', 'src/adapter/gist-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))

libname = 'liblbp-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/basic-adapter.cpp', 'src/adapter/lbp-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))


libname = 'libkmeans-adapter.a'
env.StaticLibrary(target=libname, source=['src/adapter/kmeans-adapter.cpp'])
Command('lib/'+libname, libname, Move("$TARGET", "$SOURCE"))
