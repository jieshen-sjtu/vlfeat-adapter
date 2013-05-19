/*
 * sift-adapter.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "sift-adapter.hpp"

namespace jieshen
{
    SIFT_ADAPTER::SIFT_ADAPTER()
    {
        init();
    }

    SIFT_ADAPTER::SIFT_ADAPTER(const Mat* img)
    {
        init();
        setImage(img);
    }

    SIFT_ADAPTER::~SIFT_ADAPTER()
    {
        clear();
    }

    void SIFT_ADAPTER::init()
    {
        init_image_data();
        init_sift_model();
    }

    void SIFT_ADAPTER::clear()
    {
        clear_image_data();
        clear_sift_model();
    }

    void SIFT_ADAPTER::init_image_data()
    {
        m_gray_data = NULL;
        m_img_width = 0;
        m_img_height = 0;
    }

    void SIFT_ADAPTER::clear_image_data()
    {
        if (m_gray_data)
            utils::myfree(&m_gray_data);

        m_img_width = 0;
        m_img_height = 0;
    }

    void SIFT_ADAPTER::init_sift_model()
    {
        m_noctave = DEFAULT_NOCTAVE;
        m_nlevel = DEFAULT_NLEVEL;
        m_oct_first = DEFAULT_OCT_FIRST;

        m_sift_model = vl_sift_new(m_img_width, m_img_height, m_noctave,
                                   m_nlevel, m_oct_first);
    }

    void SIFT_ADAPTER::clear_sift_model()
    {
        m_noctave = DEFAULT_NOCTAVE;
        m_nlevel = DEFAULT_NLEVEL;
        m_oct_first = DEFAULT_OCT_FIRST;

        if (m_sift_model)
        {
            vl_sift_delete(m_sift_model);
            m_sift_model = NULL;
        }
    }

    void SIFT_ADAPTER::reset_sift_model()
    {
        if (m_sift_model)
            vl_sift_delete(m_sift_model);

        m_sift_model = vl_sift_new(m_img_width, m_img_height, m_noctave,
                                   m_nlevel, m_oct_first);
    }

    void SIFT_ADAPTER::setImage(const Mat* img)
    {
        setImageData(img);
        //
    }

    void SIFT_ADAPTER::setNOctaves(const int noct)
    {
        assert(noct > 0);
        m_noctave = noct;
        reset_sift_model();
    }

    void SIFT_ADAPTER::setNLevels(const int nlevel)
    {
        assert(nlevel > 0);
        m_nlevel = nlevel;
        reset_sift_model();
    }

    void SIFT_ADAPTER::setOctFirst(const int o_min)
    {
        assert(o_min >= 0);
        m_oct_first = o_min;
        reset_sift_model();
    }

    void SIFT_ADAPTER::setEdgeThrd(const double t)
    {
        assert(t >= 1);
        vl_sift_set_edge_thresh(m_sift_model, t);
    }

    void SIFT_ADAPTER::setPeakThrd(const double t)
    {
        assert(t >= 0);
        vl_sift_set_peak_thresh(m_sift_model, t);
    }

    void SIFT_ADAPTER::setNormThrd(const double t)
    {
        assert(t >= 0);
        vl_sift_set_norm_thresh(m_sift_model, t);
    }

    void SIFT_ADAPTER::setMagnif(const double t)
    {
        assert(t >= 0);
        vl_sift_set_magnif(m_sift_model, t);
    }

    void SIFT_ADAPTER::setWindowSize(const double t)
    {
        assert(t >= 0);
        vl_sift_set_window_size(m_sift_model, t);
    }

    const Mat SIFT_ADAPTER::getImage() const
    {
        return m_org_img;
    }

    int SIFT_ADAPTER::getNOctaves() const
    {
        return m_noctave;
    }

    int SIFT_ADAPTER::getNLevels() const
    {
        return m_nlevel;
    }

    int SIFT_ADAPTER::getOctFirst() const
    {
        return m_oct_first;
    }

    void SIFT_ADAPTER::setImageData(const Mat* img)
    {
        img->copyTo(m_org_img);

        Mat gray_img;
        if (img->channels() == 3)
            cv::cvtColor(*img, gray_img, CV_BGR2GRAY);
        else
            gray_img = *img;

        gray_img.convertTo(gray_img, CV_32FC1);

        m_img_width = img->cols;
        m_img_height = img->rows;

        const int sz = m_img_width * m_img_height * sizeof(float);

        if (m_gray_data)
            free(m_gray_data);

        m_gray_data = (float*) utils::mymalloc(sz);

        memcpy(m_gray_data, gray_img.data, sz);
    }

    string SIFT_ADAPTER::info() const
    {
        string info = "=====SIFT settings=====\n";
        return info;
    }

    void SIFT_ADAPTER::extractSiftFeature()
    {

    }
}
