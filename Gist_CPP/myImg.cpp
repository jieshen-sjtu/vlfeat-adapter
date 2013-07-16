#include "myImg.h"

myImg::myImg(int h,int w){
	height = h;
	width = w;
	channel = 0;

	realImg = new double**[h];


	for (int i=0;i<height;i++){
		realImg[i] = new double*[w];
		for (int j=0;j<width;j++)
			realImg[i][j] = new double[3];
	}
	iiImg = NULL;
}

void myImg::logPadArraySym(IplImage* img,int w){
	double tmp;
	channel = img->nChannels;
	int hei = img->height;
	int wid = img->width;
	int cha = img->nChannels;


	for (int i=0;i<hei;i++){

		uchar* ptr = (uchar*) (img->imageData + i * img->widthStep);

		for (int j=0;j<wid;j++)
			for (int k=0;k<cha;k++){
				tmp = ptr[cha*j+k];
				tmp = log(tmp+1);
				realImg[i+w][j+w][cha-k-1] = tmp;
			}			
	}

	/* 4 coners */
	for (int i=0;i<w;i++)
		for (int j=0;j<w;j++)
			for (int k=0;k<cha;k++){
				realImg[i][j][k] = realImg[2*w-1-i][2*w-1-j][k];

				realImg[i][j+wid+w][k] = realImg[2*w-1-i][w+wid-1-j][k];

				realImg[i+hei+w][j][k] = realImg[w+hei-1-i][2*w-1-j][k];

				realImg[i+hei+w][j+wid+w][k] = realImg[w+hei-1-i][w+wid-1-j][k];
			}


	/* up and down */
	for (int i=0;i<w;i++)
		for (int j=w;j<w+wid;j++)
			for (int k=0;k<cha;k++){
				realImg[i][j][k] = realImg[2*w-1-i][j][k];
				realImg[i+w+hei][j][k] = realImg[w+hei-1-i][j][k];
			}

	/* left and right */
	for (int i=w;i<w+hei;i++)
		for (int j=0;j<w;j++)
			for (int k=0;k<cha;k++){
				realImg[i][j][k] = realImg[i][2*w-1-j][k];
				realImg[i][j+w+wid][k] = realImg[i][w+wid-1-j][k];
			}
}

void myImg::padSymPost(int x,int y){
	height +=x;
	width +=y;
	if ( x==0 && y==0) {return;}
	double ***anoImg = new double**[height];

	for (int i=0;i<height;i++){
		anoImg[i] = new double*[width];
		for (int j=0;j<width;j++){
			anoImg[i][j] = new double[3];
		}
	}

	/* initial*/
	for (int i=0;i<height-x;i++)
		for(int j=0;j<width-y;j++)
			for (int k=0;k<channel;k++)
				anoImg[i][j][k] = realImg[i][j][k];

	/* down */
	for (int i=0;i<x;i++)
		for(int j=0;j<width-y;j++)
			for (int k=0;k<channel;k++)
				anoImg[i+height-x][j][k] = anoImg[height-x-1-i][j][k];

	/* right */
	for (int i=0;i<height-x;i++)
		for(int j=0;j<y;j++)
			for (int k=0;k<channel;k++)
				anoImg[i][width-y+j][k] = anoImg[i][width-y-1-j][k];

	/* down right */
	for (int i=0;i<x;i++)
		for (int j=0;j<y;j++)
			for (int k=0;k<channel;k++)
				anoImg[height-x+i][width-y+j][k] = anoImg[height-x-1-i][width-y-1-j][k];

	/* release */
	for (int i=0;i<height-x;i++){
		for (int j=0;j<width-y;j++)
			delete[] realImg[i][j];
		delete[] realImg[i];
	}
	delete[] realImg;

	realImg = anoImg;
}



