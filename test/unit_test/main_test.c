//
// Created by snipy on 21.03.18.
//

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "../lib/ctest/ctest.h"

int main(int argc, const char** argv)
{
    int result = ctest_main(argc, argv);
    return result;
}