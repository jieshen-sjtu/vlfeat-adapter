/*
 * test.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: jieshen
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <vl/generic.h>
#include <vl/hog.h>

#ifdef __cplusplus
}
#endif // __cplusplus
#include "test.hpp"

#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
using namespace cv;
using namespace std;

void test(int argc, char* argv[])
{
    Mat _img = imread(argv[1]);
    string winName = "test-hog";
    namedWindow(winName);

    Mat img = _img.rowRange(Range(0, 16));
    img = img.colRange(Range(img.cols - 32, img.cols - 16));
    //Mat img = _img;

    imshow(winName, img);
    waitKey(0);

    Mat hog_img;
    vector<float> desc;

    jieshen::HOG_ADAPTER hog;
    hog.setImage(&img);
    hog.extractFeature(&desc);
    cout << "feature size: " << desc.size() << endl;
    hog.visualizeFeature(&hog_img);
    imshow(winName, hog_img);
    waitKey(0);
    float a[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < desc.size(); ++i)
    {
        cout << desc[i] << " ";
        if ((i + 1) % 10 == 0)
            cout << endl;
        if (i / 31 == 0)
            a[0] += desc[i];
        else if (i / 31 == 1)
            a[1] += desc[i];
        else if (i / 31 == 2)
            a[2] += desc[i];
        else
            a[3] += desc[i];
    }
    cout << endl;
    //cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << endl;

    cout << "-----test basic done-----" << endl;

    img = _img.rowRange(Range(0, 32));
    img = img.colRange(Range(img.cols - 32, img.cols - 0));
    Rect region(0, 0, 16, 16);
    vector<float> patch_desc;
    Mat patch_img;
    hog.extractPatchFeature(&region, &patch_desc, &patch_img);
    for (int i = 0; i < patch_desc.size(); ++i)
    {
        cout << patch_desc[i] << " ";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    cout << endl;

    namedWindow("2");
    imshow("2", patch_img);
    waitKey(0);

    /*
     hog.setCellSize(12);
     hog.extractFeature(&desc);
     hog.visualizeFeature(&hog_img);
     imshow(winName, hog_img);
     waitKey(0);
     for (int i = 0; i < desc.size(); ++i)
     if (i % 100 == 0)
     cout << desc[i] << " ";
     cout << endl;
     cout << "-----test setCellSize() done-----" << endl;

     //hog.setCellSize(20);
     hog.setNumOrient(12);
     hog.extractFeature(&desc);
     hog.visualizeFeature(&hog_img);
     imshow(winName, hog_img);
     waitKey(0);
     for (int i = 0; i < desc.size(); ++i)
     if (i % 100 == 0)
     cout << desc[i] << " ";
     cout << endl;
     cout << "-----test setNumOrient() done-----" << endl;

     hog.setNumOrient(9);
     hog.setHOGType(VlHogVariantDalalTriggs);
     hog.extractFeature(&desc);
     hog.visualizeFeature(&hog_img);
     imshow(winName, hog_img);
     waitKey(0);
     for (int i = 0; i < desc.size(); ++i)
     if (i % 100 == 0)
     cout << desc[i] << " ";
     cout << endl;
     cout << "-----test setHOGType() done-----" << endl;

     //Mat img2 = imread(argv[2]);
     hog.setImage(&_img);
     hog.extractFeature(&desc);
     hog.visualizeFeature(&hog_img);
     imshow(winName, hog_img);
     waitKey(0);
     for (int i = 0; i < desc.size(); ++i)
     if (i % 100 == 0)
     cout << desc[i] << " ";
     cout << endl;
     cout << "-----test setImage() done-----" << endl;*/
}
