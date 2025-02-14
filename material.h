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
        double fuzz; // fuzz reflection factor, 
        /* this fuzz factor is used to generate a small sphere centered on the 
         * original endpoint of the reflected ray, scaled by fuzz factor.  
         * For the bigger fuzz sphere, the rays might scatter below the surface.
         * and the surfact can absorb those. 
        */

    public:
        metal(const color& albedo, double fuzz_) : albedo(albedo), fuzz(std::fmin(1,fuzz_)) {}

        bool scatter(const ray& r_in, const hit_record& rec, 
                color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};


class dielectric: public material {
    private:
        double refraction_index;

        static double reflectance(double cosine, double ri) {
            // use schlick's approximation for reflectance.
            auto r0 = (1-ri)/(1+ri);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1-cosine), 5);
        }

    
    public:
        dielectric(double refraction_index_) : refraction_index(refraction_index_) {}

        bool scatter(const ray& r_in, const hit_record& rec,
                color& attenuation, ray& scattered) const override {
            
            attenuation = color(1.0,1.0,1.0); // does it mean how much light is reflected?
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;
            
            vec3 unit_direction = unit_vector(r_in.direction());
            // total internal reflection. 
            /*
                    Some rays doesn't have solution with snell's law.  
                    sin theta' = eta/eta' * sin theta

                    if (ray * sin_theta > 1.0) // ray doesn't have valid solution, must reflect.  
                    else // ray have valid solution, ray can refract.
            */
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || (reflectance(cos_theta, ri)>  ri)) direction = reflect(unit_direction, rec.normal);
            else direction = refract(unit_direction, rec.normal, ri);
            // vec3 refracted = refract(unit_direction, rec.normal, ri);
            scattered = ray(rec.p, direction);
            return true;
        }
};

#endif