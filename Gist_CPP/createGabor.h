#ifndef Gabor_H
#define Gabor_H
#include<stdio.h>
#include"matrix.h"

void createGabor(double *** &G,int ore[], int n);
int ssum(int a[]);
void meshgrid(matrix<int> &fx,matrix<int> &fy,int s,int e);

#endif
