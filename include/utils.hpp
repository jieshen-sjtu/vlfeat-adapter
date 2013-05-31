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
#include <string>

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

        string myitoa(int num);
    }
}

#endif /* __VLFEAT_ADAPTER_UTILS_HPP__ */
