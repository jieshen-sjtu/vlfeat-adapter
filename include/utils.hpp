/*
 * utils.hpp
 *
 *  Created on: May 16, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_UTILS_HPP__
#define __VLFEAT_ADAPTER_UTILS_HPP__

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include <string>

#include <opencv2/opencv.hpp>

using cv::Point;

using std::string;

namespace jieshen
{
    namespace utils
    {
        template<typename T>
        bool within_scope(const T& val, const T& low, const T& high)
        {
            if (val >= low && val < high)
                return true;
            return false;
        }

        void* mymalloc(size_t sz);

        template<typename T>
        void myfree(T** array)
        {
            free(*array);
            *array = NULL;
        }

        template<typename T>
        T** mymalloc_matrix(const int row, const int col)
        {
            T** result = (T**) malloc(row * sizeof(T*));
            assert(result);
            for (int i = 0; i < row; ++i)
                result[i] = (T*) mymalloc(col * sizeof(T));
            return result;
        }

        template<typename T>
        void myfree_matrix(T*** array, const int row)
        {
            for (int i = 0; i < row; ++i)
                if ((*array)[i])
                    myfree((*array) + i);
            *array = NULL;
        }

        template<typename T>
        T square(const T& x)
        {
            return (x * x);
        }

        template<typename T>
        double euclid_dist(const T& x1, const T& y1, const T& x2, const T& y2)
        {
            T x_diff = x2 - x1;
            T y_diff = y2 - y1;
            return std::sqrt(square(x_diff) + square(y_diff));
        }

        string myitoa(int num);
        float get_rot_rad(const Point& start, const Point& end);
        float get_rot_deg(const Point& start, const Point& end);
    }
}

#endif /* __VLFEAT_ADAPTER_UTILS_HPP__ */
