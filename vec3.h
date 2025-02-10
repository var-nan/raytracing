#ifndef VEC3_H
#define VEC3_H


class vec3 {

public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0,e1,e2}{}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]); }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    double length() const {return std::sqrt(length_squared());}

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // generate random vectors. 
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const {
        // return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0])< s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }
};

using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}


inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}


inline vec3 random_unit_vector() {
    // find a random vector inside the sphere.
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        // 
        if (1e-160 < lensq && lensq <= 1) return p/sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_hemisphere = random_unit_vector();
    /* if the dot product of the nomal and the generated random vector
     * is positive, then generated random vector is in the correct 
     * hemisphere as the normal, else opposite hemisphere of normal.
     * invert the generated random vector 
     */
   
    if (dot(normal, on_unit_hemisphere) > 0.0) return on_unit_hemisphere;
    else return -on_unit_hemisphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    /* For the polished materials, the reflected ray is not randomly
     * scattered. The reflected ray direction is just v+2b. From the
     * book, 'n' is a unit normal to the surface. To get the vector
     * 'b', we scale the normal vector by length of projection of 
     * 'v' onto 'n', given by the dot(v,n).
     * 
     * Since 'v' points into the surface, and we want 'b' to point
     * out of the surface, we need to negate its projection length.

     */
    return v- 2*dot(v,n)*n; 
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()))*n;
    return r_out_perp + r_out_parallel;
}

#endif