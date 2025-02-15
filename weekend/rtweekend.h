#ifndef RTWEEKEND_H
#define RTWEEKEND_H


#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>


using std::make_shared;
using std::shared_ptr;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535;

// utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi/180.0;
}

inline double random_double() {
    // returns a random real number in [0,1)
    return std::rand()/(RAND_MAX+1.0);
}

inline double random_double(double min, double max) {
    return min+(max-min)*random_double();
}

// common headers.
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif