/*
 * img-coding.cpp
 *
 *  Created on: 2012-7-20
 *      Author: jieshen
 */

#include "img-coding.hpp"
using namespace std;
using namespace cv;

void ImageCoding::encode(const Mat& img, string& strEncode, const string ext)
{
    vector<uchar> v_buf;
    imencode(ext, img, v_buf);
    const size_t buf_len = v_buf.size();
    uchar* buf = (uchar*) malloc(sizeof(uchar) * buf_len);
    copy(v_buf.begin(), v_buf.end(), buf);
    strEncode = Base64Encode(buf, buf_len);
    free(buf);
}

void ImageCoding::decode(const string& strDecode, Mat& img)
{
    string buf = Base64Decode(strDecode.c_str(), strDecode.length());

    vector<uchar> v_buf(buf.begin(), buf.end());

    img = imdecode(v_buf, 1);

}

string Base64Encode(const uchar* Data, int DataByte)
{
    const char EncodeTable[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //
    string strEncode;
    unsigned char Tmp[4] = { 0 };
    //int LineLength = 0;
    for (int i = 0; i < (int) (DataByte / 3); i++)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        Tmp[3] = *Data++;
        strEncode += EncodeTable[Tmp[1] >> 2];
        strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode += EncodeTable[Tmp[3] & 0x3F];
        //if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
    }
    //
    int Mod = DataByte % 3;
    if (Mod == 1)
    {
        Tmp[1] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode += "==";
    }
    else if (Mod == 2)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode +=
                EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode += "=";
    }

    return strEncode;
}

string Base64Decode(const char* Data, int DataByte)
{
    //
    const char DecodeTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0,
            0,
            62, // '+'
            0, 0,
            0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59,
            60,
            61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
            14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
            38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
            };
    //
    string strDecode;
    int nValue;
    int i = 0;
    while (i < DataByte)
    {
        if (*Data != '\r' && *Data != '\n')
        {
            nValue = DecodeTable[(int)*Data++] << 18;
            nValue += DecodeTable[(int)*Data++] << 12;
            strDecode += (nValue & 0x00FF0000) >> 16;
            if (*Data != '=')
            {
                nValue += DecodeTable[(int)*Data++] << 6;
                strDecode += (nValue & 0x0000FF00) >> 8;
                if (*Data != '=')
                {
                    nValue += DecodeTable[(int)*Data++];
                    strDecode += nValue & 0x000000FF;
                }
            }
            i += 4;
        }
        else //
        {
            Data++;
            i++;
        }
    }
    return strDecode;
}

void __test_img_coding(int argc, char* argv[])
{
    CV_Assert(argc==2);

    Mat img = imread(argv[1]);
    CV_Assert(img.data);

    string buf;
    ImageCoding::encode(img, buf);
    Mat res;
    ImageCoding::decode(buf, res);
    const string winName = "__test_img_coding";
    namedWindow(winName);
    imshow(winName, res);
    waitKey(0);
    destroyWindow(winName);
}
