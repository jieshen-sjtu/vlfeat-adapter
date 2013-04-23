#ifndef _SIIMG_H
#define _SIIMG_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <complex>
#include "highgui.h"
#include "matrix.h"
#include "myImg.h"

using namespace std;

class singleImg{
private:
	void xfft();
	void yfft();
	void xifft();
	void yifft();
public:
	int height;
	int width;
	int channel;
	float*** realImg;
	float*** iiImg;
	
	singleImg(myImg my);
	singleImg(int , int , int );
	void starmulti(singleImg ,double*** gf,int n);
	void ifft2();
	void fft2();
	void abs();
	float get(int,int,int);
	float geti(int,int,int);
};

#endif