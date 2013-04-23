/*function output = prefilt(img, fc)
% ima = prefilt(img, fc);
% fc  = 4 (default)
% 
% Input images are double in the range [0, 255];
% You can also input a block of images [ncols nrows 3 Nimages]
%
% For color images, normalization is done by dividing by the local
% luminance variance.*/

#include "prefilt.h"
#include "myImg.h"
#include "createGabor.h"
#include <math.h>

myImg prefilt(IplImage* img,int fc){
	//fc = 4; % 4 cycles/image
	int w = 5;
	double s1 = fc/sqrt(log(2.0));
	
	int height = img->height+2*w;
	int width = img->width+2*w;

	myImg newImg = myImg(height,width);
	
	newImg.logPadArraySym(img,w);

	int sn,sm,c,N,n;
	sn = newImg.height;
	sm = newImg.width;
	c = newImg.channel;
	N = 1;
	n = sn>sm?sn:sm;
	n = n+n%2;

	newImg.padSymPost(n-sn,n-sm);

	matrix<int> fx(n,n),fy(n,n);
	meshgrid(fx,fy,-n/2,n/2-1);

	matrix<double> gf(n,n);
	gf.squareexps1(fx,fy,s1);
	gf.fftshift();
	

	//because N = 1, the dimention of gf is only 3 which is different from the matlab where it's 4
	double ***newgf = new double**[gf.row];
	for (int i=0;i<gf.row;i++){
		newgf[i] = new double*[gf.col];
		for (int j=0;j<gf.col;j++){
			newgf[i][j] = new double[c];
			for (int k=0;k<c;k++)
				newgf[i][j][k] = gf.get(i,j);
		}
	}

	newImg.getoutput(newgf);

	myImg lstdImg = myImg(newImg.height,newImg.width);
	
	lstdImg.localstd(newImg,newgf);

	newImg.div2trunc(lstdImg,w);

	for (int i=0;i<gf.row;i++){
		for (int j=0;j<gf.col;j++)
			delete[] newgf[i][j];
		delete[] newgf[i];				
	}

	delete[] newgf;
	newgf= NULL;

	return newImg;	
}


