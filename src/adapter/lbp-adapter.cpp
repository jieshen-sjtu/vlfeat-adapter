/*
 * lbp-adapter.cpp
 *
 *  Created on: May 27, 2013
 *      Author: jieshen
 */

#include "lbp-adapter.hpp"
#include "../utils.hpp"

#include <iostream>
using std::cerr;
using std::endl;

namespace jieshen
{
    LBP_ADAPTER::LBP_ADAPTER()
    {
        init();
    }

    LBP_ADAPTER::LBP_ADAPTER(const Mat* img)
    {
        init();
        setImage(img);
    }

    LBP_ADAPTER::~LBP_ADAPTER()
    {
        clear();
    }

    void LBP_ADAPTER::init()
    {
        BASIC_ADAPTER::init();
        init_lbp_model();
    }

    void LBP_ADAPTER::init_lbp_model()
    {
        init_lbp_parameters();
        m_lbp_model = NULL;
        m_lbp_features = NULL;
        m_has_extracted = false;
    }

    void LBP_ADAPTER::init_lbp_parameters()
    {
        m_cell_size = DEFAULT_CELLSIZE_INVALID;
    }

    void LBP_ADAPTER::clear_lbp_model()
    {
        clear_model_related_data();
        init_lbp_parameters();
    }

    void LBP_ADAPTER::clear_model_related_data()
    {
        m_has_extracted = false;

        if (m_lbp_model)
        {
            vl_lbp_delete(m_lbp_model);
            m_lbp_model = NULL;
        }

        if (m_lbp_features)
            utils::myfree(&m_lbp_features);
    }

    void LBP_ADAPTER::setImage(const Mat* img)
    {
        img->copyTo(m_org_img);
        m_has_set_image = true;
        m_has_extracted = false;
    }

    void LBP_ADAPTER::setCellSize(const int cellsz)
    {
        if (cellsz <= 0)
        {
            cerr << "Cell Size must be bigger than zero" << endl;
            exit(-1);
        }
        if (cellsz == m_cell_size)
            return;

        m_cell_size = cellsz;
        m_has_extracted = false;
    }

    void LBP_ADAPTER::resetCellSize()
    {
        if (m_cell_size == DEFAULT_CELLSIZE_INVALID)
            return;

        m_cell_size = DEFAULT_CELLSIZE_INVALID;
        m_has_extracted = false;
    }

    int LBP_ADAPTER::getCellSize() const
    {
        if (m_cell_size == DEFAULT_CELLSIZE_INVALID)
            return DEFAULT_CELLSIZE;
        else
            return m_cell_size;
    }

    int LBP_ADAPTER::getLbpXDim() const
    {
        if (!m_has_set_image)
        {
            cerr << "Please set the image first" << endl;
            return 0;
        }
        return (m_org_img.cols / getCellSize());
    }

    int LBP_ADAPTER::getLbpYDim() const
    {
        if (!m_has_set_image)
        {
            cerr << "Please set the image first" << endl;
            return 0;
        }
        return (m_org_img.rows / getCellSize());
    }

    int LBP_ADAPTER::getLbpCellDim() const
    {
        if (!m_lbp_model)
        {
            cerr << "Please call extractLbpFeature() first" << endl;
            exit(-1);
        }

        return vl_lbp_get_dimension(m_lbp_model);
    }

    void LBP_ADAPTER::clearImage()
    {
        clear_gray_image_data();
        if (m_org_img.data)
            m_org_img.release();
        m_has_set_image = false;

        m_has_extracted = false;
    }

    void LBP_ADAPTER::resetLbpModel()
    {
        clear_lbp_model();
    }

    void LBP_ADAPTER::clear()
    {
        clear_lbp_model();
        clearImage();
    }

    void LBP_ADAPTER::set_lbp_model()
    {
        clear_model_related_data();

        m_lbp_model = vl_lbp_new(VlLbpUniform, false);
    }

    void LBP_ADAPTER::extractLbpFeature(vector<float>* descriptors)
    {
        check_image();

        if (m_has_extracted)
            return;

        set_gray_image_data();
        set_lbp_model();

        const int dim = getLbpFeatureDim();
        // cerr << dim << endl;
        m_lbp_features = (float*) utils::mymalloc(dim * sizeof(float));
        vl_lbp_process(m_lbp_model, m_lbp_features, m_gray_data, m_org_img.cols,
                       m_org_img.rows, getCellSize());

        if (descriptors)
        {
            descriptors->resize(dim, 0);
            copy(m_lbp_features, m_lbp_features + dim, descriptors->begin());
        }

        m_has_extracted = true;
    }

    void LBP_ADAPTER::extractLbpPatchFeature(const Rect* region,
                                             vector<float>* descriptors)
    {
        check_image();

        if (!m_has_extracted)
            extractLbpFeature();

        const int lbp_x = region->x / getCellSize();
        const int lbp_y = region->y / getCellSize();
        const int lbp_w = region->width / getCellSize();
        const int lbp_h = region->height / getCellSize();

        const int lbp_w_org = getLbpXDim();
        const int lbp_h_org = getLbpYDim();

        const int celldim = getLbpCellDim();

        const int sz = lbp_w * lbp_h * celldim;
        descriptors->resize(sz, 0.0);

        for (int nd = 0; nd < celldim; ++nd)
            for (int ny = 0; ny < lbp_h; ++ny)
            {
                const float* p_org = m_lbp_features + (ny + lbp_y) * lbp_w_org
                        + nd * lbp_w_org * lbp_h_org;
                vector<float>::iterator p_desc = descriptors->begin()
                        + ny * lbp_w + nd * lbp_w * lbp_h;

                for (int nx = 0; nx < lbp_w; ++nx)
                {
                    *(p_desc + nx) = *(p_org + lbp_x + nx);
                }
            }
    }

    int LBP_ADAPTER::getLbpFeatureDim() const
    {
        check_image();

        if (!m_lbp_model)
        {
            cerr << "Please call extractLbpFeature() first" << endl;
            return 0;
        }

        const int dim = getLbpXDim() * getLbpYDim() * getLbpCellDim();

        return dim;
    }

    const float* LBP_ADAPTER::getLbpFeature() const
    {
        check_image();

        if (!m_has_extracted)
        {
            cerr << "Please call extractLbpFeature() first" << endl;
            exit(-1);
        }

        return m_lbp_features;
    }

    string LBP_ADAPTER::info() const
    {
        string info = "=====LBP settings=====\n";

        info += "Cell Size: " + utils::myitoa(getCellSize()) + "\n";

        info += "\n-----Image Info-----\n";

        info += "Size:      " + utils::myitoa(m_org_img.cols) + " * "
                + utils::myitoa(m_org_img.rows) + "\n";

        return info;
    }

}
