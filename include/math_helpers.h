#pragma once
#include <cstdlib>

float randomFloat()
{
    return (float) rand() / RAND_MAX;
}

int randomInt(int left, int right) //random integer from range [left, right)
{
    int val = left + (right - left) * randomFloat();
    if(val == right && left != right) //practically impossible BUT ...
        val -= 1;
    return val;
}