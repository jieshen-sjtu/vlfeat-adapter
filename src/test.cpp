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

void test_hog(int argc, char* argv[])
{
    Mat _img = imread(argv[1]);
    string winName = "test-hog";
    namedWindow(winName);

    Mat img = _img.rowRange(Range(0, 64));
    img = img.colRange(Range(img.cols - 128, img.cols));
    //Mat img = _img;

    imshow(winName, img);
    waitKey(0);

    Mat hog_img;
    vector<float> desc;

    jieshen::HOG_ADAPTER hog;
    cout << hog.info() << endl;
    hog.setImage(&img);
    hog.extractFeature(&desc);
    cout << "feature size: " << desc.size() << endl;
    hog.visualizeFeature(&hog_img);
    namedWindow("basic-test");
    imshow("basic-test", hog_img);
    waitKey(0);

    vl_size dim = hog.getHOGCellDim();
    vl_size nx = hog.getHOGWidth();
    vl_size ny = hog.getHOGHeight();

    int t = 0;
    /*
     for (int d = 0; d < dim; ++d)
     {
     for (int y = 0; y < ny; ++y)
     {
     for (int x = 0; x < nx; ++x)
     {
     cout << desc[t] << " ";
     ++t;
     }
     cout << endl;
     }
     cout << endl;
     }
     */
    cout << endl;

    cout << "-----test basic done-----" << endl;

    hog.extractFeatureFlip(&desc);
    hog.visualizeFeatureFlip(&hog_img);
    //cv::flip(hog_img, hog_img, 1);
    //imshow(winName, hog_img);
    namedWindow("flip-test");
    imshow("flip-test", hog_img);
    waitKey(0);
    t = 0;
    /*
     for (int d = 0; d < dim; ++d)
     {
     for (int y = 0; y < ny; ++y)
     {
     for (int x = 0; x < nx; ++x)
     {
     cout << desc[t] << " ";
     ++t;
     }
     cout << endl;
     }
     cout << endl;
     }
     */
    cout << endl;
    cout << "-----test flip done-----" << endl;

    /*
     img = _img.rowRange(Range(0, 64));
     img = img.colRange(Range(img.cols - 64, img.cols - 0));*/
    Rect region(64, 0, 64, 64);
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

    namedWindow("patch-test");
    imshow("patch-test", patch_img);
    waitKey(0);

    cout << endl << "-----test patch done-----" << endl;


    hog.setCellSize(12);
    hog.extractFeature(&desc);
    hog.visualizeFeature(&hog_img);
    namedWindow("cell-test");
    imshow("cell-test", hog_img);
    waitKey(0);

    cout << "-----test setCellSize() done-----" << endl;

    hog.setCellSize(8);
    hog.setNumOrient(12);
    hog.extractFeature(&desc);
    hog.visualizeFeature(&hog_img);
    namedWindow("ort-test");
    imshow("ort-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setNumOrient() done-----" << endl;

    hog.setNumOrient(9);
    hog.setHOGType(VlHogVariantDalalTriggs);
    hog.extractFeature(&desc);
    hog.visualizeFeature(&hog_img);
    namedWindow("type-test");
    imshow("type-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setHOGType() done-----" << endl;

    hog.setHOGType(VlHogVariantUoctti);
    hog.setImage(&_img);
    hog.extractFeature(&desc);
    hog.visualizeFeature(&hog_img);
    namedWindow("image-test");
    imshow("image-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setImage() done-----" << endl;

    destroyAllWindows();
}

void test_sift(int argc, char* argv[])
{

}
