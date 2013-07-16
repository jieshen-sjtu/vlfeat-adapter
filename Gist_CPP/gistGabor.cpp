#include "gistGabor.h"

int* nx = NULL;
int* ny = NULL;
/*
nx = fix(linspace(0,size(x,1),N+1));
ny = fix(linspace(0,size(x,2),N+1));
*/
//double ***y;
/*
 Input:
   gImg = input image (it can be a block: [nrows, ncols, c, Nimages])
   w = number of windows (w*w)
   G = precomputed transfer functions = [n n Nfilters]
   n = size(G,1)
   Nfilters = size(G,3);

 Output:
   g: are the global features = [Nfeatures Nimages], 
                    Nfeatures = w*w*Nfilters*c
*/

void gistGabor(myImg gImg,int w,double ***G,int n,int Nfilters,double ** &g){
	//[n n Nfilters] = size(G);
	// w = 4 n = 128 Nfilters = 20 W = 16 N = 3
	int nrows = gImg.height,ncols = gImg.width;
	
	int c=gImg.channel,N = c;
	
	int W = w*w;

	//g = zeros([W*Nfilters N]);
//	if (g==NULL) {
		g = new double*[W*Nfilters];
		for (int i=0;i<W*Nfilters;i++){
			g[i] = new double[N];
			for (int j=0;j<N;j++)
				g[i][j] = 0;
		}
	//}

	singleImg sinImg = singleImg(gImg);
	sinImg.fft2();
	
	//ig = abs(ifft2(img.*repmat(G(:,:,n), [1 1 N]))); 
	singleImg ig = singleImg(sinImg.height,sinImg.width,sinImg.channel);

	int k=0;
	double **v= new double*[W];
	for (int i=0;i<W;i++)
		v[i] = new double[N];

	//----------------prepare work---------------------
	if (nx == NULL)
		getnxny(ig.height,ig.width,w+1);
	//----------------prepare work-------------------------


	for (int i=0;i<Nfilters;i++){
		ig.starmulti(sinImg,G,i);
		ig.ifft2();
		ig.abs();
		downN(ig,w,v);


		//g(k+1:k+W,:) = reshape(v, [W N]);
		// v : [W N] reshape was done downN
		for (int p=k;p<k+W;p++)
			for (int q=0;q<N;q++)
				g[p][q] = v[p-k][q];
		k = k + W;
	}
		

	if (c == 3){
		 /* If the input was a color image, then reshape 'g' so that one column
			 is one images output:
			g = reshape(g, [size(g,1)*3 size(g,2)/3]);  */
		int row = W*Nfilters;
		int col = N;
		double **gg = new double*[row*3];

		for (int i=0;i<row*3;i++){
			gg[i] = new double[col/3];
			for (int j=0;j<col/3;j++){
				if (i>=0 && i<row) gg[i][j] = g[i][j*3];
				else if (i>=row && i<2*row) gg[i][j] = g[i-row][j*3+1];
				else if (i>=2*row) gg[i][j] = g[i-2*row][j*3+2];
			}
		}

		for (int i=0;i<row;i++)
			delete[] g[i];
		delete[] g;
		g = gg;
	}
	
	for (int i=0;i<W;i++)
		delete[] v[i];
	delete[] v;
	v = NULL;
}

void getnxny(int h,int w,int num){
	nx = new int[num];
	ny = new int[num];

	for (int i=0;i<num;i++){
		nx[i]=(int)floor(h*i*1.0/(num-1));
		ny[i]=(int)floor(w*i*1.0/(num-1));
	}
}
/*
% averaging over non-overlapping square image blocks
%
% Input
%   x = [nrows ncols nchanels]
% Output
%   v = [NN NN nchanels]
*/
void downN(singleImg x,int NN,double** &v){
	int count;
	double sum=0;
	for (int xx=0;xx<NN;xx++)
		for (int yy=0;yy<NN;yy++){
			for(int k=0;k<x.channel;k++){
				sum =0;
				count = 0;

				for (int p=nx[xx];p<nx[xx+1];p++)
					for (int q=ny[yy];q<ny[yy+1];q++){
						sum+=x.get(p,q,k);
						count++;
					}

				//y[xx][yy][k] = sum/count; reshape
				v[xx+yy*NN][k] = sum/count;
			}
		}
}