void myImg::xfft(){
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

void myImg::yfft(){
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

void myImg::fft2(){
	xfft();
	yfft();
}


void myImg::xifft(){
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

void myImg::yifft(){
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

void myImg::ifft2(){ 
	xifft();
	yifft();
}


void myImg::starmulti(double*** gf){
	for(int i=0;i<height;i++)
		for (int j=0;j<width;j++)
			for (int k=0;k<channel;k++){
				realImg[i][j][k]*=gf[i][j][k];
				iiImg[i][j][k]*=gf[i][j][k];
			}
}

void myImg::getoutput(double*** gf){
	
	double*** oriImg = new double**[height];

	for (int i=0;i<height;i++){
		oriImg[i] = new double*[width];
		for (int j=0;j<width;j++){
			oriImg[i][j] = new double[channel];
			for(int k=0;k<channel;k++)
				oriImg[i][j][k] = realImg[i][j][k];
		}
	}
	iiImg = new double**[height];

	for (int i=0;i<height;i++){
		iiImg[i] = new double*[width];

		for (int j=0;j<width;j++){
			iiImg[i][j] = new double[3];
			for (int k=0;k<3;k++)
				iiImg[i][j][k] = 0;
		}
	}

	fft2();
	starmulti(gf);
	ifft2();

	for (int i=0;i<height;i++)
		for (int j=0;j<width;j++)
			for(int k=0;k<channel;k++)
				realImg[i][j][k]= oriImg[i][j][k] - realImg[i][j][k] ;

	
	for (int i=0;i<height;i++){
		for (int j=0;j<width;j++){
			delete[] iiImg[i][j];
			delete[] oriImg[i][j];
		}
		delete[] iiImg[i];
		delete[] oriImg[i];
	}
	delete[] iiImg;
	delete[] oriImg;
	iiImg = NULL;
	oriImg = NULL;
}

void myImg::localstd(myImg newImg,double*** gf){

	channel = newImg.channel;
	
	iiImg = new double**[height];
	for (int i=0;i<height;i++){
		iiImg[i] = new double*[width];

		for (int j=0;j<width;j++){
			iiImg[i][j] = new double[3];
			for (int k=0;k<3;k++)
				iiImg[i][j][k] = 0;
		}
	}

	for (int i=0;i<height;i++)
		for(int j=0;j<width;j++){
			realImg[i][j][0] = 0;
			for (int k=0;k<newImg.channel;k++)
				realImg[i][j][0] +=newImg.get(i,j,k);

			realImg[i][j][0]/=newImg.channel;
			realImg[i][j][0] =realImg[i][j][0]*realImg[i][j][0];
		}

	fft2();
	starmulti(gf);
	ifft2();
	
	for (int i=0;i<height;i++)
		for(int j=0;j<width;j++){

			realImg[i][j][0] = sqrt(sqrt(pow(realImg[i][j][0],2)+pow(iiImg[i][j][0],2)));

			for (int k=1;k<newImg.channel;k++)
				realImg[i][j][k] = realImg[i][j][0];
		}

	for (int i=0;i<height;i++){
		for (int j=0;j<width;j++){
			delete[] iiImg[i][j];
		}
		delete[] iiImg[i];
	}
	delete[] iiImg;
	iiImg = NULL;
}

double myImg::get(int i,int j,int k){
	return realImg[i][j][k];
}

void myImg::div2trunc(myImg lstdImg,int w){
	for (int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			for (int k=0;k<channel;k++)
				realImg[i][j][k]/= (0.2+lstdImg.get(i,j,k));

	height = height - 2*w;
	width = width-2*w;
	double *** tmpImg = new double**[height];

	for (int i=0;i<height;i++){
		tmpImg[i] = new double*[width];
		for (int j=0;j<width;j++){
			tmpImg[i][j] = new double[channel];
			for(int k=0;k<channel;k++)
				tmpImg[i][j][k] = realImg[i+w][j+w][k];
		}
	}

	/* release */
	for (int i=0;i<height+2*w;i++){
		for (int j=0;j<width+2*w;j++)
			delete[] realImg[i][j];
		delete[] realImg[i];
	}
	delete[] realImg;

	realImg = tmpImg;
}

void myImg::destroy(){
	if (realImg != NULL){
		for (int i=0;i<height;i++){
			for (int j=0;j<width;j++)
				delete[] realImg[i][j];
			delete[] realImg[i];
		}
		delete[] realImg;
		realImg = NULL;
	}

	if (iiImg != NULL){
		for (int i=0;i<height;i++){
			for (int j=0;j<width;j++)
				delete[] iiImg[i][j];
			delete[] iiImg[i];
		}
		delete[] iiImg;
		iiImg = NULL;
	}
}

/* --------------useless-----------
inline void myImg::swap(double &a,double &b){
	double t;
    t = a;
    a = b;
    b = t;
}


void myImg::bitrp (double xreal [], double ximag [], int n){
    // 位反转置换 Bit-reversal Permutation
    int i, j, a, b, p;

    for (i = 1, p = 0; i < n; i *= 2)
        p ++;
    
    for (i = 0; i < n; i ++){
        a = i;
        b = 0;
        for (j = 0; j < p; j ++){
            b = (b << 1) + (a & 1);    // b = b * 2 + a % 2;
            a >>= 1;        // a = a / 2;
		}
        if ( b > i){
			swap (xreal [i], xreal [b]);
            swap (ximag [i], ximag [b]);
        }
	}
 }
void myImg::ifft(double xreal [], double ximag [], int n){
	double wreal [N / 2], wimag [N / 2], treal, timag, ureal, uimag, arg;
    int m, k, j, t, index1, index2;
    
    bitrp (xreal, ximag, n);

    // 计算 1 的前 n / 2 个 n 次方根 Wj = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
    arg = 2 * PI / n;
    treal = cos (arg);
    timag = sin (arg);
    wreal [0] = 1.0;
    wimag [0] = 0.0;
    for (j = 1; j < n / 2; j ++)
        {
        wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
        wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
        }

    for (m = 2; m <= n; m *= 2)
        {
        for (k = 0; k < n; k += m)
            {
            for (j = 0; j < m / 2; j ++)
                {
                index1 = k + j;
                index2 = index1 + m / 2;
                t = n * j / m;    // 旋转因子 w 的实部在 wreal [] 中的下标为 t
                treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
                timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
                ureal = xreal [index1];
                uimag = ximag [index1];
                xreal [index1] = ureal + treal;
                ximag [index1] = uimag + timag;
                xreal [index2] = ureal - treal;
                ximag [index2] = uimag - timag;
                }
            }
        }

    for (j=0; j < n; j ++)
        {
        xreal [j] /= n;
        ximag [j] /= n;
        }
}
void myImg::fft(double xreal [], double ximag [], int n){
	double wreal [N / 2], wimag [N / 2], treal, timag, ureal, uimag, arg;
    int m, k, j, t, index1, index2;
    
    bitrp(xreal, ximag, n);

    // 计算 1 的前 n / 2 个 n 次方根的共轭复数 W'j = wreal [j] + i * wimag [j] , j = 0, 1, ... , n / 2 - 1
    arg = - 2 * PI / n;
    treal = cos (arg);
    timag = sin (arg);
    wreal [0] = 1.0;
    wimag [0] = 0.0;
    for (j = 1; j < n / 2; j ++)
        {
        wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
        wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
        }

    for (m = 2; m <= n; m *= 2)
        {
        for (k = 0; k < n; k += m)
            {
            for (j = 0; j < m / 2; j ++)
                {
                index1 = k + j;
                index2 = index1 + m / 2;
                t = n * j / m;    // 旋转因子 w 的实部在 wreal [] 中的下标为 t
                treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
                timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
                ureal = xreal [index1];
                uimag = ximag [index1];
                xreal [index1] = ureal + treal;
                ximag [index1] = uimag + timag;
                xreal [index2] = ureal - treal;
                ximag [index2] = uimag - timag;
                }
            }
        }
}


*/
