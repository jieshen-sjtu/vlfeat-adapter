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
#include <stdlib.h>
#include <stdio.h>
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
        clear();
    }

    void HOG_ADAPTER::init()
    {
        init_image_data();
        init_hog_model();
    }

    void HOG_ADAPTER::init_image_data()
    {
        m_gray_data = NULL;
        m_img_width = 0;
        m_img_height = 0;
    }

    void HOG_ADAPTER::clear_image_data()
    {
        if (m_gray_data)
            utils::myfree(&m_gray_data);

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

        m_hog_feature_flip = NULL;
        m_hog_img_flip = NULL;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::reset_hog_model()
    {
        if (m_hog_model)
            vl_hog_delete(m_hog_model);
        m_hog_model = vl_hog_new(m_hog_type, m_num_orient, VL_FALSE);

        if (m_hog_feature)
            utils::myfree(&m_hog_feature);

        if (m_hog_img)
            utils::myfree(&m_hog_img);

        m_has_extracted = false;

        // Flipped HOG data
        if (m_hog_feature_flip)
            utils::myfree(&m_hog_feature_flip);

        if (m_hog_img_flip)
            utils::myfree(&m_hog_img_flip);

        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::clear_hog_model()
    {
        // HOG setting
        m_hog_type = VlHogVariantUoctti;
        m_num_orient = DEFAULT_NUM_ORT;
        m_cell_size = DEFAULT_CELLSIZE;

        // HOG data
        if (m_hog_model)
        {
            vl_hog_delete(m_hog_model);
            m_hog_model = NULL;
        }

        if (m_hog_feature)
            utils::myfree(&m_hog_feature);

        if (m_hog_img)
            utils::myfree(&m_hog_img);

        m_has_extracted = false;

        // Flipped HOG data
        if (m_hog_feature_flip)
            utils::myfree(&m_hog_feature_flip);

        if (m_hog_img_flip)
            utils::myfree(&m_hog_img_flip);

        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::setImage(const Mat* img)
    {
        set_image_data(img);

        vl_hog_put_image(m_hog_model, m_gray_data, m_img_width, m_img_height, 1,
                         m_cell_size);

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::set_image_data(const Mat* img)
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

    void HOG_ADAPTER::setHOGType(const VlHogVariant type)
    {
        assert(type == VlHogVariantDalalTriggs || type == VlHogVariantUoctti);
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

        reset_hog_model();

        if (m_gray_data)
            vl_hog_put_image(m_hog_model, m_gray_data, m_img_width,
                             m_img_height, 1, m_cell_size);
    }

    void HOG_ADAPTER::clear()
    {
        clear_image_data();
        clear_hog_model();
    }

    string HOG_ADAPTER::info() const
    {
        string info = "=====HOG settings=====\n";

        info += "Type: ";
        if (m_hog_type == VlHogVariantUoctti)
            info += "Uoctti";
        else if (m_hog_type == VlHogVariantDalalTriggs)
            info += "DalalTriggs";
        else
            info += "Unknown";
        info += "\n";

        info += "CellSize: " + utils::myitoa(m_cell_size) + "\n";

        info += "NumOrient: " + utils::myitoa(m_num_orient) + "\n";

        return info;
    }

    const Mat HOG_ADAPTER::getImage() const
    {
        return m_org_img;
    }

    VlHogVariant HOG_ADAPTER::getHOGType() const
    {
        return m_hog_type;
    }

    int HOG_ADAPTER::getNumOrient() const
    {
        return m_num_orient;
    }

    int HOG_ADAPTER::getCellSize() const
    {
        return m_cell_size;
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

    vl_size HOG_ADAPTER::getHOGFeatureDim() const
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

    vl_size HOG_ADAPTER::getHOGImageSize() const
    {
        return (getHOGImageWidth() * getHOGImageHeight());
    }

    const float* HOG_ADAPTER::getHOGFeature() const
    {
        return m_hog_feature;
    }

    const float* HOG_ADAPTER::getHOGFeatureFlip() const
    {
        return m_hog_feature_flip;
    }

    const float* HOG_ADAPTER::getHOGImage() const
    {
        return m_hog_img;
    }

    const float* HOG_ADAPTER::getHOGImageFlip() const
    {
        return m_hog_img_flip;
    }

    void HOG_ADAPTER::extractFeature(vector<float>* descriptors)
    {
        vl_size total_dim = getHOGFeatureDim();

        if (m_has_extracted)
        {
            if (descriptors)
            {
                descriptors->resize(total_dim);
                std::copy(m_hog_feature, m_hog_feature + total_dim,
                          descriptors->begin());
            }

            return;
        }

        assert(m_gray_data);

        const vl_size sz = total_dim * sizeof(float);

        if (m_hog_feature)
            free(m_hog_feature);

        m_hog_feature = (float*) utils::mymalloc(sz);

        vl_hog_put_image(m_hog_model, m_gray_data, m_img_width, m_img_height, 1,
                         m_cell_size);

        vl_hog_extract(m_hog_model, m_hog_feature);

        if (descriptors)
        {
            descriptors->resize(total_dim);
            std::copy(m_hog_feature, m_hog_feature + total_dim,
                      descriptors->begin());
        }

        m_has_extracted = true;

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
                                          Mat* hog_img)
    {

        assert(descriptors && _check_region(region));

        if (!m_has_extracted)
        {
            extractFeature();
        }

        if (hog_img)
            visualizeFeature();

        _extract_patch_feature_aux(region, m_hog_feature, descriptors,
                                   m_hog_img, hog_img);

    }

    void HOG_ADAPTER::_extract_patch_feature_aux(const Rect* region,
                                                 const float* feature,
                                                 vector<float>* descriptors,
                                                 const float* hog_img_data,
                                                 Mat* hog_img)
    {
        const int hog_x = region->x / m_cell_size;
        const int hog_y = region->y / m_cell_size;
        const int hog_w = std::ceil(1.0 * region->width / m_cell_size);
        const int hog_h = std::ceil(1.0 * region->height / m_cell_size);

        const vl_size hog_w_org = getHOGWidth();
        const vl_size hog_h_org = getHOGHeight();

        // feature
        const int dim = getHOGCellDim();
        const int sz = hog_w * hog_h * dim;
        descriptors->resize(sz, 0.0);
        for (int nd = 0; nd < dim; ++nd)
            for (int ny = 0; ny < hog_h; ++ny)
            {
                const float* p_org = feature + (ny + hog_y) * hog_w_org
                        + nd * hog_w_org * hog_h_org;
                vector<float>::iterator p_desc = descriptors->begin()
                        + ny * hog_w + nd * hog_w * hog_h;

                for (int nx = 0; nx < hog_w; ++nx)
                {
                    *(p_desc + nx) = *(p_org + nx + hog_x);
                }
            }

        // hog image
        if (hog_img)
        {
            const vl_size glysz = vl_hog_get_glyph_size(m_hog_model);
            const int hog_i_w = hog_w * glysz;
            const int hog_i_h = hog_h * glysz;

            if (hog_img->data)
                hog_img->release();
            hog_img->create(hog_i_w, hog_i_h, CV_32FC1);

            for (int ny = 0; ny < hog_i_h; ++ny)
            {
                float* p_patch = hog_img->ptr<float>(ny);
                const float* p_org = hog_img_data
                        + (ny + hog_y * glysz) * hog_w_org * glysz;
                for (int nx = 0; nx < hog_i_w; ++nx)
                {
                    *(p_patch + nx) = *(p_org + (nx + hog_x * glysz));
                }
            }
        }
    }

    void HOG_ADAPTER::extractFeatureFlip(vector<float>* descriptors)
    {
        if (!m_has_extracted)
            extractFeature(NULL);

        const vl_index* perm_idx = vl_hog_get_permutation(m_hog_model);
        const vl_size dim = getHOGCellDim();

        const vl_size hog_total_dim = getHOGFeatureDim();

        if (m_hog_feature_flip)
            free(m_hog_feature_flip);

        m_hog_feature_flip = (float*) utils::mymalloc(
                hog_total_dim * sizeof(float));

        const vl_size nx = getHOGWidth();
        const vl_size ny = getHOGHeight();
        const vl_size stride = nx * ny;

        for (int d = 0; d < dim; ++d)
        {
            for (int y = 0; y < ny; ++y)
            {
                float* p_flip = m_hog_feature_flip + d * stride + y * nx;
                float* p_org = m_hog_feature + perm_idx[d] * stride + y * nx;

                for (int x = 0; x < nx; ++x)
                {
                    *(p_flip + x) = *(p_org + nx - 1 - x);
                }
            }
        }

        if (descriptors)
        {
            descriptors->resize(hog_total_dim, 0);
            std::copy(m_hog_feature_flip, m_hog_feature_flip + hog_total_dim,
                      descriptors->begin());
        }

        m_has_extracted_flip = true;
    }

    void HOG_ADAPTER::extractPatchFeatureFlip(const Rect* region,
                                              vector<float>* descriptors,
                                              Mat* hog_img_flip)
    {
        assert(descriptors && _check_region(region));

        if (!m_has_extracted_flip)
        {
            extractFeatureFlip();
        }

        if (hog_img_flip)
            visualizeFeatureFlip();

        _extract_patch_feature_aux(region, m_hog_feature_flip, descriptors,
                                   m_hog_img_flip, hog_img_flip);
    }

    void HOG_ADAPTER::visualizeFeature(Mat* hog_img)
    {
        if (!m_has_extracted)
            extractFeature(NULL);

        _visualize_feature_aux(m_hog_feature, &m_hog_img, hog_img);
    }

    void HOG_ADAPTER::visualizeFeatureFlip(Mat* hog_img_flip)
    {
        if (!m_has_extracted_flip)
            extractFeatureFlip(NULL);

        _visualize_feature_aux(m_hog_feature_flip, &m_hog_img_flip,
                               hog_img_flip);
    }

    void HOG_ADAPTER::_visualize_feature_aux(const float* feature,
                                             float** hog_image_data,
                                             Mat* hog_image)
    {
        vl_size hog_img_width = getHOGImageWidth();
        vl_size hog_img_height = getHOGImageHeight();
        vl_size sz = hog_img_width * hog_img_height * sizeof(float);

        if (*hog_image_data)
            free(*hog_image_data);

        *hog_image_data = (float*) utils::mymalloc((size_t) sz);

        vl_hog_render(m_hog_model, *hog_image_data, feature, getHOGWidth(),
                      getHOGHeight());

        if (hog_image == NULL)
            return;

        if (hog_image->data)
            hog_image->release();
        hog_image->create(hog_img_height, hog_img_width, CV_32FC1);

        memcpy(hog_image->data, *hog_image_data, sz);

        if (__DEBUG_HOG_ADAPTER)
        {
            cout << "render done" << endl;
        }
    }

    bool HOG_ADAPTER::_check_region(const Rect* region)
    {
        return (region && utils::within_scope(region->x, 0, m_img_width)
                && utils::within_scope(region->y, 0, m_img_height)
                && utils::within_scope(region->x + region->width - 1, 0,
                                       m_img_width)
                && utils::within_scope(region->y + region->height - 1, 0,
                                       m_img_height));

    }
}
