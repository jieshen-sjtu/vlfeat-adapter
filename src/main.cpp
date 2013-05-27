/*
 * main.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: jieshen
 */

#include "test.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    int selection(0);
    cerr << "Run demo" << endl;
    cerr << "1. HOG" << endl;
    cerr << "2. SIFT" << endl;
    cerr << "3. GIST" << endl;
    cerr << "4. Dense SIFT" << endl;
    cerr << "5. LBP" << endl;

    cin >> selection;

    switch (selection)
    {
        case 1:
            test_hog(argc, argv);
            break;
        case 2:
            test_sift(argc, argv);
            break;
        case 3:
            test_gist(argc, argv);
            break;
        case 4:
            test_dsift(argc, argv);
            break;
        case 5:
            test_lbp(argc, argv);
            break;
        default:
            cout << "Are you kidding?!" << endl;
            return -1;
    }

    return 0;
}
