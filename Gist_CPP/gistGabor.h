#ifndef gGabor_H
#define gGabor_H
#include "myImg.h"
#include "singleImg.h"

void gistGabor(myImg gImg,int w,double ***G,int n,int Nfilters,double **&g);
void downN(singleImg x,int NN,double** &);
void getnxny(int h,int w,int num);
#endif
