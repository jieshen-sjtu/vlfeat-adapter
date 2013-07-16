/*
 * dsift.hpp
 *
 *  Created on: May 24, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/dsift.h>

#ifdef __cplusplus
}
#endif

#include "basic-adapter.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using cv::Mat;
using cv::Point;
using std::vector;
using std::string;

namespace jieshen
{
    class DSIFT_Frame
    {
    public:
        DSIFT_Frame()
                : x(0), y(0), scale(0), norm(0)
        {
        }

        void clear()
        {
            x = 0;
            y = 0;
            scale = 0;
            norm = 0;
            descriptor.clear();
        }

    public:
        int x;
        int y;
        float scale;
        float norm;
        vector<float> descriptor;
    };

    class DSIFT_ADAPTER: public BASIC_ADAPTER
    {
#define __DSIFT_ADAPTER_EPS 0.0001
#define STEP_ARRAY_SIZE 2
#define BIN_SIZE_ARRAY_SIZE 2
#define REGION_ARRAY_SIZE 4
        enum
        {
            DEFAULT_NUM_BINX = 4,
            DEFAULT_NUM_BINY = 4,
            DEFAULT_NUM_BINT = 8,
            DEFAULT_WIN_SIZE = 2,
            DEFAULT_FLAT_WINDOW = 0,
            DEFAULT_MAGNIF = 3,

            DEFAULT_WIN_SIZE_INVALID = 0
        };

        static const int DEFAULT_REGION[REGION_ARRAY_SIZE];
        static const int DEFAULT_STEP[STEP_ARRAY_SIZE];
        static const int DEFAULT_STEP_INVALID[STEP_ARRAY_SIZE];
        static const int DEFAULT_BIN_SIZE[BIN_SIZE_ARRAY_SIZE];
        static const int DEFAULT_BIN_SIZE_INVALID[BIN_SIZE_ARRAY_SIZE];

    public:
        DSIFT_ADAPTER();
        DSIFT_ADAPTER(const Mat* img);
        ~DSIFT_ADAPTER();
    public:
        void setImage(const Mat* img);
        void setStep(const int* step);
        void setRegion(const int* region);
        void setBinSize(const int* bin_sz);
        void setWindowSize(const double win_sz);
        void setUseFlatWindow(bool use);

        void resetStep();
        void resetRegion();
        void resetBinSize();
        void resetWindowSize();
        void resetUseFlatWindow();

        void getStep(vector<int>& step) const;
        void getRegion(vector<int>& region) const;
        void getBinSize(vector<int>& bin_sz) const;
        double getWindowSize() const;
        bool getWindowType() const;

        void clearImage();
        void resetDSiftModel();

        string info() const;
        void clear();

    public:
        void extractDSiftFeature();
        int getNumKeyPoints() const;
        int getDSiftFeatureDim() const;
        const vector<DSIFT_Frame>& getAllFrames() const;

    private:
        void init();
        void init_dsift_model();
        void init_dsift_parameters();
        void clear_dsift_model();
        void clear_model_related_data();

        void set_dsift_model();
        void set_gray_image_data();

    private:
        VlDsiftFilter* m_dsift_model;
        int m_step[STEP_ARRAY_SIZE];
        int m_region[REGION_ARRAY_SIZE];
        int m_bin_size[BIN_SIZE_ARRAY_SIZE];
        double m_win_size;
        bool m_use_flat_win;

        bool m_has_extracted;

        vector<DSIFT_Frame> m_frames;
    };

    static bool _is_same_array(const int* a, const int* b, const int sz);
}

#endif /* __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__ */
