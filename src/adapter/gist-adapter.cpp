/*
 * gist-adapter.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jieshen
 */

#include "gist-adapter.hpp"

#include "../utils.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace jieshen
{
    const int GIST_ADAPTER::DEFAULT_ORT[3] = { 8, 8, 4 };

    GIST_ADAPTER::GIST_ADAPTER()
    {
        init();
    }

    GIST_ADAPTER::GIST_ADAPTER(const Mat* img)
    {
        init();
        setImage(img);
    }

    GIST_ADAPTER::~GIST_ADAPTER()
    {
        clear();
    }

    void GIST_ADAPTER::init()
    {
        BASIC_ADAPTER::init();
        init_gist_model();
    }

    void GIST_ADAPTER::init_gist_model()
    {
        m_nblock = 0;
        m_nscale = 0;
        m_orients = NULL;

        m_gist_features = NULL;
        m_has_extracted = false;

        m_has_set_image = false;
        m_is_gray = true;
        m_gray_img = NULL;
        m_color_img = NULL;
    }

    void GIST_ADAPTER::clear()
    {
        BASIC_ADAPTER::clear();
        clear_gist_model();
    }

    void GIST_ADAPTER::clear_gist_model()
    {
        clear_raw_memory_data();

        m_nblock = 0;
        m_nscale = 0;

        m_has_extracted = false;

    }

    void GIST_ADAPTER::clear_raw_memory_data()
    {
        if (m_orients)
            utils::myfree(&m_orients);

        if (m_gist_features)
            utils::myfree(&m_gist_features);

        if (m_is_gray)
        {
            if (m_gray_img)
            {
                image_delete(m_gray_img);
                m_gray_img = NULL;
            }
        }
        else
        {
            if (m_color_img)
            {
                color_image_delete(m_color_img);
                m_color_img = NULL;
            }
        }
    }

    void GIST_ADAPTER::reset_gist_model()
    {
        if (m_nblock == 0)
            m_nblock = DEFAULT_NBLOCK;

        if (m_nscale == 0)
            m_nscale = DEFAULT_NSCALE;

        if (m_orients == NULL)
        {
            m_orients = (int*) utils::mymalloc(m_nscale * sizeof(int));
            memcpy(m_orients, DEFAULT_ORT, m_nscale * sizeof(int));
        }

        if (m_gist_features)
            utils::myfree(&m_gist_features);

        m_has_extracted = false;
    }

    void GIST_ADAPTER::setImage(const Mat* img)
    {
        set_image_data(img);
        // reset_gist_model();

        if (m_gist_features)
            utils::myfree(&m_gist_features);

        m_has_extracted = false;

        if (img->channels() == 1)
            m_is_gray = true;
        else
            m_is_gray = false;

        if (m_is_gray)
        {
            m_gray_img = image_new(m_img_width, m_img_height);
            memcpy(m_gray_img->data, m_gray_data,
                   m_img_width * m_img_height * sizeof(float));
        }
        else
        {
            m_color_img = color_image_new(m_img_width, m_img_height);
            Mat _img;
            img->convertTo(_img, CV_32FC3);

            for (int y = 0; y < m_img_height; ++y)
            {
                const cv::Vec3f* p_line = _img.ptr<cv::Vec3f>(y);
                int stride = y * m_img_width;

                float* r = m_color_img->c1 + stride;
                float* g = m_color_img->c2 + stride;
                float* b = m_color_img->c3 + stride;

                for (int x = 0; x < m_img_width; ++x)
                {
                    *(r + x) = (p_line + x)->val[2];
                    *(g + x) = (p_line + x)->val[1];
                    *(b + x) = (p_line + x)->val[0];
                }
            }
        }

        m_has_set_image = true;
    }

    void GIST_ADAPTER::setNBlock(const int nblock)
    {
        assert(nblock > 0);

        if (nblock == m_nblock)
            return;

        m_nblock = nblock;

        m_has_extracted = false;
    }

    void GIST_ADAPTER::setOrients(const int nort, const int* orts)
    {
        assert(nort > 0 && orts);

        if (nort == m_nscale)
        {
            bool same = true;
            for (int i = 0; same && i < nort; ++i)
                if (orts[i] != m_orients[i])
                    same = false;
            if (same)
                return;
        }

        m_nscale = nort;

        if (m_orients)
            utils::myfree(&m_orients);
        memcpy(m_orients, orts, m_nscale * sizeof(int));

        m_has_extracted = false;
    }

    int GIST_ADAPTER::getNBlock() const
    {
        return m_nblock;
    }

    int GIST_ADAPTER::getNScale() const
    {
        return m_nscale;
    }

    const int* GIST_ADAPTER::getNOrientsPerScale() const
    {
        return m_orients;
    }

    int GIST_ADAPTER::getGistFeatureDim() const
    {
        int dim = m_nblock * m_nblock;
        int s = 0;
        for (int i = 0; i < m_nscale; ++i)
            s += m_orients[i];
        dim *= s;

        dim *= m_org_img.channels();

        return dim;
    }

    const float* GIST_ADAPTER::getGistFeature() const
    {
        return m_gist_features;
    }

    void GIST_ADAPTER::extractGistFeature(vector<float>* descriptor)
    {
        if (m_has_extracted)
            return;

        assert(m_has_set_image);

        reset_gist_model();

        if (m_is_gray)
            m_gist_features = bw_gist_scaletab(m_gray_img, m_nblock, m_nscale,
                                               m_orients);
        else
            m_gist_features = color_gist_scaletab(m_color_img, m_nblock,
                                                  m_nscale, m_orients);

        m_has_extracted = true;

        if (descriptor)
        {
            int dim = getGistFeatureDim();
            descriptor->resize(dim, 0.0);
            std::copy(m_gist_features, m_gist_features + dim,
                      descriptor->begin());

            std::cerr << dim << std::endl;

        }
    }

    string GIST_ADAPTER::info() const
    {
        using std::cout;
        using std::endl;
        std::string info = "=====GIST Settings=====\n";

        info += "NBlock:" + utils::myitoa(m_nblock) + "\n";
        info += "NScale:" + utils::myitoa(m_nscale) + "\n";
        info += string("NOrient:");

        for (int i = 0; i < m_nscale; ++i)
            info += utils::myitoa(m_orients[i]) + " ";

        info += "\n";

        info += "\n-----Image Info-----\n";
        info += "Size: " + utils::myitoa(m_img_width) + " * "
                + utils::myitoa(m_img_height) + "\n";

        return info;
    }
}
