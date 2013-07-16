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
        virtual void clearImage() = 0;
        virtual void clear() = 0;
        virtual string info() const = 0;

    protected:
        virtual void init();
        virtual void clear_model_related_data() = 0;

    public:
        const Mat getImage() const;

    protected:
        void init_image_data();
        virtual void set_gray_image_data();
        void clear_gray_image_data();
        void check_image() const;

    protected:
        bool m_has_set_image;
        Mat m_org_img;
        float* m_gray_data;
    };
}

#endif /* __VLFEAT_ADAPTER_BASIC_ADAPTER_HPP__ */
