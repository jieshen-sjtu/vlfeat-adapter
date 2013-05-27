/*
 * dsift-adapter.cpp
 *
 *  Created on: May 24, 2013
 *      Author: jieshen
 */

#include "dsift-adapter.hpp"
#include "../utils.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/imopv.h>

#ifdef __cplusplus
}
#endif

#include <iostream>
using std::cerr;
using std::endl;

namespace jieshen
{
    const int DSIFT_ADAPTER::DEFAULT_REGION[REGION_ARRAY_SIZE] = { 0, 0, -1, -1 };
    const int DSIFT_ADAPTER::DEFAULT_STEP[STEP_ARRAY_SIZE] = { 5, 5 };
    const int DSIFT_ADAPTER::DEFAULT_STEP_INVALID[STEP_ARRAY_SIZE] = { 0, 0 };
    const int DSIFT_ADAPTER::DEFAULT_BIN_SIZE[BIN_SIZE_ARRAY_SIZE] = { 5, 5 };
    const int DSIFT_ADAPTER::DEFAULT_BIN_SIZE_INVALID[BIN_SIZE_ARRAY_SIZE] = {
                    0, 0 };

    DSIFT_ADAPTER::DSIFT_ADAPTER()
    {
        init();
    }

    DSIFT_ADAPTER::DSIFT_ADAPTER(const Mat* img)
    {
        init();
        setImage(img);
    }

    DSIFT_ADAPTER::~DSIFT_ADAPTER()
    {
        clear();
    }

    void DSIFT_ADAPTER::init()
    {
        BASIC_ADAPTER::init();
        init_dsift_model();
    }

    void DSIFT_ADAPTER::setImage(const Mat* img)
    {
        img->copyTo(m_org_img);
        m_has_set_image = true;
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::clear()
    {
        clear_dsift_model();
        clearImage();
    }

    void DSIFT_ADAPTER::clear_dsift_model()
    {
        clear_model_related_data();
        init_dsift_parameters();
    }

    void DSIFT_ADAPTER::clearImage()
    {
        clear_gray_image_data();
        if (m_org_img.data)
            m_org_img.release();
        m_has_set_image = false;

        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::init_dsift_model()
    {
        init_dsift_parameters();
        m_dsift_model = NULL;
        m_has_extracted = false;

        m_frames.reserve(1024);
    }

    void DSIFT_ADAPTER::init_dsift_parameters()
    {
        memcpy(m_step, DEFAULT_STEP_INVALID, STEP_ARRAY_SIZE * sizeof(int));

        memcpy(m_region, DEFAULT_REGION, REGION_ARRAY_SIZE * sizeof(int));

        memcpy(m_bin_size, DEFAULT_BIN_SIZE_INVALID,
               BIN_SIZE_ARRAY_SIZE * sizeof(int));

        m_win_size = DEFAULT_WIN_SIZE_INVALID;

        m_use_flat_win = DEFAULT_FLAT_WINDOW;
    }

    void DSIFT_ADAPTER::clear_model_related_data()
    {
        if (m_dsift_model)
        {
            vl_dsift_delete(m_dsift_model);
            m_dsift_model = NULL;
        }

        for (size_t i = 0; i < m_frames.size(); ++i)
            m_frames[i].clear();
        m_frames.clear();

        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::setStep(const int* step)
    {
        if (step[0] <= 0 || step[1] <= 0)
        {
            cerr << "step should be a positive integer" << endl;
            exit(-1);
        }

        if (_is_same_array(m_step, step, STEP_ARRAY_SIZE))
            return;

        memcpy(m_step, step, STEP_ARRAY_SIZE * sizeof(int));
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::setRegion(const int* region)
    {
        if (!(utils::within_scope(region[0], 0, region[2])
                && utils::within_scope(region[1], 0, region[3])
                && region[2] < m_org_img.cols) && region[3] < m_org_img.rows)
        {
            cerr << "Region not inside the image" << endl;
            exit(-1);
        }

        if (_is_same_array(m_region, region, REGION_ARRAY_SIZE))
            return;

        memcpy(m_region, region, REGION_ARRAY_SIZE * sizeof(int));

        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::setBinSize(const int* bin_sz)
    {
        if (bin_sz[0] <= 0 || bin_sz[1] <= 0)
        {
            cerr << "Bin Size should be positive" << endl;
            exit(-1);
        }

        if (_is_same_array(m_bin_size, bin_sz, BIN_SIZE_ARRAY_SIZE))
            return;

        memcpy(m_bin_size, bin_sz, BIN_SIZE_ARRAY_SIZE * sizeof(int));
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::setWindowSize(const double win_sz)
    {
        if (win_sz <= 0)
        {
            cerr << "Gaussian window size should be positive" << endl;
            exit(-1);
        }

        if (std::abs(win_sz - m_win_size) < __DSIFT_ADAPTER_EPS)
            return;

        m_win_size = win_sz;
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::setUseFlatWindow(bool use)
    {
        if (use == m_use_flat_win)
            return;

        m_use_flat_win = use;
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::resetStep()
    {
        if (_is_same_array(m_step, DEFAULT_STEP_INVALID, STEP_ARRAY_SIZE))
            return;

        memcpy(m_step, DEFAULT_STEP_INVALID, STEP_ARRAY_SIZE * sizeof(int));
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::resetRegion()
    {
        if (_is_same_array(m_region, DEFAULT_REGION, REGION_ARRAY_SIZE))
            return;

        memcpy(m_region, DEFAULT_REGION, REGION_ARRAY_SIZE * sizeof(int));
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::resetBinSize()
    {
        if (_is_same_array(m_bin_size, DEFAULT_BIN_SIZE_INVALID,
                           BIN_SIZE_ARRAY_SIZE))
            return;

        memcpy(m_bin_size, DEFAULT_BIN_SIZE_INVALID,
               BIN_SIZE_ARRAY_SIZE * sizeof(int));
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::resetWindowSize()
    {
        if (std::abs(
                m_win_size - DEFAULT_WIN_SIZE_INVALID) < __DSIFT_ADAPTER_EPS)
            return;
        m_win_size = DEFAULT_WIN_SIZE_INVALID;
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::resetUseFlatWindow()
    {
        if (m_use_flat_win == DEFAULT_FLAT_WINDOW)
            return;
        m_use_flat_win = DEFAULT_FLAT_WINDOW;
        m_has_extracted = false;
    }

    void DSIFT_ADAPTER::getStep(vector<int>& step) const
    {
        step.resize(STEP_ARRAY_SIZE, 0);
        if (m_has_extracted)
        {
            vl_dsift_get_steps(m_dsift_model, &step[0], &step[1]);
            return;
        }

        if (_is_same_array(m_step, DEFAULT_STEP_INVALID, STEP_ARRAY_SIZE))
            copy(DEFAULT_STEP, DEFAULT_STEP + STEP_ARRAY_SIZE, step.begin());
        else
            copy(m_step, m_step + STEP_ARRAY_SIZE, step.begin());
    }

    void DSIFT_ADAPTER::getRegion(vector<int>& region) const
    {
        region.resize(REGION_ARRAY_SIZE, 0);

        if (m_has_extracted)
        {
            vl_dsift_get_bounds(m_dsift_model, &region[0], &region[1],
                                &region[2], &region[3]);
            return;
        }

        copy(m_region, m_region + REGION_ARRAY_SIZE, region.begin());
    }

    void DSIFT_ADAPTER::getBinSize(vector<int>& bin_sz) const
    {
        bin_sz.resize(BIN_SIZE_ARRAY_SIZE, 0);

        if (m_has_extracted)
        {
            const VlDsiftDescriptorGeometry* geom = vl_dsift_get_geometry(
                    m_dsift_model);
            bin_sz[0] = geom->binSizeX;
            bin_sz[1] = geom->binSizeY;
            return;
        }

        if (_is_same_array(m_bin_size, DEFAULT_BIN_SIZE_INVALID,
                           BIN_SIZE_ARRAY_SIZE))
            copy(DEFAULT_BIN_SIZE, DEFAULT_BIN_SIZE + BIN_SIZE_ARRAY_SIZE,
                 bin_sz.begin());
        else
            copy(m_bin_size, m_bin_size + BIN_SIZE_ARRAY_SIZE, bin_sz.begin());
    }

    double DSIFT_ADAPTER::getWindowSize() const
    {
        if (m_has_extracted)
            return vl_dsift_get_window_size(m_dsift_model);

        if (std::abs(
                m_win_size - DEFAULT_WIN_SIZE_INVALID) < __DSIFT_ADAPTER_EPS)
            return DEFAULT_WIN_SIZE;
        return m_win_size;
    }

    bool DSIFT_ADAPTER::getWindowType() const
    {
        if (m_has_extracted)
            return vl_dsift_get_flat_window(m_dsift_model);
        return m_use_flat_win;
    }

    void DSIFT_ADAPTER::set_dsift_model()
    {
        clear_model_related_data();

        m_dsift_model = vl_dsift_new(m_org_img.cols, m_org_img.rows);

        vector<int> step;
        getStep(step);
        vl_dsift_set_steps(m_dsift_model, step[0], step[1]);

        vector<int> _region;
        getRegion(_region);
        int region[REGION_ARRAY_SIZE];
        copy(_region.begin(), _region.end(), region);
        if (!_is_same_array(region, DEFAULT_REGION, REGION_ARRAY_SIZE))
            vl_dsift_set_bounds(m_dsift_model, region[0], region[1], region[2],
                                region[3]);

        vector<int> _bin_sz;
        getBinSize(_bin_sz);
        int bin_sz[BIN_SIZE_ARRAY_SIZE];
        copy(_bin_sz.begin(), _bin_sz.end(), bin_sz);
        if (!_is_same_array(bin_sz, DEFAULT_BIN_SIZE, BIN_SIZE_ARRAY_SIZE))
        {
            VlDsiftDescriptorGeometry geom;
            geom.binSizeX = bin_sz[0];
            geom.binSizeY = bin_sz[1];
            geom.numBinT = DEFAULT_NUM_BINT;
            geom.numBinX = DEFAULT_NUM_BINX;
            geom.numBinY = DEFAULT_NUM_BINY;
            vl_dsift_set_geometry(m_dsift_model, &geom);
        }

        vl_dsift_set_flat_window(m_dsift_model, getWindowType());
        vl_dsift_set_window_size(m_dsift_model, getWindowSize());
    }

    void DSIFT_ADAPTER::resetDSiftModel()
    {
        clear_dsift_model();
    }

    void DSIFT_ADAPTER::extractDSiftFeature()
    {
        check_image();

        if (m_has_extracted)
            return;

        set_gray_image_data();
        set_dsift_model();

        vl_dsift_process(m_dsift_model, m_gray_data);

        m_has_extracted = true;

        const int num_key_pts = vl_dsift_get_keypoint_num(m_dsift_model);
        const VlDsiftKeypoint* key_points = vl_dsift_get_keypoints(
                m_dsift_model);
        const int feature_dim = vl_dsift_get_descriptor_size(m_dsift_model);
        const float* features = vl_dsift_get_descriptors(m_dsift_model);

        m_frames.resize(num_key_pts);
        for (size_t i = 0; i < num_key_pts; ++i)
        {
            DSIFT_Frame& frame = m_frames[i];
            const VlDsiftKeypoint* cur_key_pt = key_points + i;
            const float* cur_feature = features + feature_dim * i;

            frame.x = cur_key_pt->x;
            frame.y = cur_key_pt->y;
            frame.scale = cur_key_pt->s;
            frame.norm = cur_key_pt->norm;

            frame.descriptor.resize(feature_dim, 0.0);

            for (size_t j = 0; j < feature_dim; ++j)
                frame.descriptor[j] = VL_MIN(512.0*cur_feature[j], 255.0);
        }
    }

    void DSIFT_ADAPTER::set_gray_image_data()
    {
        BASIC_ADAPTER::set_gray_image_data();
        vector<int> bin_sz;
        getBinSize(bin_sz);
        double sigmaX = std::sqrt(
                utils::square(bin_sz[0] * 1.0 / DEFAULT_MAGNIF) - 0.25);
        double sigmaY = std::sqrt(
                utils::square(bin_sz[1] * 1.0 / DEFAULT_MAGNIF) - 0.25);

        //std::cout << sigmaX << " " << sigmaY << endl;

        /*
         const int smoothStride = m_org_img.cols;
         const int stride = 1;
         float* smoothedImage = (float*) utils::mymalloc(
         (m_org_img.cols ) * m_org_img.rows
         * sizeof(float));
         vl_imsmooth_f(smoothedImage, smoothStride, m_gray_data, m_org_img.cols,
         m_org_img.rows, stride, sigmaX, sigmaY);

         for (int y = 0; y < m_org_img.rows; ++y)
         {
         float* line = smoothedImage + y * m_org_img.cols;
         for (int x = 0; x < m_org_img.cols; ++x)
         std::cout << line[x] << " ";
         std::cout << endl;
         }*/

        const int sz = m_org_img.rows * m_org_img.cols * sizeof(float);
        Mat gray_img;
        gray_img.create(m_org_img.rows, m_org_img.cols, CV_32FC1);
        memcpy(gray_img.data, m_gray_data, sz);
        Mat smoothImage;
        cv::GaussianBlur(gray_img, smoothImage, cv::Size(0, 0), sigmaX, sigmaY,
                         cv::BORDER_REPLICATE);

        //std::cout << smoothImage << endl;

        memcpy(m_gray_data, smoothImage.data, sz);

    }

    int DSIFT_ADAPTER::getNumKeyPoints() const
    {
        if (!m_has_extracted)
        {
            cerr << "Please call extractDSiftFeature() first" << endl;
            return 0;
        }
        return vl_dsift_get_keypoint_num(m_dsift_model);
    }

    int DSIFT_ADAPTER::getDSiftFeatureDim() const
    {
        if (!m_has_extracted)
        {
            cerr << "Please call extractDSiftFeature() first" << endl;
            return 0;
        }

        return vl_dsift_get_descriptor_size(m_dsift_model);
    }

    const vector<DSIFT_Frame>& DSIFT_ADAPTER::getAllFrames() const
    {
        if (!m_has_extracted)
        {
            cerr << "There is no dense-sift frame. Please call extractDSiftFeature() first"
                 << endl;
            exit(-1);
        }

        return m_frames;
    }

    string DSIFT_ADAPTER::info() const
    {
        string info = "=====Dense Sift settings=====\n";

        vector<int> step;
        getStep(step);
        info += "Step:     ";
        for (size_t i = 0; i < step.size(); ++i)
            info += utils::myitoa(step[i]) + " ";
        info += "\n";

        vector<int> bin_size;
        getBinSize(bin_size);
        info += "Bin Size: ";
        for (size_t i = 0; i < bin_size.size(); ++i)
            info += utils::myitoa(bin_size[i]) + " ";
        info += "\n";

        vector<int> region;
        getRegion(region);
        info += "Region:   ";
        for (size_t i = 0; i < region.size(); ++i)
            info += utils::myitoa(region[i]) + " ";
        info += "\n";

        info += "Gaussian Window Size: " + utils::myitoa(getWindowSize())
                + "\n";

        info += "Use Flat Window:      "
                + string(getWindowType() ? "Yes" : "No") + "\n";

        info += "\n-----Image Info-----\n";

        info += "Size:     " + utils::myitoa(m_org_img.cols) + " * "
                + utils::myitoa(m_org_img.rows) + "\n";

        return info;
    }

    bool _is_same_array(const int* a, const int* b, const int sz)
    {
        bool same = true;
        for (int i = 0; same && i < sz; ++i)
            if (a[i] != b[i])
                same = false;

        return same;

    }
}
