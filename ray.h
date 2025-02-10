#ifndef RAY_H
#define RAY_H

#include "vec3.h"


/*
ray : P(t) = A + t*b;
where P is a 3D position along a line in 3D. A is the ray origin
and b is the ray direction. The ray parameter t is a real number.
P(t) moves the point along the ray. For negative t you can go
any where on the 3D line. For positive t, you can only get
only parts in front of A, called half-line or ray.
*/

class ray {
    private:
        point3 orig;
        vec3 dir;
    public:
        ray() {}
        ray(const point3& orig_, const vec3& dire_) :
            orig(orig_), dir(dire_){}

        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}

        // returns the position of ray after t intervals. 
        point3 at(double t) const {
            return orig + t*dir;
        }
};

#endif