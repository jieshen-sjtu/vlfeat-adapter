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

#include "basic-adapter.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using cv::Mat;
using cv::Rect;
using cv::Point;
using std::vector;
using std::string;

namespace jieshen
{
#define DEFAULT_SIFT_DIM 128

    class SIFT_Frame
    {
    public:
        int x;
        int y;
        float scale;
        float angle;
        vector<float> descriptor;

        SIFT_Frame()
                : x(0), y(0), scale(0), angle(0)
        {
            descriptor.resize(DEFAULT_SIFT_DIM, 0);
        }

        void clear()
        {
            x = 0;
            y = 0;
            scale = 0;
            angle = 0;
            descriptor.clear();
        }
    };

    class SIFT_ADAPTER: public BASIC_ADAPTER
    {
#define __SIFT_ADAPTER_EPS 0.0001
        enum
        {
            DEFAULT_NOCTAVE = -1,
            DEFAULT_NLEVEL = 3,
            DEFAULT_OCT_FIRST = 0,
            DEFAULT_EDGE_THRD = 10,
            DEFAULT_PEAK_THRD = 0,
            DEFAULT_NORM_THRD = 0,
            DEFAULT_MAGNIF = 3,
            DEFAULT_WIN_SIZE = 2,

            DEFAULT_NOCTAVE_INVALID = 0,
            DEFAULT_NLEVEL_INVALID = 0,
            DEFAULT_OCT_FIRST_INVALID = -1,
            DEFAULT_EDGE_THRD_INVALID = -1,
            DEFAULT_PEAK_THRD_INVALID = -1,
            DEFAULT_NORM_THRD_INVALID = -1,
            DEFAULT_MAGNIF_INVALID = -1,
            DEFAULT_WIN_SIZE_INVALID = -1
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

        // set back to default
        void resetNOctaves();
        void resetNLevels();
        void resetOctFirst();

        void resetEdgeThrd();
        void resetPeakThrd();
        void resetNormThrd();
        void resetMagnif();
        void resetWindowSize();

        void resetSiftModel();
        void clearImage();

        // derived, should be overwritten
        void clear();
        string info() const;

        // basic info
        // const Mat getImage() const;
        int getNOctaves() const;
        int getNLevels() const;
        int getOctFirst() const;

        double getEdgeThrd() const;
        double getPeakThrd() const;
        double getNormThrd() const;
        double getMagnif() const;
        double getWindowSize() const;

        const vector<SIFT_Frame>& getAllFrames() const;
        const Mat getSiftImage() const;

        // computation
        void extractSiftFeature();
        void visualizeSiftFeature(Mat* sift_img = NULL);

    private:
        // data management
        void init();
        /*
         void init_image_data();
         void clear_image_data();
         void set_image_data(const Mat* img);
         */

        void init_sift_model();
        void init_sift_parameters();
        void clear_sift_model();
        void clear_raw_memory_data();
        void set_sift_model();

        bool _same_val(double a, double b) const;

    private:
        // image info, derived
        /*
         Mat m_org_img;
         float* m_gray_data;
         int m_img_width;
         int m_img_height;*/

        Mat m_sift_img;

        // SIFT settings
        int m_noctave;
        int m_nlevel;
        int m_oct_first;
        double m_edge_thrd;
        double m_peak_thrd;
        double m_norm_thrd;
        double m_magnif;
        double m_window_sz;

        // SIFT data
        VlSiftFilt* m_sift_model;
        bool m_has_extracted;

        // raw memory data
        vector<SIFT_Frame> m_frames;
        int m_num_frames;
    };
}

#endif /* __VLFEAT_ADAPTER_SIFT_ADAPTER_HPP__ */
