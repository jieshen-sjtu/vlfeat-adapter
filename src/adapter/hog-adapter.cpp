/*
 * hog-adapter.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: jieshen
 */

#include "hog-adapter.hpp"

#include "basic-adapter.hpp"
#include "utils.hpp"

#include <cassert>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define __DEBUG_HOG_ADAPTER false

using std::cerr;
using std::endl;

namespace jieshen
{
    const VlHogVariant HOG_ADAPTER::DEFAULT_HOG_TYPE = VlHogVariantUoctti;

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
        BASIC_ADAPTER::init();
        init_hog_model();
    }

    void HOG_ADAPTER::init_hog_model()
    {
        init_hog_parameters();

        m_hog_model = NULL;

        m_hog_feature = NULL;
        m_hog_img = NULL;
        m_has_extracted = false;

        m_hog_feature_flip = NULL;
        m_hog_img_flip = NULL;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::init_hog_parameters()
    {
        m_hog_type = DEFAULT_HOG_TYPE;
        m_cell_size = DEFAULT_NUM_ORT_INVALID;
        m_num_orient = DEFAULT_CELLSIZE_INVALID;
    }

    void HOG_ADAPTER::set_hog_model()
    {
        clear_model_related_data();

        m_hog_model = vl_hog_new(getHOGType(), getNumOrient(), VL_FALSE);
    }

    void HOG_ADAPTER::clear_hog_model()
    {
        clear_model_related_data();
        init_hog_parameters();
    }

    void HOG_ADAPTER::clear_model_related_data()
    {
        m_has_extracted = false;
        m_has_extracted_flip = false;

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

        // Flipped HOG data
        if (m_hog_feature_flip)
            utils::myfree(&m_hog_feature_flip);

        if (m_hog_img_flip)
            utils::myfree(&m_hog_img_flip);
    }

    void HOG_ADAPTER::clearImage()
    {
        m_has_extracted = false;
        m_has_extracted_flip = false;

        clear_gray_image_data();
        if (m_org_img.data)
            m_org_img.release();

        m_has_set_image = false;
    }

    void HOG_ADAPTER::resetHOGModel()
    {
        clear_hog_model();
    }

    void HOG_ADAPTER::setImage(const Mat* img)
    {
        img->copyTo(m_org_img);
        //set_image_data();
        m_has_set_image = true;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::setHOGType(const VlHogVariant type)
    {
        if (type != VlHogVariantDalalTriggs && type != VlHogVariantUoctti)
        {
            cerr << "check the HOG type: should be one of the following:"
                 << endl << "1. VlHogVariantUoctti" << endl
                 << "2. VlHogVariantDalalTriggs" << endl;
            exit(-1);
        }

        if (type == m_hog_type)
            return;

        m_hog_type = type;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::setNumOrient(const int ort)
    {
        if (ort <= 0)
        {
            cerr << "number of orientation should be > 0" << endl;
            exit(-1);
        }

        if (ort == m_cell_size)
            return;

        m_cell_size = ort;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::setCellSize(const int cellsz)
    {
        if (cellsz <= 0)
        {
            cerr << "cell size should be > 0" << endl;
            exit(-1);
        }

        if (cellsz == m_cell_size)
            return;

        m_cell_size = cellsz;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::resetHOGType()
    {
        if (m_hog_type == DEFAULT_HOG_TYPE)
            return;

        m_hog_type = DEFAULT_HOG_TYPE;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::resetNumOrient()
    {
        if (m_num_orient == DEFAULT_NUM_ORT_INVALID)
            return;

        m_num_orient = DEFAULT_NUM_ORT_INVALID;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::resetCellSize()
    {
        if (m_cell_size == DEFAULT_CELLSIZE_INVALID)
            return;

        m_cell_size = DEFAULT_CELLSIZE_INVALID;

        m_has_extracted = false;
        m_has_extracted_flip = false;
    }

    void HOG_ADAPTER::clear()
    {
        clear_hog_model();
        clearImage();
    }

    string HOG_ADAPTER::info() const
    {
        string info = "=====HOG settings=====\n";

        info += "Type:        ";
        if (m_hog_type == VlHogVariantUoctti)
            info += "Uoctti";
        else if (m_hog_type == VlHogVariantDalalTriggs)
            info += "DalalTriggs";
        else
            info += "Unknown";
        info += "\n";

        info += "CellSize:     " + utils::myitoa(getCellSize()) + "\n";

        info += "NumOrient:    " + utils::myitoa(getNumOrient()) + "\n";

        info += "\n-----Image Info-----\n";
        info += "Size:         " + utils::myitoa(m_org_img.cols) + " * "
                + utils::myitoa(m_org_img.rows) + "\n";
        info += "Feature Size: " + utils::myitoa(getHOGFeatureDim()) + "\n";

        return info;
    }

    VlHogVariant HOG_ADAPTER::getHOGType() const
    {
        return m_hog_type;
    }

    int HOG_ADAPTER::getNumOrient() const
    {
        if (m_num_orient == DEFAULT_NUM_ORT_INVALID)
            return DEFAULT_NUM_ORT;
        return m_num_orient;
    }

    int HOG_ADAPTER::getCellSize() const
    {
        if (m_cell_size == DEFAULT_CELLSIZE_INVALID)
            return DEFAULT_CELLSIZE;
        return m_cell_size;
    }

    vl_size HOG_ADAPTER::getHOGXDim() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            return 0;
        }
        return vl_hog_get_width(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGYDim() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            return 0;
        }
        return vl_hog_get_height(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGCellDim() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            return 0;
        }
        return vl_hog_get_dimension(m_hog_model);
    }

    vl_size HOG_ADAPTER::getHOGFeatureDim() const
    {
        vl_size w = getHOGXDim();
        vl_size h = getHOGYDim();
        vl_size d = getHOGCellDim();

        return (w * h * d);
    }

    vl_size HOG_ADAPTER::getHOGImageWidth() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            exit(-1);
        }
        return (vl_hog_get_glyph_size(m_hog_model) * getHOGXDim());
    }

    vl_size HOG_ADAPTER::getHOGImageHeight() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            exit(-1);
        }
        return (vl_hog_get_glyph_size(m_hog_model) * getHOGYDim());
    }

    vl_size HOG_ADAPTER::getHOGImageSize() const
    {
        if (!m_hog_model)
        {
            cerr << "Please call extractHOGFeature() first" << endl;
            exit(-1);
        }
        return (getHOGImageWidth() * getHOGImageHeight());
    }

    const float* HOG_ADAPTER::getHOGFeature() const
    {
        if (!m_has_extracted)
        {
            cerr << "There is no HOG feature. Please check the image\\"
                 " and model setting, then call extractHOGFeature()"
                 << endl;
            exit(-1);
        }
        return m_hog_feature;
    }

    const float* HOG_ADAPTER::getHOGFeatureFlip() const
    {
        if (!m_has_extracted_flip)
        {
            cerr << "There is no HOG flip feature. Please check the image\\"
                 " and model setting, then call extractHOGFeatureFlip()"
                 << endl;
            exit(-1);
        }
        return m_hog_feature_flip;
    }

    const float* HOG_ADAPTER::getHOGImage() const
    {
        if (!m_hog_img)
        {
            cerr << "There is no HOG image. Please call the visualizeHOGFeature() first."
                 << endl;
            exit(-1);
        }
        return m_hog_img;
    }

    const float* HOG_ADAPTER::getHOGImageFlip() const
    {
        if (!m_hog_img)
        {
            cerr << "There is no HOG image. Please call the visualizeHOGFeatureFlip() first."
                 << endl;
            exit(-1);
        }
        return m_hog_img_flip;
    }

    void HOG_ADAPTER::extractHOGFeature(vector<float>* descriptors)
    {
        check_image();

        if (m_has_extracted)
        {
            if (descriptors)
            {
                vl_size total_dim = getHOGFeatureDim();
                descriptors->resize(total_dim);
                std::copy(m_hog_feature, m_hog_feature + total_dim,
                          descriptors->begin());
            }

            return;
        }

        set_gray_image_data();
        set_hog_model();
        vl_hog_put_image(m_hog_model, m_gray_data, m_org_img.cols,
                         m_org_img.rows, 1, getCellSize());

        vl_size total_dim = getHOGFeatureDim();

        //cerr << total_dim << endl;

        const vl_size sz = total_dim * sizeof(float);

        m_hog_feature = (float*) utils::mymalloc(sz);

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
            cerr << "image size: " << m_org_img.cols << " * " << m_org_img.rows
                 << endl;
            cerr << "hog size: " << getHOGXDim() << " * " << getHOGYDim()
                 << " * " << getHOGCellDim() << endl;
            cerr << endl << "extract HOG-feature done" << endl;

            /*
             for (int i = 0; i < sz / sizeof(float); ++i)
             if (i % 100 == 0)
             cerr << m_hog_feature[i] << " ";
             cerr << endl;*/
        }
    }

    void HOG_ADAPTER::extractHOGPatchFeature(const Rect* region,
                                             vector<float>* descriptors,
                                             Mat* hog_img)
    {
        check_image();

        assert(descriptors && _check_region(region));

        if (!m_has_extracted)
        {
            extractHOGFeature();
        }

        if (hog_img)
            visualizeHOGFeature();

        _extract_patch_feature_aux(region, m_hog_feature, descriptors,
                                   m_hog_img, hog_img);

    }

    void HOG_ADAPTER::_extract_patch_feature_aux(const Rect* region,
                                                 const float* feature,
                                                 vector<float>* descriptors,
                                                 const float* hog_img_data,
                                                 Mat* hog_img)
    {
        const int hog_x = region->x / getCellSize();
        const int hog_y = region->y / getCellSize();
        const int hog_w = std::ceil(1.0 * region->width / getCellSize());
        const int hog_h = std::ceil(1.0 * region->height / getCellSize());

        const vl_size hog_w_org = getHOGXDim();
        const vl_size hog_h_org = getHOGYDim();

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

    void HOG_ADAPTER::extractHOGFeatureFlip(vector<float>* descriptors)
    {
        check_image();

        if (m_has_extracted_flip)
            return;

        if (!m_has_extracted)
            extractHOGFeature(NULL);

        const vl_index* perm_idx = vl_hog_get_permutation(m_hog_model);
        const vl_size dim = getHOGCellDim();

        const vl_size hog_total_dim = getHOGFeatureDim();

        if (m_hog_feature_flip)
            free(m_hog_feature_flip);

        m_hog_feature_flip = (float*) utils::mymalloc(
                hog_total_dim * sizeof(float));

        const vl_size nx = getHOGXDim();
        const vl_size ny = getHOGYDim();
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

    void HOG_ADAPTER::extractHOGPatchFeatureFlip(const Rect* region,
                                                 vector<float>* descriptors,
                                                 Mat* hog_img_flip)
    {
        check_image();

        if (!(descriptors && _check_region(region)))
        {
            cerr << "Check the region and ensure it should be in the image area!"
                 << endl;
            exit(-1);
        }

        if (!m_has_extracted_flip)
            extractHOGFeatureFlip();

        if (hog_img_flip)
            visualizeHOGFeatureFlip();

        _extract_patch_feature_aux(region, m_hog_feature_flip, descriptors,
                                   m_hog_img_flip, hog_img_flip);
    }

    void HOG_ADAPTER::visualizeHOGFeature(Mat* hog_img)
    {
        check_image();

        if (!m_has_extracted)
            extractHOGFeature(NULL);

        _visualize_feature_aux(m_hog_feature, &m_hog_img, hog_img);
    }

    void HOG_ADAPTER::visualizeHOGFeatureFlip(Mat* hog_img_flip)
    {
        check_image();

        if (!m_has_extracted_flip)
            extractHOGFeatureFlip(NULL);

        _visualize_feature_aux(m_hog_feature_flip, &m_hog_img_flip,
                               hog_img_flip);
    }

    void HOG_ADAPTER::visualizeHOGPatchFeature(const Rect* region,
                                               Mat* hog_img_patch)
    {
        check_image();

        if (!_check_region(region))
        {
            cerr << "Check the region and ensure it should be in the image area!"
                 << endl;
            exit(-1);
        }

        if (!m_has_extracted)
            extractHOGFeature(NULL);

        if (!m_hog_img)
            visualizeHOGFeature(NULL);

        const int hog_x = region->x / getCellSize();
        const int hog_y = region->y / getCellSize();
        const int hog_w = std::ceil(1.0 * region->width / getCellSize());
        const int hog_h = std::ceil(1.0 * region->height / getCellSize());

        const vl_size hog_w_org = getHOGXDim();
        const vl_size hog_h_org = getHOGYDim();

        const vl_size glysz = vl_hog_get_glyph_size(m_hog_model);
        const int hog_i_w = hog_w * glysz;
        const int hog_i_h = hog_h * glysz;

        if (hog_img_patch->data)
            hog_img_patch->release();
        hog_img_patch->create(hog_i_w, hog_i_h, CV_32FC1);

        for (int ny = 0; ny < hog_i_h; ++ny)
        {
            float* p_patch = hog_img_patch->ptr<float>(ny);
            const float* p_org = m_hog_img
                    + (ny + hog_y * glysz) * hog_w_org * glysz;
            for (int nx = 0; nx < hog_i_w; ++nx)
            {
                *(p_patch + nx) = *(p_org + (nx + hog_x * glysz));
            }
        }
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

        vl_hog_render(m_hog_model, *hog_image_data, feature, getHOGXDim(),
                      getHOGYDim());

        // scale the original data to the range of [0, 255]
        float min_val((*hog_image_data)[0]), max_val((*hog_image_data)[0]);
        for (int i = 1; i < hog_img_width * hog_img_height; ++i)
        {
            if ((*hog_image_data)[i] < min_val)
                min_val = (*hog_image_data)[i];
            if ((*hog_image_data)[i] > max_val)
                max_val = (*hog_image_data)[i];
        }

        if (std::abs(max_val - min_val) > 0.000001)
        {
            float new_min_val(0), new_max_val(255);
            float k = (new_max_val - new_min_val) / (max_val - min_val);
            float b = new_min_val - k * min_val;
            for (int i = 0; i < hog_img_width * hog_img_height; ++i)
                (*hog_image_data)[i] = k * (*hog_image_data)[i] + b;
        }
        if (hog_image == NULL)
            return;

        if (hog_image->data)
            hog_image->release();
        hog_image->create(hog_img_height, hog_img_width, CV_32FC1);

        memcpy(hog_image->data, *hog_image_data, sz);

        if (__DEBUG_HOG_ADAPTER)
        {
            cerr << "render done" << endl;
        }
    }

    bool HOG_ADAPTER::_check_region(const Rect* region)
    {
        int m_img_width = m_org_img.cols;
        int m_img_height = m_org_img.rows;

        return (region && utils::within_scope(region->x, 0, m_img_width)
                && utils::within_scope(region->y, 0, m_img_height)
                && utils::within_scope(region->x + region->width - 1, 0,
                                       m_img_width)
                && utils::within_scope(region->y + region->height - 1, 0,
                                       m_img_height));

    }
}
