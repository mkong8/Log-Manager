#ifndef GENFUNC
#define GENFUNC

#include <limits>
#include <cmath>

double calc_distance(const int x1, const int y1, const int x2, const int y2){
    return sqrt((double)(x2 - x1)*(x2 - x1) + (double)(y2 - y1)*(y2 - y1));
    // typcast as doubles to ensure no early rounding
}

#endif //GENFUNC