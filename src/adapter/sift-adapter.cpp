/*
 * sift-adapter.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "sift-adapter.hpp"
#include "../utils.hpp"

#include <iostream>
using std::cerr;
using std::endl;
using std::abs;

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
        BASIC_ADAPTER::init();
        init_sift_model();
    }

    void SIFT_ADAPTER::clear()
    {
        clear_sift_model();
        clearImage();
    }

    void SIFT_ADAPTER::init_sift_model()
    {
        init_sift_parameters();

        m_sift_model = NULL;
        m_has_extracted = false;

        m_num_frames = 0;
        m_frames.reserve(1024);
    }

    void SIFT_ADAPTER::init_sift_parameters()
    {
        m_noctave = DEFAULT_NOCTAVE_INVALID;
        m_nlevel = DEFAULT_NLEVEL_INVALID;
        m_oct_first = DEFAULT_OCT_FIRST_INVALID;

        m_edge_thrd = DEFAULT_EDGE_THRD_INVALID;
        m_peak_thrd = DEFAULT_PEAK_THRD_INVALID;
        m_norm_thrd = DEFAULT_NORM_THRD_INVALID;
        m_magnif = DEFAULT_MAGNIF_INVALID;
        m_window_sz = DEFAULT_WIN_SIZE_INVALID;
    }

    void SIFT_ADAPTER::clear_sift_model()
    {
        clear_model_related_data();
        init_sift_parameters();
    }

    void SIFT_ADAPTER::clear_model_related_data()
    {
        m_has_extracted = false;

        if (m_sift_model)
        {
            vl_sift_delete(m_sift_model);
            m_sift_model = NULL;
        }

        m_num_frames = 0;

        for (size_t i = 0; i < m_frames.size(); ++i)
            m_frames[i].clear();
        m_frames.clear();
        m_frames.reserve(1024);

        if (m_sift_img.data)
            m_sift_img.release();
    }

    void SIFT_ADAPTER::set_sift_model()
    {
        clear_model_related_data();

        m_sift_model = vl_sift_new(m_org_img.cols, m_org_img.rows, getNOctaves(),
                                   getNLevels(), getOctFirst());

        vl_sift_set_edge_thresh(m_sift_model, getEdgeThrd());
        vl_sift_set_peak_thresh(m_sift_model, getPeakThrd());
        vl_sift_set_norm_thresh(m_sift_model, getNormThrd());
        vl_sift_set_magnif(m_sift_model, getMagnif());
        vl_sift_set_window_size(m_sift_model, getWindowSize());
    }

    void SIFT_ADAPTER::setImage(const Mat* img)
    {
        img->copyTo(m_org_img);
        //set_image_data();
        m_has_set_image = true;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setNOctaves(const int noct)
    {
        if (noct <= 0)
        {
            cerr << "number of octaves should be a positive integer" << endl;
            exit(-1);
        }

        if (noct == m_noctave)
            return;

        m_noctave = noct;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setNLevels(const int nlevel)
    {
        if (nlevel <= 0)
        {
            cerr << "number of levels should be a positive integer" << endl;
            exit(-1);
        }

        if (nlevel == m_nlevel)
            return;

        m_nlevel = nlevel;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setOctFirst(const int o_min)
    {
        if (o_min < 0)
        {
            cerr << "octave first should be a non-negative integer" << endl;
            exit(-1);
        }

        if (o_min == m_oct_first)
            return;

        m_oct_first = o_min;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setEdgeThrd(const double t)
    {
        if (t < 1)
        {
            cerr << "edge threshold should be >= 1" << endl;
            exit(-1);
        }
        if (_same_val(t, m_edge_thrd))
            return;

        m_edge_thrd = t;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setPeakThrd(const double t)
    {
        if (t < 0)
        {
            cerr << "peak threshold should be >= 0" << endl;
            exit(-1);
        }
        if (_same_val(t, m_peak_thrd))
            return;

        m_peak_thrd = t;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setNormThrd(const double t)
    {
        if (t <= 0)
        {
            cerr << "norm threshold should be > 0" << endl;
            exit(-1);
        }
        if (_same_val(t, m_norm_thrd))
            return;

        m_norm_thrd = t;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setMagnif(const double t)
    {
        if (t < 0)
        {
            cerr << "magnification should be >= 0" << endl;
            exit(-1);
        }

        if (_same_val(t, m_magnif))
            return;

        m_magnif = t;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::setWindowSize(const double t)
    {
        if (t < 0)
        {
            cerr << "window size should be >= 0" << endl;
            exit(-1);
        }

        if (_same_val(t, m_window_sz))
            return;

        m_window_sz = t;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetNOctaves()
    {
        if (m_noctave == DEFAULT_NOCTAVE_INVALID)
            return;

        m_noctave = DEFAULT_NOCTAVE_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetNLevels()
    {
        if (m_nlevel == DEFAULT_NLEVEL_INVALID)
            return;

        m_nlevel = DEFAULT_NLEVEL_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetOctFirst()
    {
        if (m_oct_first == DEFAULT_OCT_FIRST_INVALID)
            return;

        m_oct_first = DEFAULT_OCT_FIRST;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetEdgeThrd()
    {
        if (_same_val(m_edge_thrd, (double) DEFAULT_EDGE_THRD_INVALID))
            return;

        m_edge_thrd = DEFAULT_EDGE_THRD_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetPeakThrd()
    {
        if (_same_val(m_peak_thrd, (double) DEFAULT_PEAK_THRD_INVALID))
            return;
        m_peak_thrd = DEFAULT_PEAK_THRD_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetNormThrd()
    {
        if (_same_val(m_norm_thrd, (double) DEFAULT_NORM_THRD_INVALID))
            return;
        m_norm_thrd = DEFAULT_NORM_THRD_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetMagnif()
    {
        if (_same_val(m_magnif, (double) DEFAULT_MAGNIF_INVALID))
            return;
        m_magnif = DEFAULT_MAGNIF_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetWindowSize()
    {
        if (_same_val(m_window_sz, (double) DEFAULT_WIN_SIZE_INVALID))
            return;
        m_window_sz = DEFAULT_WIN_SIZE_INVALID;
        m_has_extracted = false;
    }

    void SIFT_ADAPTER::resetSiftModel()
    {
        clear_sift_model();
    }

    void SIFT_ADAPTER::clearImage()
    {
        clear_gray_image_data();
        if(m_org_img.data)
            m_org_img.release();
        m_has_set_image = false;

        m_has_extracted = false;
    }

    int SIFT_ADAPTER::getNOctaves() const
    {
        if (!m_has_extracted)
        {
            if (m_noctave == DEFAULT_NOCTAVE_INVALID)
                return DEFAULT_NOCTAVE;
            else
                return m_noctave;
        }
        return vl_sift_get_noctaves(m_sift_model);
    }

    int SIFT_ADAPTER::getNLevels() const
    {
        if (!m_has_extracted)
        {
            if (m_nlevel == DEFAULT_NLEVEL_INVALID)
                return DEFAULT_NLEVEL;
            else
                return m_nlevel;
        }
        return vl_sift_get_nlevels(m_sift_model);
    }

    int SIFT_ADAPTER::getOctFirst() const
    {
        if (!m_has_extracted)
        {
            if (m_oct_first == DEFAULT_OCT_FIRST_INVALID)
                return DEFAULT_OCT_FIRST;
            else
                return m_oct_first;
        }
        return vl_sift_get_octave_first(m_sift_model);
    }

    double SIFT_ADAPTER::getEdgeThrd() const
    {
        if (!m_has_extracted)
        {
            if (std::abs(
                    m_edge_thrd - DEFAULT_EDGE_THRD_INVALID)<__SIFT_ADAPTER_EPS)
                return DEFAULT_EDGE_THRD;
            else
                return m_edge_thrd;
        }
        return vl_sift_get_edge_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getPeakThrd() const
    {
        if (!m_has_extracted)
        {
            if (std::abs(
                    m_peak_thrd - DEFAULT_PEAK_THRD_INVALID)<__SIFT_ADAPTER_EPS)
                return DEFAULT_PEAK_THRD;
            else
                return m_peak_thrd;
        }
        return vl_sift_get_peak_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getNormThrd() const
    {
        if (!m_has_extracted)
        {
            if (std::abs(
                    m_norm_thrd - DEFAULT_NORM_THRD_INVALID)<__SIFT_ADAPTER_EPS)
                return DEFAULT_NORM_THRD;
            else
                return m_norm_thrd;
        }
        return vl_sift_get_norm_thresh(m_sift_model);
    }

    double SIFT_ADAPTER::getMagnif() const
    {
        if (!m_has_extracted)
        {
            if (std::abs(m_magnif - DEFAULT_MAGNIF_INVALID) < __SIFT_ADAPTER_EPS)
                return DEFAULT_MAGNIF;
            else
                return m_magnif;
        }
        return vl_sift_get_magnif(m_sift_model);
    }

    double SIFT_ADAPTER::getWindowSize() const
    {
        if (!m_has_extracted)
        {
            if (std::abs(
                    m_window_sz - DEFAULT_WIN_SIZE_INVALID)<__SIFT_ADAPTER_EPS)
                return DEFAULT_WIN_SIZE;
            else
                return m_window_sz;
        }
        return vl_sift_get_window_size(m_sift_model);
    }

    const vector<SIFT_Frame>& SIFT_ADAPTER::getAllFrames() const
    {
        if (!m_has_extracted)
        {
            cerr << "There is no sift frame. Please check the image and model setting"
                 << endl;
            exit(-1);
        }
        return m_frames;
    }

    const Mat SIFT_ADAPTER::getSiftImage() const
    {
        if (!m_has_extracted || !m_sift_img.data)
        {
            cerr << "Please call the visualizeSiftFeature() method first"
                 << endl;
            exit(-1);
        }
        return m_sift_img;
    }

    string SIFT_ADAPTER::info() const
    {
        string info = "=====SIFT settings=====\n";

        info += "NOctave:  " + utils::myitoa(getNOctaves()) + "\n";

        info += "NLevel:   " + utils::myitoa(getNLevels()) + "\n";

        info += "FirstOct: " + utils::myitoa(getOctFirst()) + "\n";

        info += "EdgeThrd: " + utils::myitoa(getEdgeThrd()) + "\n";

        info += "PeakThrd: " + utils::myitoa(getPeakThrd()) + "\n";

        info += "NormThrd: " + utils::myitoa(getNormThrd()) + "\n";

        info += "Magnif:   " + utils::myitoa(getMagnif()) + "\n";

        info += "WindowSz: " + utils::myitoa(getWindowSize()) + "\n";

        info += "\n-----Image Info-----\n";

        info += "Size:     " + utils::myitoa(m_org_img.cols) + " * "
                + utils::myitoa(m_org_img.rows) + "\n";

        info += "Key Point:" + utils::myitoa(m_num_frames) + "\n";

        return info;
    }

    void SIFT_ADAPTER::extractSiftFeature()
    {
        check_image();

        if (m_has_extracted)
            return;

        set_gray_image_data();
        set_sift_model();

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

        m_has_extracted = true;

    }

    /*
     * The visualization code is from : Rob Hess <hess@eecs.oregonstate.edu>
     */
    void SIFT_ADAPTER::visualizeSiftFeature(Mat* sift_img)
    {
        check_image();

        if (!m_has_extracted)
            extractSiftFeature();

        int m_img_width = m_org_img.cols;
        int m_img_height = m_org_img.rows;

        if (m_sift_img.data)
            m_sift_img.release();
        m_sift_img.create(m_img_height, m_img_width, CV_8UC3);

        for (int y = 0; y < m_img_height; ++y)
        {
            cv::Vec3b* p_line = m_sift_img.ptr<cv::Vec3b>(y);
            const float* p_gray_data = m_gray_data + y * m_img_width;

            for (int x = 0; x < m_img_width; ++x)
            {
                for (int i = 0; i < 3; ++i)
                    p_line[x].val[i] = p_gray_data[x];
            }
        }

        const double scale = 5.0;
        const double hscale = 0.75;
        const cv::Scalar color = cv::Scalar(255, 0, 255);

        for (int i = 0; i < m_num_frames; ++i)
        {
            int len, hlen, blen, start_x, start_y, end_x, end_y, h1_x, h1_y,
                    h2_x, h2_y;
            double scl, ori;

            Point start, end, h1, h2;

            const SIFT_Frame& cur_frame = m_frames[i];

            start_x = cur_frame.x;
            start_y = cur_frame.y;
            scl = cur_frame.scale;
            ori = cur_frame.angle;

            len = cvRound(scl * scale);
            hlen = cvRound(scl * hscale);
            blen = len - hlen;
            end_x = cvRound(len * cos(ori)) + start_x;
            end_y = cvRound(len * -sin(ori)) + start_y;
            h1_x = cvRound(blen * cos(ori + CV_PI / 18.0)) + start_x;
            h1_y = cvRound(blen * -sin(ori + CV_PI / 18.0)) + start_y;
            h2_x = cvRound(blen * cos(ori - CV_PI / 18.0)) + start_x;
            h2_y = cvRound(blen * -sin(ori - CV_PI / 18.0)) + start_y;
            start = cvPoint(start_x, start_y);
            end = cvPoint(end_x, end_y);
            h1 = cvPoint(h1_x, h1_y);
            h2 = cvPoint(h2_x, h2_y);

            cv::line(m_sift_img, start, end, color, 1, 8, 0);
            cv::line(m_sift_img, end, h1, color, 1, 8, 0);
            cv::line(m_sift_img, end, h2, color, 1, 8, 0);
        }

        if (sift_img)
            m_sift_img.copyTo(*sift_img);
    }

    bool SIFT_ADAPTER::_same_val(double a, double b) const
    {
        double diff = std::abs(a - b);
        if (diff < __SIFT_ADAPTER_EPS)
            return true;
        else
            return false;
    }

}
