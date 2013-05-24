/*
 * gist-adapter.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_GIST_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_GIST_ADAPTER_HPP__

#include "basic-adapter.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using std::vector;

#ifdef __cplusplus
extern "C"
{
#endif

#include <gist.h>

#ifdef __cplusplus
}
#endif

namespace jieshen
{
    class GIST_ADAPTER: public BASIC_ADAPTER
    {
        enum
        {
            DEFAULT_NBLOCK = 4,
            DEFAULT_NSCALE = 3,
            DEFAULT_NBLOCK_INVALID = 0,
            DEFAULT_NSCALE_INVALID = 0
        };
        static const int DEFAULT_ORT[3];
    public:
        GIST_ADAPTER();
        GIST_ADAPTER(const Mat* img);
        ~GIST_ADAPTER();

    public:
        void setImage(const Mat* img);
        void setNBlock(const int nblock);
        void setOrients(const int nort, const int* orts);

        void resetNBlock();
        void resetOrients();
        void resetGistModel();
        void clearImage();

        int getNBlock() const;
        int getNScale() const;
        const int* getNOrientsPerScale() const;

    public:
        int getGistFeatureDim() const;
        const float* getGistFeature() const;

    public:
        void clear();
        string info() const;
        void extractGistFeature(vector<float>* descriptor = NULL);

    private:
        void init();
        void init_gist_model();
        void init_gist_parameters();
        void clear_gist_model();
        void set_gist_model();
<<<<<<< Updated upstream
        void clear_raw_memory_data();
=======
        void clear_model_related_data();
>>>>>>> Stashed changes

    private:
        // Gist model
        int m_nblock;
        int m_nscale;
        int* m_orients;

        // Gist data
        float* m_gist_features;
        bool m_has_extracted;

        // image data
        //bool m_has_set_image;
        bool m_is_gray;
        image_t* m_gray_img;
        color_image_t* m_color_img;
    };
}

#endif /* __VLFEAT_ADAPTER_GIST_ADAPTER_HPP__ */
