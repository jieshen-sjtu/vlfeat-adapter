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
#include "adapter/hog-adapter.hpp"
#include "adapter/sift-adapter.hpp"
#include "adapter/gist-adapter.hpp"
#include "adapter/dsift-adapter.hpp"

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
    hog.extractHOGFeature(&desc);

    cout << "feature size: " << desc.size() << endl;
    hog.visualizeHOGFeature(&hog_img);
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

    hog.extractHOGFeatureFlip(&desc);
    hog.visualizeHOGFeatureFlip(&hog_img);
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
    hog.extractHOGPatchFeature(&region, &patch_desc, &patch_img);
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
    hog.extractHOGFeature(&desc);
    hog.visualizeHOGFeature(&hog_img);
    namedWindow("cell-test");
    imshow("cell-test", hog_img);
    waitKey(0);

    cout << "-----test setCellSize() done-----" << endl;

    hog.setCellSize(8);
    hog.setNumOrient(12);
    hog.extractHOGFeature(&desc);
    hog.visualizeHOGFeature(&hog_img);
    namedWindow("ort-test");
    imshow("ort-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setNumOrient() done-----" << endl;

    hog.setNumOrient(9);
    hog.setHOGType(VlHogVariantDalalTriggs);
    hog.extractHOGFeature(&desc);
    hog.visualizeHOGFeature(&hog_img);
    namedWindow("type-test");
    imshow("type-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setHOGType() done-----" << endl;

    hog.setHOGType(VlHogVariantUoctti);
    hog.setImage(&_img);
    hog.extractHOGFeature(&desc);
    hog.visualizeHOGFeature(&hog_img);
    namedWindow("image-test");
    imshow("image-test", hog_img);
    waitKey(0);

    cout << endl;
    cout << "-----test setImage() done-----" << endl;

    destroyAllWindows();
}

void test_sift(int argc, char* argv[])
{
    jieshen::SIFT_ADAPTER sift_model;
    Mat img = imread(argv[1]);
    sift_model.setImage(&img);

    sift_model.extractSiftFeature();
    const vector<jieshen::SIFT_Frame>& all_frames = sift_model.getAllFrames();

    cout << all_frames.size() << endl;

    for (size_t i = 0; i < all_frames.size(); ++i)
    {
        cv::circle(img, Point(all_frames[i].x, all_frames[i].y), 3,
                   Scalar(0, 0, 255), 2);
    }

    Mat sift_img;

    namedWindow("basic-test");
    imshow("basic-test", img);
    waitKey(0);

    sift_model.visualizeSiftFeature(&sift_img);
    cout << sift_model.info() << endl;
    namedWindow("basic-sift");
    imshow("basic-sift", sift_img);
    waitKey(0);

    cout << "basic test done" << endl;

    sift_model.setNOctaves(8);
    sift_model.extractSiftFeature();
    sift_model.visualizeSiftFeature(&sift_img);
    cout << sift_model.info() << endl;
    namedWindow("oct-sift");
    imshow("oct-sift", sift_img);
    waitKey(0);

    cout << "set oct test done" << endl;

    sift_model.resetNOctaves();
    sift_model.setNLevels(5);
    sift_model.extractSiftFeature();
    sift_model.visualizeSiftFeature(&sift_img);
    cout << sift_model.info() << endl;
    namedWindow("nlevel-sift");
    imshow("nlevel-sift", sift_img);
    waitKey(0);

    cout << "set nlevel test done" << endl;

    sift_model.setOctFirst(2);
    sift_model.extractSiftFeature();
    sift_model.visualizeSiftFeature(&sift_img);
    cout << sift_model.info() << endl;
    namedWindow("oct-first-sift");
    imshow("oct-first-sift", sift_img);
    waitKey(0);

    cout << "set oct-first test done" << endl;

    sift_model.setEdgeThrd(50);
    sift_model.extractSiftFeature();
    sift_model.visualizeSiftFeature(&sift_img);
    cout << sift_model.info() << endl;
    namedWindow("edge-thrd-sift");
    imshow("edge-thrd-sift", sift_img);
    waitKey(0);

    cout << "set edge-thrd test done" << endl;

    destroyAllWindows();
}

void test_gist(int argc, char* argv[])
{
    Mat img = imread(argv[1]);

    jieshen::GIST_ADAPTER gist_model;
    cout << gist_model.info() << endl;
    gist_model.setImage(&img);
    vector<float> descriptor;
    cout << "start extract" << endl;
    gist_model.extractGistFeature(&descriptor);

    cout << gist_model.info() << endl;

    cout << descriptor.size() << endl;
/*
    for (size_t i = 0; i < descriptor.size(); ++i)
    {
        cout << descriptor[i] << " ";
        if ((i + 1) % 20 == 0)
            cout << endl;
    }*/
}

void test_dsift(int argc, char* argv[])
{
    Mat img = imread(argv[1]);
    jieshen::DSIFT_ADAPTER dsift_model;
    dsift_model.setImage(&img);

    dsift_model.extractDSiftFeature();

    cout << dsift_model.info() << endl;
}
