/*
 * basic-adapter.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "basic-adapter.hpp"
#include "../utils.hpp"

namespace jieshen
{
    BASIC_ADAPTER::BASIC_ADAPTER()
    {
        init();
    }

    BASIC_ADAPTER::BASIC_ADAPTER(const Mat* img)
    {
        init();
        m_org_img = *img;
    }

    BASIC_ADAPTER::~BASIC_ADAPTER()
    {
        clear_gray_image_data();
    }

    void BASIC_ADAPTER::init()
    {
        init_image_data();
    }

    void BASIC_ADAPTER::init_image_data()
    {
        m_has_set_image = false;
        m_gray_data = NULL;
    }

    void BASIC_ADAPTER::clear_gray_image_data()
    {
        if (m_gray_data)
            utils::myfree(&m_gray_data);
    }

    void BASIC_ADAPTER::set_gray_image_data()
    {
        if (!m_org_img.data)
        {
            std::cerr << "Please set image first" << std::endl;
            exit(-1);
        }

        //img->copyTo(m_org_img);

        Mat gray_img;
        if (m_org_img.channels() == 3)
            cv::cvtColor(m_org_img, gray_img, CV_BGR2GRAY);
        else
            gray_img = m_org_img.clone();

        gray_img.convertTo(gray_img, CV_32FC1);

        int m_img_width = m_org_img.cols;
        int m_img_height = m_org_img.rows;

        const int sz = m_img_width * m_img_height * sizeof(float);

        if (m_gray_data)
            free(m_gray_data);

        m_gray_data = (float*) utils::mymalloc(sz);

        memcpy(m_gray_data, gray_img.data, sz);
    }

    const Mat BASIC_ADAPTER::getImage() const
    {
        return m_org_img;
    }

    void BASIC_ADAPTER::check_image() const
    {
        if (!m_has_set_image)
        {
            std::cerr << "Please set the image for processing" << std::endl;
            exit(-1);
        }
    }
}
