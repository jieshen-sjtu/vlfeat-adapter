/*
 * hog-adapter.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: jieshen
 */

#include "hog-adapter.hpp"

#include "../utils.hpp"

#include <cassert>
#include <cmath>
using namespace std;

#define __DEBUG_HOG_ADAPTER true

namespace jieshen
{
    HOG_ADAPTER::HOG_ADAPTER()
    {
        init();
    }

    HOG_ADAPTER::HOG_ADAPTER(const Mat* img)
    {
        init();
        setImage(img);
    }

    HOG_ADAPTER::~HOG_ADAPTER()
    {
        if (m_gray_data)
        {
            free(m_gray_data);
            m_gray_data = NULL;
        }

        if (m_hog_model)
        {
            vl_hog_delete(m_hog_model);
            m_hog_model = NULL;
        }

        if (m_hog_feature)
        {
            free(m_hog_feature);
            m_hog_feature = NULL;
        }

        if (m_hog_img)
        {
            free(m_hog_img);
            m_hog_img = NULL;
        }
    }

    void HOG_ADAPTER::init()
    {
        init_image_info();
        init_hog_model();
    }

    void HOG_ADAPTER::init_image_info()
    {
        m_gray_data = NULL;
        m_img_width = 0;
        m_img_height = 0;
    }

    void HOG_ADAPTER::init_hog_model()
    {
        m_hog_type = VlHogVariantUoctti;
        m_num_orient = DEFAULT_NUM_ORT;
        m_cell_size = DEFAULT_CELLSIZE;

        m_hog_model = vl_hog_new(m_hog_type, m_num_orient, VL_FALSE);

        m_hog_feature = NULL;
        m_hog_img = NULL;
        m_has_extracted = false;
    }

    void HOG_ADAPTER::reset_hog_model()
    {
        if (m_hog_model)
            vl_hog_delete(m_hog_model);
        m_hog_model = vl_hog_new(m_hog_type, m_num_orient, VL_FALSE);
    }

    void HOG_ADAPTER::setImage(const Mat* img)
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

        m_gray_data = (float*) malloc(sz);

        memcpy(m_gray_data, gray_img.data, sz);

