/*
 * basic-adapter.hpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_BASIC_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_BASIC_ADAPTER_HPP__

#include <opencv2/opencv.hpp>
#include <string>

using cv::Mat;
using std::string;

namespace jieshen
{
    class BASIC_ADAPTER
    {
    public:
        BASIC_ADAPTER();
        BASIC_ADAPTER(const Mat* img);
        virtual ~BASIC_ADAPTER();

    public:
        virtual void setImage(const Mat* img) = 0;
        virtual void clear();
        virtual string info() const = 0;

    protected:
        virtual void init();

    public:
        const Mat getImage() const;

    protected:
        void init_image_data();
        void set_image_data(const Mat* img);
        void clear_image_data();

    protected:
        Mat m_org_img;
        float* m_gray_data;
        int m_img_width;
        int m_img_height;
    };
}

#endif /* __VLFEAT_ADAPTER_BASIC_ADAPTER_HPP__ */
