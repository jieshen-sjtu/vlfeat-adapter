#include"singleImg.h"

singleImg::singleImg(myImg my){
	height = my.height;
	width = my.width;
	channel = my.channel;
	realImg = new float**[height];
	iiImg = new float**[height];
	for (int i=0;i<height;i++){
		realImg[i] = new float*[width];
		iiImg[i] = new float*[width];
		for (int j=0;j<width;j++){
			realImg[i][j] = new float[channel];
			iiImg[i][j] = new float[channel];
			for (int k=0;k<channel;k++){
				realImg[i][j][k] = my.get(i,j,k);
				iiImg[i][j][k] = 0;
			}
		}
	}
}

singleImg::singleImg(int h,int w,int c){
	height = h;
	width = w;
	channel = c;
	realImg = new float**[height];
	iiImg = new float**[height];
	for (int i=0;i<height;i++){
		realImg[i] = new float*[width];
		iiImg[i] = new float*[width];
		for (int j=0;j<width;j++){
			realImg[i][j] = new float[channel];
			iiImg[i][j] = new float[channel];
			for (int k=0;k<channel;k++){
				realImg[i][j][k] =0;
				iiImg[i][j][k] = 0;
			}
		}
	}
}



void singleImg::starmulti(singleImg ori,double*** gf,int n){
	for(int i=0;i<height;i++)
		for (int j=0;j<width;j++)
			for (int k=0;k<channel;k++){
				realImg[i][j][k]=ori.get(i,j,k)*gf[i][j][n];
				iiImg[i][j][k]=ori.geti(i,j,k)*gf[i][j][n];
			}
}




void singleImg::xfft(){
	fftw_complex *in, *out;
    fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width);
	p = fftw_plan_dft_1d(width, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int k=0;k<channel;k++)
		for (int i=0;i<height;i++){		
			
			for(int j=0;j<width;j++){
				in[j][0] = realImg[i][j][k];
				in[j][1] = iiImg[i][j][k];
			}

			fftw_execute(p); /* repeat as needed */			

			for(int j=0;j<width;j++){
				realImg[i][j][k] = out[j][0];
				iiImg[i][j][k] = out[j][1];
			}
		}
	
    fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
}
void singleImg::yfft(){
	fftw_complex *in, *out;
    fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height);
	p = fftw_plan_dft_1d(height, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int k=0;k<channel;k++)
		for (int j=0;j<width;j++){		
			
			for(int i=0;i<height;i++){
				in[i][0] = realImg[i][j][k];
				in[i][1] = iiImg[i][j][k];
			}

			fftw_execute(p); /* repeat as needed */

			for(int i=0;i<height;i++){
				realImg[i][j][k] =out[i][0];
				iiImg[i][j][k] = out[i][1];
			}
		}
		
    fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
}
void singleImg::fft2(){
	xfft();
	yfft();
}
void singleImg::xifft(){
	fftw_complex *in, *out;
    fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width);
	p = fftw_plan_dft_1d(width, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	for (int k=0;k<channel;k++)
		for (int i=0;i<height;i++){		
			
			for(int j=0;j<width;j++){
				in[j][0] = realImg[i][j][k];
				in[j][1] = iiImg[i][j][k];
			}

			fftw_execute(p); /* repeat as needed */			

			for(int j=0;j<width;j++){
				realImg[i][j][k] = out[j][0]/width;
				iiImg[i][j][k] = out[j][1]/width;
			}
		}
	
    fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
}
void singleImg::yifft(){
	fftw_complex *in, *out;
    fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height);
	p = fftw_plan_dft_1d(height, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	for (int k=0;k<channel;k++)
		for (int j=0;j<width;j++){		
			
			for(int i=0;i<height;i++){
				in[i][0] = realImg[i][j][k];
				in[i][1] = iiImg[i][j][k];
			}

			fftw_execute(p); /* repeat as needed */

			for(int i=0;i<height;i++){
				realImg[i][j][k] = out[i][0]/height;
				iiImg[i][j][k] = out[i][1]/height;
			}
		}
		
    fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
}

void singleImg::ifft2(){ 
	xifft();
	yifft();
}

void singleImg::abs(){
	for (int i=0;i<height;i++)
		for (int j=0;j<width;j++)
			for (int k=0;k<channel;k++){
				realImg[i][j][k] = sqrt(pow(realImg[i][j][k],2)+pow(iiImg[i][j][k],2));
				iiImg[i][j][k] = 0;
			}
}

float singleImg::get(int i, int j, int k){
	return realImg[i][j][k];
}

float singleImg::geti(int i,int j,int k){
	return iiImg[i][j][k];
}