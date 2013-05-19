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

        template<typename T>
        void myfree(T** array)
        {
            free(*array);
            *array = NULL;
        }

        void* mymalloc(size_t sz);

        string myitoa(int num);
    }
}

#endif /* __VLFEAT_ADAPTER_UTILS_HPP__ */
