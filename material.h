#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec,
                 color& attenuation, ray& scattered) const  {
                    return false;
        }
};


class lambertian : public material {
    private:
        color albedo;

    public:
        lambertian(const color& albedo) : albedo(albedo){}

        bool scatter(const ray& r_in, const hit_record& rec, 
                color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            // catch degenerate scatter direction.
            if (scatter_direction.near_zero()) scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    private:
        color albedo;

    public:
        metal(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, 
                color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return true;
        }
};

#endif