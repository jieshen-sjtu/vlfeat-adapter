/*
 * lbp-adapter.hpp
 *
 *  Created on: May 27, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_LBP_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_LBP_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/lbp.h>

#ifdef __cplusplus
}
#endif

#include "basic-adapter.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using cv::Rect;
using std::vector;

namespace jieshen
{
    class LBP_ADAPTER: public BASIC_ADAPTER
    {
        enum
        {
            DEFAULT_CELLSIZE = 16, DEFAULT_CELLSIZE_INVALID = 0
        };
    public:
        LBP_ADAPTER();
        LBP_ADAPTER(const Mat* img);
        ~LBP_ADAPTER();

    public:
        void setImage(const Mat* img);
        void setCellSize(const int cellsz);

        void resetCellSize();

        int getCellSize() const;
        int getLbpXDim() const;
        int getLbpYDim() const;
        int getLbpCellDim() const;
        int getLbpFeatureDim() const;

        void clearImage();
        void resetLbpModel();

        string info() const;
        void clear();

    public:
        void extractLbpFeature(vector<float>* descriptors = NULL);
        void extractLbpPatchFeature(const Rect* region,
                                    vector<float>* descriptors);
        const float* getLbpFeature() const;

    private:
        void init();
        void init_lbp_model();
        void init_lbp_parameters();
        void clear_lbp_model();
        void clear_model_related_data();

        void set_lbp_model();

    private:
        VlLbp* m_lbp_model;
        float* m_lbp_features;
        bool m_has_extracted;

        int m_cell_size;
    };
}

#endif /* __VLFEAT_ADAPTER_LBP_ADAPTER_HPP__ */
