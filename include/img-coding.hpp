/*
 * img-coding.hpp
 *
 *  Created on: 2012-7-20
 *      Author: jieshen
 */

#ifndef __COUPLING_IMG_CODING_HPP__
#define __COUPLING_IMG_CODING_HPP__

#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;

class ImageCoding
{
public:
    static void encode(const Mat& img, string& strEncode, const string ext =
            ".jpg");
    static void decode(const string& strDecode, Mat& img);
};

string Base64Encode(const uchar* Data, int DataByte);
string Base64Decode(const char* Data, int DataByte);

void __test_img_coding(int argc, char* argv[]);

#endif /* __COUPLING_IMG_CODING_HPP__ */
