/*
 * hog-adapter.hpp
 *
 *  Created on: Mar 27, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_HOG_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_HOG_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/hog.h>

#ifdef __cplusplus
}
#endif

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using std::vector;

namespace jieshen
{
    class HOG_ADAPTER
    {
        enum
        {
            DEFAULT_NUM_ORT = 9, DEFAULT_CELLSIZE = 8
        };
    public:
        HOG_ADAPTER();
        HOG_ADAPTER(const Mat* img);
        ~HOG_ADAPTER();

    public:
        // settings
        void setImage(const Mat* img);
        void setHOGType(const VlHogVariant type = VlHogVariantUoctti);
        void setNumOrient(const int ort = DEFAULT_NUM_ORT);
        void setCellSize(const int cellsz = DEFAULT_CELLSIZE);

        // basic info
        vl_size getHOGWidth() const;
        vl_size getHOGHeight() const;
        vl_size getHOGCellDim() const;
        vl_size getHOGTotalDim() const;

        vl_size getHOGImageWidth() const;
        vl_size getHOGImageHeight() const;

        // computation
        void extractFeature(vector<float>* descriptors = NULL);
        void extractPatchFeature(const Rect* region, vector<float>* descriptors,
                                 Mat* hog_img = NULL, bool bPrecise = false);

        // visualization
        void visualizeFeature(Mat* hog_img = NULL);

    private:
        void init();
        void init_image_info();
        void init_hog_model();
        void reset_hog_model();

    private:
        // image info
        Mat m_org_img;
        float* m_gray_data;
        int m_img_width;
        int m_img_height;

        // HOG setting
        VlHogVariant m_hog_type;
        int m_num_orient;
        int m_cell_size;

        VlHog* m_hog_model;
        float* m_hog_feature;
        float* m_hog_img;
        bool m_has_extracted;
    };
}

#endif /* __VLFEAT_ADAPTER_HOG_ADAPTER_HPP__ */
