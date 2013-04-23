/*	function G = createGabor(or, n)
%
% G = createGabor(numberOfOrientationsPerScale, n);
%
% Precomputes filter transfer functions. All computations are done on the
% Fourier domain. 
%
% If you call this function without output arguments it will show the
% tiling of the Fourier domain.
%
% Input
%     numberOfOrientationsPerScale = vector that contains the number of
%                                orientations at each scale (from HF to BF)
%     n = imagesize (square images)
%
% output
%     G = transfer functions for a jet of gabor filters */

#include "highgui.h"
#include "createGabor.h"
#include "matrix.h"
#include <cmath>
#include<iostream>

void createGabor(double *** &G,int ore[], int n){

	int Nscales = sizeof(ore);
	
	int Nfilters = ssum(ore);

	int l=0;
	matrix<double> param(Nfilters,4);

	matrix<int> fx(n,n),fy(n,n);

	for (int i=0;i<Nscales-1;i++){
		for (int j=1;j<=ore[i];j++){
			param.set(l,0,0.35);
			param.set(l,1,0.3/pow(1.85,i));
			param.set(l,2,16*pow((double)ore[i],2)/pow(32.0,2));
			param.set(l,3,(j-1)*PI/(ore[i]));
			l=l+1;
		}
	}

	// Frequencies:
	meshgrid(fx,fy,-n/2,n/2-1);

	matrix<double> fr(n,n),t(n,n),tr(n,n);
	fr.squaresqrt(fx,fy);
	fr.fftshift();
	t.angle(fx,fy);
	t.fftshift();	


	//% Transfer functions:
	//G=zeros([n n Nfilters]);
	double dtmp;
	
	G = new double **[n];
	for(int i=0;i<n;i++){
		G[i] = new double*[n];
		for (int j=0;j<n;j++)
			G[i][j] = new double[Nfilters];
	}

	for (int i=0;i<Nfilters;i++){
		//par=param(i,:);
		tr.add(t,param.get(i,3)); 
		tr.pi();

		for (int p =0;p<n;p++){
			for(int q=0;q<n;q++){
				dtmp = exp(-10.0*param.get(i,0)*pow(fr.get(p,q)/n/param.get(i,1)-1,2)-2*param.get(i,2)*PI*pow(tr.get(p,q),2));
				G[p][q][i] = dtmp;
			}
		}				 
		//G(:,:,i)=exp(-10*param(i,1)*(fr/n/param(i,2)-1).^2-2*param(i,3)*pi*tr.^2);
	}

/*	for showing the figure
if nargout == 0
    figure
    for i=1:Nfilters
        max(max(G(:,:,i)))
        contour(fftshift(G(:,:,i)),[1 .7 .6],'r');
        hold on
        drawnow
    end
    axis('on')
    axis('square')
    axis('ij')
end*/
}

int ssum(int a[]){
	int s=0;

	for (int i=0;i<sizeof(a)-1;i++){
		s = s+a[i];
	}
	return s;
}

void meshgrid(matrix<int> &fx,matrix<int> &fy, int start,int end){
	int k = 0;
	for (int i=0;i<=end-start;i++){
		k = start;
		for (int j=0;j<=end-start;j++){
			fx.set(i,j,k);
			fy.set(j,i,k);
			k++;
		}
	}
}
