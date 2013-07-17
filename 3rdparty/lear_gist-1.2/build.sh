#!/bin/bash
tar -zxvf fftw.tgz
cd fftw-3.3.3/
./configure --enable-single
make
cp api/fftw3.h ../src
cp .libs/libfftw3f.a ../src
cd ../src
rm -rf fftw-3.3.3
scons
