#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include <algorithm>



// double hit_sphere(const point3& center, double radius, const ray& r) {
//     /*
//        Given a center of sphere, the sphere equation is 
//        x^2 + y^2 + z^2 = r^2;
//        Given a point (x,y,z), if it satisfies the above equality, then
//        the point is on the sphere, if  x^2 + y^2 + z^2 < r^2, the point 
//        lies inside the sphere, if x^2 + y^2 + z^2 > r^2, the point lies
//        outside the sphere.  
//     */
//     auto a = dot(r.direction(), r.direction());
//     // vec3 oc = center - r.origin();
//     vec3 oc = (center - r.origin()); // ray from ray's origin to sphere center.
//     auto h = dot(r.direction(), oc);
//     auto b = -2.0 * dot(r.direction(), oc);
//     auto c = dot( oc, oc) - radius*radius;
//     auto discriminant = h*h -   a*c;

//     if (discriminant < 0) // ray do not touch sphere surface. no real solutions.
//         return -1.0;
//     else return (h - std::sqrt(discriminant))/(a);

// }

// color ray_color(const ray& r) {
//     // if t > 0, the ray touches the surface of sphere atleast one point. 
//     // t is the number of intervals at which the ray touches the sphere. 
//     auto t = hit_sphere(point3(0,0,-1), 0.5, r);
//     if (t > 0.0){
//         vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
//         return 0.5 *color(N.x()+1, N.y()+1, N.z()+1);
//     }

//     /*
//         Gradient. this function will linearly blend white and blue depending on
//         the height of the y coordinate after scaling the ray direction to unit
//         length (so -1.0 < y < 1.0).
        
//         blended value = (1-a) * startValue + a*endValue;
//     */
//     vec3 unit_direction = unit_vector(r.direction());
//     // the 1.0 below is for horizontal gradient?
//     auto a = 0.5 * (unit_direction.y() + 1.0);
//     return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
// }



int main(){

    // world
    hittable_list world;


    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++){
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center- point3(4,0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5,1);
                    auto fuzz = random_double(0,0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(.04, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1,0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4,1,0), 1.0, material3));


    // auto R = std::cos(pi/4);

    // // auto material_left = make_shared<lambertian>(color(0,0,1));
    // // auto material_right = make_shared<lambertian>(color(1,0,0));

    // // world.add(make_shared<sphere>(point3(-R, 0,-1), R, material_left));
    // // world.add(make_shared<sphere>(point3( R, 0,-1), R, material_right));

    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8,0.0));
    // auto material_center = make_shared<lambertian>(color(0.1,0.2,0.5));
    // auto material_left = make_shared<dielectric>(1.5);
    // auto material_bubble = make_shared<dielectric>(1.0/1.5);
    // auto material_right = make_shared<metal>(color(0.8,0.6,0.2), 1.0);

    // world.add(make_shared<sphere>(point3(0,-100.5,-1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3(0, 0.0, -1.2), 0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,0.0,-1.0), 0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}