#ifndef HITTABLE_H
#define HITTABLE_H


class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            /*
                The normals always point against the ray. If the ray and the 
                outward normal are in same direction, their dot product is
                positive. The the ray is expected to present inside the sphere.

                if the dot product is negative, then ray and outward normal 
                are in opposite direction. ray is outside of the sphere.  
            */
            // sets the hit record normal vector.
            // NOTE: the parameter 'outward normal' is assumed to have unit length.

            front_face = dot(r.direction(), outward_normal) < 0; // both
            normal = front_face ? outward_normal : -1.0*outward_normal; // 
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif