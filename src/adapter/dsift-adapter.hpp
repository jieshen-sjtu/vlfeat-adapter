/*
 * dsift.hpp
 *
 *  Created on: May 24, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/dsift.h>

#ifdef __cplusplus
}
#endif

#include "basic-adapter.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using std::vector;

namespace jieshen
{
    class DSIFT_Frame
    {
    public:
        DSIFT_Frame()
                : x(0), y(0), scale(0), norm(0)
        {
        }

        void clear()
        {
            x = 0;
            y = 0;
            scale = 0;
            norm = 0;
            descriptor.clear();
        }

    public:
        int x;
        int y;
        float scale;
        float norm;
        vector<float> descriptor;
    };

    class DSIFT_ADAPTER: public BASIC_ADAPTER
    {
    private:
        VlDsiftFilter* m_dsift_model;

    };
}

#endif /* __VLFEAT_ADAPTER_DSIFT_ADAPTER_HPP__ */
