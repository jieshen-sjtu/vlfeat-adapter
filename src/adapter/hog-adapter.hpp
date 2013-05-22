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

#include "basic-adapter.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using cv::Mat;
using cv::Rect;
using std::vector;
using std::string;

namespace jieshen
{
    class HOG_ADAPTER: public BASIC_ADAPTER
    {
        enum
        {
            DEFAULT_NUM_ORT = 9,
            DEFAULT_CELLSIZE = 8,
            DEFAULT_NUM_ORT_INVALID = 0,
            DEFAULT_CELLSIZE_INVALID = 0
        };
        static const VlHogVariant DEFAULT_HOG_TYPE;

    public:
        HOG_ADAPTER();
        HOG_ADAPTER(const Mat* img);
        ~HOG_ADAPTER();

    public:
        // settings
        void setImage(const Mat* img);
        void setHOGType(const VlHogVariant type = DEFAULT_HOG_TYPE);
        void setNumOrient(const int ort = DEFAULT_NUM_ORT);
        void setCellSize(const int cellsz = DEFAULT_CELLSIZE);

        // derived, should be overwritten
        void clear();
        string info() const;

        // basic info
        //const Mat getImage() const;
        VlHogVariant getHOGType() const;
        int getNumOrient() const;
        int getCellSize() const;

        vl_size getHOGWidth() const;
        vl_size getHOGHeight() const;
        vl_size getHOGCellDim() const;
        vl_size getHOGFeatureDim() const;

        vl_size getHOGImageWidth() const;
        vl_size getHOGImageHeight() const;
        vl_size getHOGImageSize() const;

        // data info
        const float* getHOGFeature() const;
        const float* getHOGFeatureFlip() const;
        const float* getHOGImage() const;
        const float* getHOGImageFlip() const;

        // computation
        void extractFeature(vector<float>* descriptors = NULL);
        void extractPatchFeature(const Rect* region, vector<float>* descriptors,
                                 Mat* hog_img = NULL);
        void extractFeatureFlip(vector<float>* descriptors = NULL);
        void extractPatchFeatureFlip(const Rect* region,
                                     vector<float>* descriptors,
                                     Mat* hog_img = NULL);

        // visualization
        void visualizeFeature(Mat* hog_img = NULL);
        void visualizeFeatureFlip(Mat* hog_img_flip = NULL);

    private:
        // data management, derived, should be overwritten
        void init();
        /*
         void init_image_data();
         void set_image_data(const Mat* img);
         void clear_image_data();*/

        // HOG specific
        void init_hog_model();
        void clear_hog_model();
        void reset_hog_model();

        void clear_raw_memory_data();

        // auxiliary function
        void _visualize_feature_aux(const float* feature,
                                    float** hog_image_data, Mat* hog_image);
        bool _check_region(const Rect* region);
        void _extract_patch_feature_aux(const Rect* region,
                                        const float* feature,
                                        vector<float>* descriptors,
                                        const float* hog_img_data,
                                        Mat* hog_img);

    private:
        // image info
        /*
         bool m_has_set_image;
         Mat m_org_img;
         float* m_gray_data;
         int m_img_width;
         int m_img_height;*/



        // HOG setting
        VlHogVariant m_hog_type;
        int m_num_orient;
        int m_cell_size;

        // HOG data
        VlHog* m_hog_model;

        float* m_hog_feature;
        float* m_hog_img;
        bool m_has_extracted;

        // Flipped HOG data
        float* m_hog_feature_flip;
        float* m_hog_img_flip;
        bool m_has_extracted_flip;
    };
}

#endif /* __VLFEAT_ADAPTER_HOG_ADAPTER_HPP__ */
