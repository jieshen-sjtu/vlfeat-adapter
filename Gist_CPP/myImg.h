#ifndef _MYIMG_H
#define _MYIMG_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "highgui.h"
#include "matrix.h"
#include "fftw3.h"
#pragma comment(lib,"libfftw3-3.lib")
#pragma comment(lib,"libfftw3f-3.lib")
#pragma comment(lib,"libfftw3l-3.lib")
using namespace std;

class myImg{
private:
	void xfft();
	void yfft();
	void xifft();
	void yifft();
public:
	int height;
	int width;
	int channel;
	double*** realImg;
	double*** iiImg;
		
	myImg(int h,int w);
	
	void logPadArraySym(IplImage* img,int w);
	void padSymPost(int x,int y);
	void fft2();
	void ifft2();
	void starmulti(double*** gf);
	void getoutput(double*** gf);
	void localstd(myImg newImg,double*** gf);
	double get(int , int , int);
	void div2trunc(myImg lstdImg,int );
	void destroy();
};
#endif
