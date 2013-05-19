/*
 * sift-adapter.hpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_SIFT_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_SIFT_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/sift.h>

#ifdef __cplusplus
}
#endif

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using cv::Mat;
using cv::Rect;
using std::vector;
using std::string;

namespace jieshen
{
    class SIFT_ADAPTER
    {
        enum
        {
            DEFAULT_NOCTAVE = -1,
            DEFAULT_NLEVEL = 3,
            DEFAULT_OCT_FIRST = 0,
            DEFAULT_EDGE_THRD = 10,
            DEFAULT_PEAK_THRD = 0,
            DEFAULT_NORM_THRD = 0,
            DEFAULT_MAGNIF = 3,
            DEFAULT_WIN_SIZE = 2
        };
    public:
        SIFT_ADAPTER();
        SIFT_ADAPTER(const Mat* img);
        ~SIFT_ADAPTER();

    public:
        // settings
        void setImage(const Mat* img);
        void setNOctaves(const int noct);
        void setNLevels(const int nlevel);
        void setOctFirst(const int o_min);

        void setEdgeThrd(const double t);
        void setPeakThrd(const double t);
        void setNormThrd(const double t);
        void setMagnif(const double t);
        void setWindowSize(const double t);

        // derived
        void clear();
        string info() const;
        void setImageData(const Mat* img);

        // basic info
        const Mat getImage() const;
        int getNOctaves() const;
        int getNLevels() const;
        int getOctFirst() const;

        // computation
        void extractSiftFeature();

    private:
        // data management
        void init();
        void init_image_data();
        void clear_image_data();

        void init_sift_model();
        void clear_sift_model();
        void reset_sift_model();

    private:
        // image info
        Mat m_org_img;
        float* m_gray_data;
        int m_img_width;
        int m_img_height;

        // SIFT settings
        int m_noctave;
        int m_nlevel;
        int m_oct_first;

        // SIFT data
        VlSiftFilt* m_sift_model;
    };
}

#endif /* __VLFEAT_ADAPTER_SIFT_ADAPTER_HPP__ */