        vl_hog_put_image(m_hog_model, m_gray_data, m_img_width, m_img_height, 1,
                         m_cell_size);
    }

    void HOG_ADAPTER::setHOGType(const VlHogVariant type)
    {
        assert(type == VlHogVariantDalalTriggs || type==VlHogVariantUoctti);
        m_hog_type = type;

        reset_hog_model();
        if (m_gray_data)
            vl_hog_put_image(m_hog_model, m_gray_data, m_img_width,
                             m_img_height, 1, m_cell_size);
    }

    void HOG_ADAPTER::setNumOrient(const int ort)
    {
        assert(ort > 0);
        m_num_orient = ort;

        reset_hog_model();
        if (m_gray_data)
            vl_hog_put_image(m_hog_model, m_gray_data, m_img_width,
                             m_img_height, 1, m_cell_size);
    }

    void HOG_ADAPTER::setCellSize(const int cellsz)
    {
        assert(cellsz > 0);
        m_cell_size = cellsz;

        vl_hog_put_image(m_hog_model, m_gray_data, m_img_width, m_img_height, 1,
                         m_cell_size);
    }

    vl_size HOG_ADAPTER::getHOGWidth() const
    {
        return vl_hog_get_width(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGHeight() const
    {
        return vl_hog_get_height(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGCellDim() const
    {
        return vl_hog_get_dimension(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGTotalDim() const
    {
        vl_size w = getHOGWidth();
        vl_size h = getHOGHeight();
        vl_size d = getHOGCellDim();

        return (w * h * d);
    }

    vl_size HOG_ADAPTER::getHOGImageWidth() const
    {
        return (vl_hog_get_glyph_size(m_hog_model) * getHOGWidth());
    }

    vl_size HOG_ADAPTER::getHOGImageHeight() const
    {
        return (vl_hog_get_glyph_size(m_hog_model) * getHOGHeight());
    }

    void HOG_ADAPTER::extractFeature(vector<float>* descriptors)
    {
        assert(m_gray_data);

        vl_size total_dim = getHOGTotalDim();
        const vl_size sz = total_dim * sizeof(float);

        if (m_hog_feature)
            free(m_hog_feature);

        m_hog_feature = (float*) vl_malloc(sz);
        memset(m_hog_feature, 0, sz);

        vl_hog_put_image(m_hog_model, m_gray_data, m_img_width, m_img_height, 1,
                         m_cell_size);

        vl_hog_extract(m_hog_model, m_hog_feature);

        m_has_extracted = true;

        if (descriptors)
        {
            descriptors->resize(total_dim);
            std::copy(m_hog_feature, m_hog_feature + total_dim,
                      descriptors->begin());
        }

        if (__DEBUG_HOG_ADAPTER)
        {
            cout << "image size: " << m_org_img.cols << " * " << m_org_img.rows
                 << endl;
            cout << "hog size: " << getHOGWidth() << " * " << getHOGHeight()
                 << " * " << getHOGCellDim() << endl;
            cout << endl << "extract HOG-feature done" << endl;

            /*
             for (int i = 0; i < sz / sizeof(float); ++i)
             if (i % 100 == 0)
             cout << m_hog_feature[i] << " ";
             cout << endl;*/
        }
    }

    void HOG_ADAPTER::extractPatchFeature(const Rect* region,
                                          vector<float>* descriptors,
                                          Mat* hog_img, bool bPrecise)
    {
        assert(region && descriptors);

        assert(utils::within_scope(region->x, 0, m_img_width));
        assert(utils::within_scope(region->y,0, m_img_height));
        assert(utils::within_scope(region->x + region->width - 1, 0, m_img_width));
        assert(utils::within_scope(region->y + region->height - 1, 0, m_img_height));

        if (!m_has_extracted)
        {
            extractFeature();
            if (hog_img)
                visualizeFeature();
        }

        if (!bPrecise)
        {
            const int hog_x = region->x / m_cell_size;
            const int hog_y = region->y / m_cell_size;
            const int hog_w = std::ceil(1.0 * region->width / m_cell_size);
            const int hog_h = std::ceil(1.0 * region->height / m_cell_size);
            const int x = hog_x * m_cell_size;
            const int y = hog_y * m_cell_size;
            const int w = hog_w * m_cell_size;
            const int h = hog_h * m_cell_size;

            // feature
            const int dim = getHOGCellDim();
            const int sz = hog_w * hog_h * dim;
            descriptors->resize(sz, 0.0);
            for (int nx = 0; nx < hog_w; ++nx)
                for (int ny = 0; ny < hog_h; ++ny)
                    for (int nd = 0; nd < dim; ++nd)
                    {
                        (*descriptors)[nx + ny * hog_w + nd * hog_w * hog_h] = m_hog_feature[(nx
                                + hog_x) + (ny + hog_y) * hog_w
                                + nd * hog_w * hog_h];
                    }

            // hog image
            if (hog_img)
            {
                const vl_size glysz = vl_hog_get_glyph_size(m_hog_model);
                const int hog_i_w = hog_w * glysz;
                const int hog_i_h = hog_h * glysz;
                const int sz = hog_i_w * hog_i_h * sizeof(float);
                float* img_data = (float*) malloc(sz);
                memset(img_data, 0, sz);
                for (int nx = 0; nx < hog_i_w; ++nx)
                    for (int ny = 0; ny < hog_i_h; ++ny)
                    {
                        img_data[nx + ny * hog_i_w] = m_hog_img[(nx
                                + hog_x * glysz)
                                + (ny + hog_y * glysz) * hog_w * glysz];
                    }

                if (hog_img->data)
                    hog_img->release();
                hog_img->create(hog_i_w, hog_i_h, CV_32FC1);

                memcpy(hog_img->data, m_hog_img, sz);
            }

        }

    }

    void HOG_ADAPTER::visualizeFeature(Mat* hog_img)
    {
        vl_size hog_img_width = getHOGImageWidth();
        vl_size hog_img_height = getHOGImageHeight();
        vl_size sz = hog_img_width * hog_img_height * sizeof(float);

        if (m_hog_img)
            free(m_hog_img);

        m_hog_img = (float*) malloc(sz);

        vl_hog_render(m_hog_model, m_hog_img, m_hog_feature, getHOGWidth(),
                      getHOGHeight());

        if (hog_img == NULL)
            return;

        if (hog_img->data)
            hog_img->release();
        hog_img->create(hog_img_height, hog_img_width, CV_32FC1);

        memcpy(hog_img->data, m_hog_img, sz);

        if (__DEBUG_HOG_ADAPTER)
        {
            cout << "render done" << endl;
        }
    }
}
