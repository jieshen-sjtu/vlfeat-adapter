/*
 * utils.hpp
 *
 *  Created on: May 16, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_UTILS_HPP__
#define __VLFEAT_ADAPTER_UTILS_HPP__

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
    }
}

#endif /* __VLFEAT_ADAPTER_UTILS_HPP__ */
