#ifndef CAMERA_H
#define CAMERA_H

/*
    Two purposes of camera class. 
        1. construct and dispatch rays into the world.  
        2. use the results of these rays to construct the rendered image. 

*/

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

class camera {
    public:

        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10; // random samples per each pixel.
        int max_depth = 10; // maximum number of ray bounces into scene.

        double vfov = 90;
        point3 lookfrom = point3(0,0,0); // point camera is looking from.
        point3 lookat = point3(0,0,-1); // point camera is looking at
        point3 vup = vec3(0,1,0); // camera-relative up direction.

        double defocus_angle = 0; // variation angle of rays through each pixel.
        double focus_dist = 10; // distance from camera lookfrom point to plane of perfect focus.

        void render(const hittable& world) {
            
            initialize();

            // render code.
            std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

            /* iterate over all the pixels in the image, compute color for each ray from camera center to pixel_center in image. */

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height -j) << ' ' << std::flush;

                for (int i = 0;i < image_width; i++){
                    
                    // sample some rays around this pixel, and average the colors returned by all samples.
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i,j);
                        pixel_color += ray_color(r, max_depth, world);
                    }

                    // auto pixel_center = pixel00_loc + (i*pixel_delta_u) + (j*pixel_delta_v);
                    // auto ray_direction = pixel_center - camera_center;
                    // ray r(camera_center, ray_direction);

                    // color pixel_color = ray_color(r, world); 
                    write_color(std::cout, pixel_samples_scale* pixel_color);
                }
            }
            std::clog << "\rDone.           \n";



        }

    private:

        int image_height; // rendered image height
        point3 camera_center; 
        point3 pixel00_loc; // location of pixel 0,0
        vec3 pixel_delta_u; // offset to pixel to the right.
        vec3 pixel_delta_v; // offset to pixel below.
        double pixel_samples_scale; // color scale factor for a sum of pixel samples.
        vec3 u,v,w; // camera frame basis vectors.

        vec3 defocus_disk_u; // defocus disk horizontal radius.
        vec3 defocus_disk_v; // defocus disk vertical radius.

        void initialize() {
            // initialize the camera here.

            image_height = int(std::max(int(image_width/aspect_ratio),1));
            // image_height = int(image_width/aspect_ratio);
            // image_height = (image_height < 1) ? 1 : image_height;
            // std::cout << "Image height: " << image_height << std::endl;

            pixel_samples_scale = 1.0/samples_per_pixel;

            camera_center = point3(0,0,0);
            
            camera_center = lookfrom;
            // viewport dimensions.
            // auto focal_length = (lookfrom -lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2*h*focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            w = unit_vector(lookfrom-lookat);
            u = unit_vector(cross(vup,w));
            v = cross(w,u);

            // vectors accross the horizontal and down the vertical viewport edges.
            auto viewport_u = viewport_width*u;
            auto viewport_v = viewport_height*-v;

            // horizontal and vertical delta vectors
            pixel_delta_u = viewport_u/image_width;
            pixel_delta_v = viewport_v/image_height;

            // viewport upper left is the position left most corner of the viewport
            auto viewport_upper_left = camera_center - (focus_dist*w) 
                    - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v)*0.5;

            // camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        vec3 sample_square() const {
            // return a vector to a random point in the [-0.5,-0.5] - [0.5, 0.5] unit square.
            return vec3(random_double()-0.5, random_double()-0.5, 0);
        }

        ray get_ray(int i, int j) const {
            // construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i,j.

            auto offset = sample_square(); // get a random ray from the unit square.
            auto pixel_sample = pixel00_loc + ((i+offset.x()) * pixel_delta_u) 
                        + ((j+offset.y()) * pixel_delta_v); // actual pixel location w.r.t this offset.

            auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            return ray(ray_origin, ray_direction);
            
        }

        color ray_color (const ray& r, int depth, const hittable& world) {

            if (depth <= 0) return color(0,0,0); // if we've exceeded ray bounce limit, no more light is gathered.

            hit_record rec;

            // if the ray hits any objects in the world.
            /* the reason for 0.001 in the interval is still not understood, related to some shadow acne.*/
            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0,0,0);
                // // vec3 direction = random_on_hemisphere(rec.normal);
                // vec3 direction = rec.normal + random_unit_vector();
                // // return 0.5 * (rec.normal + color(1,1,1));
                // return 0.5 * ray_color(ray(rec.p, direction), depth-1, world); // return only 50% of the color of ray from a bounce. 
            }

            // ray doesn't hit any objects, return the color according
            // to the gradient.

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 -a)* color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0);
        }

        point3 defocus_disk_sample() const {
            // returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return camera_center + 
                    (p[0] * defocus_disk_u) + 
                    (p[1] * defocus_disk_v);
        }
};


#endif