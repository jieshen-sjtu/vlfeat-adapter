/*
 * utils.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "utils.hpp"

#include <iostream>
using std::cout;
using std::endl;

namespace jieshen
{
    namespace utils
    {
        void* mymalloc(size_t sz)
        {
            void* result = malloc(sz);
            assert(result);
            memset(result, 0, sz);
            return result;
        }

        string myitoa(int num)
        {
            char str[100];

            sprintf(str, "%d", num);

            string res(str);

            return res;
        }
    }
}
