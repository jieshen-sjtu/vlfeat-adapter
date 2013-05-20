/*
 * utils.cpp
 *
 *  Created on: May 19, 2013
 *      Author: jieshen
 */

#include "utils.hpp"
#include <sstream>

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
            int max_length = 100;
            char *str = (char *) mymalloc(max_length);

            sprintf(str, "%d", num);

            string res(str);

            myfree(&str);

            return res;
        }
    }
}
