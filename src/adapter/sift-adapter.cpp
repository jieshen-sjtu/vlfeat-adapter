/*
 * sift-adapter.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "sift-adapter.hpp"
#include "../utils.hpp"

#include <iostream>

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

        m_sift_model = NULL;
        m_has_extracted = false;

        m_num_frames = 0;
        m_frames.reserve(1024);

    }

    void SIFT_ADAPTER::set_sift_model()
    {
        m_sift_model = vl_sift_new(m_img_width, m_img_height, m_noctave,
                                   m_nlevel, m_oct_first);

        m_has_extracted = false;

        m_noctave = getNOctaves();
        m_nlevel = getNLevels();
        m_oct_first = getOctFirst();

        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::clear_sift_model()
    {
        if (m_sift_model)
        {
            vl_sift_delete(m_sift_model);
            m_sift_model = NULL;
        }

        m_has_extracted = false;

        m_noctave = DEFAULT_NOCTAVE;
        m_nlevel = DEFAULT_NLEVEL;
        m_oct_first = DEFAULT_OCT_FIRST;

        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::clear_raw_memory_data()
    {
        m_num_frames = 0;

        for (size_t i = 0; i < m_frames.size(); ++i)
            m_frames[i].clear();
        m_frames.clear();
    }

    void SIFT_ADAPTER::reset_sift_model()
    {
        if (m_sift_model)
            vl_sift_delete(m_sift_model);

        m_sift_model = vl_sift_new(m_img_width, m_img_height, m_noctave,
                                   m_nlevel, m_oct_first);

        m_has_extracted = false;

        m_noctave = getNOctaves();
        m_nlevel = getNLevels();
        m_oct_first = getOctFirst();

        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::setImage(const Mat* img)
    {
        set_image_data(img);
        reset_sift_model();
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
        m_has_extracted = false;
        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::setPeakThrd(const double t)
    {
        assert(t >= 0);
        vl_sift_set_peak_thresh(m_sift_model, t);
        m_has_extracted = false;
        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::setNormThrd(const double t)
    {
        assert(t >= 0);
        vl_sift_set_norm_thresh(m_sift_model, t);
        m_has_extracted = false;
        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::setMagnif(const double t)
    {
        assert(t >= 0);
        vl_sift_set_magnif(m_sift_model, t);
        m_has_extracted = false;
        clear_raw_memory_data();
    }

    void SIFT_ADAPTER::setWindowSize(const double t)
    {
        assert(t >= 0);
        vl_sift_set_window_size(m_sift_model, t);
        m_has_extracted = false;
        clear_raw_memory_data();
    }

    const Mat SIFT_ADAPTER::getImage() const
    {
        return m_org_img;
    }

    int SIFT_ADAPTER::getNOctaves() const
    {
        return vl_sift_get_noctaves(m_sift_model);
    }

    int SIFT_ADAPTER::getNLevels() const
    {
        return vl_sift_get_nlevels(m_sift_model);
    }

    int SIFT_ADAPTER::getOctFirst() const
    {
        return vl_sift_get_octave_first(m_sift_model);
    }

    double SIFT_ADAPTER::getEdgeThrd() const
    {
        return vl_sift_get_edge_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getPeakThrd() const
    {
        return vl_sift_get_peak_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getNormThrd() const
    {
        return vl_sift_get_norm_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getMagnif() const
    {
        return vl_sift_get_magnif(m_sift_model);
    }

    double SIFT_ADAPTER::getWindowSize() const
    {
        return vl_sift_get_window_size(m_sift_model);
    }

    const vector<SIFT_Frame>& SIFT_ADAPTER::getAllFrames() const
    {
        return m_frames;
    }

    void SIFT_ADAPTER::set_image_data(const Mat* img)
    {
        img->copyTo(m_org_img);

        Mat gray_img;
        if (img->channels() == 3)
            cv::cvtColor(*img, gray_img, CV_BGR2GRAY);
        else
            gray_img = img->clone();

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

        info += "NOctave: " + utils::myitoa(getNOctaves()) + "\n";

        info += "NLevel: " + utils::myitoa(getNLevels()) + "\n";

        info += "FirstOct: " + utils::myitoa(getOctFirst()) + "\n";

        info += "-----Image Info-----\n";
        info += "Size: " + utils::myitoa(m_img_width) + " * "
                + utils::myitoa(m_img_height) + "\n";

        return info;
    }

    void SIFT_ADAPTER::extractSiftFeature()
    {
        if (m_has_extracted)
            return;

        m_num_frames = 0;

        bool bfirst = true;
        while (true) // for each octave
        {
            int err = 0;
            if (bfirst)
            {
                err = vl_sift_process_first_octave(m_sift_model, m_gray_data);
                bfirst = false;
            }
            else
                err = vl_sift_process_next_octave(m_sift_model);

            if (err)
                break;

            vl_sift_detect(m_sift_model);
            const int cur_num_key_pts = vl_sift_get_nkeypoints(m_sift_model);
            const VlSiftKeypoint* cur_key_pts = vl_sift_get_keypoints(
                    m_sift_model);

            //std::cout << "detected " << cur_num_key_pts << std::endl;

            for (int i = 0; i < cur_num_key_pts; ++i)
            {
                const VlSiftKeypoint* cur_keypt = cur_key_pts + i;

                double angles[4];
                int nangle = vl_sift_calc_keypoint_orientations(m_sift_model,
                                                                angles,
                                                                cur_keypt);

                for (int q = 0; q < nangle; ++q)
                {

                    float cur_desc[DEFAULT_SIFT_DIM];
                    memset(cur_desc, 0, DEFAULT_SIFT_DIM * sizeof(float));

                    vl_sift_calc_keypoint_descriptor(m_sift_model, cur_desc,
                                                     cur_keypt, angles[q]);

                    SIFT_Frame cur_frame;
                    cur_frame.x = cur_keypt->x;
                    cur_frame.y = cur_keypt->y;
                    cur_frame.scale = cur_keypt->sigma;
                    cur_frame.angle = angles[q];
                    copy(cur_desc, cur_desc + DEFAULT_SIFT_DIM,
                         cur_frame.descriptor.begin());

                    m_frames.push_back(cur_frame);

                    ++m_num_frames;
                }
            }
        }

    }

}
