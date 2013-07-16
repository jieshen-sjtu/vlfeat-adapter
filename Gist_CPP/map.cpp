#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include "createGabor.h"
#include "prefilt.h"
#include "gistGabor.h"
#include "base64encoder.h"

int main(){
	
	int landmarkId;
	string imageNameMd5,imageEncoded,imageData;

	double ***G=NULL;
	double **g = NULL;
	IplImage * image = NULL;
	CvMat * encodedMat = NULL;
	//IplImage* image =cvLoadImage("test.jpg");
	// This is used for computing
	int imageSize = 128;
	IplImage *img = NULL;
	int numberBlocks = 4;
	int orientationsPerScale[]= {8,8,4};
	createGabor(G,orientationsPerScale,imageSize);
	
	while (cin>>landmarkId>>imageNameMd5>>imageEncoded){

		try {
			imageData.assign(Base64Decode(imageEncoded.c_str(),imageEncoded.length()));
			encodedMat = cvCreateMat(1,imageData.length(),CV_8UC1);
			encodedMat->data.ptr = (uchar *)imageData.c_str();
			image = cvDecodeImage(encodedMat);
		}
		catch(...){
			cvReleaseImage(&image);
			cvReleaseMat(&encodedMat);
		}

		if (!image){
			cvReleaseImage(&image);
			cvReleaseMat(&encodedMat);
			continue;
		}
	
		//-----------------resize the pic to 128*128--------

		CvSize sz;
		sz.height = imageSize; //128
		sz.width = imageSize;
		img = cvCreateImage(sz,image->depth,image->nChannels);

		cvResize(image,img,CV_INTER_CUBIC);

		//-----------------resize the pic to 128*128--------


		if (!img) {
			cvReleaseImage(&image);
			cvReleaseMat(&encodedMat);
			cvReleaseImage(&img);
			//continue;
		}

		myImg output = prefilt(img,4);

		gistGabor(output,numberBlocks,G,imageSize,ssum(orientationsPerScale),g);
		
		cout<<landmarkId<<"L"<<imageNameMd5<<"\t";
		cout<<sizeof(g);
		for (int i=0;i<960;i++)
			cout<<setprecision(4)<<g[i][0]<<'X';

		cvReleaseImage(&image);
		cvReleaseMat(&encodedMat);
		cvReleaseImage(&img);
		output.destroy();

		cout<<endl;
	}
	return 0;
}
