import os
import sys

SRC = Split('''src/main.cpp
               src/test.cpp
               src/adapter/hog-adapter.cpp
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
                    libimg-coding
                    ''')

env = Environment(LIBPATH=LIB_PATH, LIBS=LIB_NAME, CPPPATH=INCLUDE_PATH, LINKFLAGS='-fopenmp',
                  CFLAGS='-O3', CXXFLAGS='-O3', CXX='g++');
env.ParseConfig('pkg-config --cflags --libs opencv');

env.Program(target="vlfeat-adapter", source=SRC)
