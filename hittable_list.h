#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {

    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list(){}

        hittable_list(shared_ptr<hittable> object) {add(object);}

        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        void clear() { objects.clear();}

        bool hit (const ray& r, interval ray_t, hit_record &rec) const override {
            auto closest_so_far = ray_t.max;
            bool hit_anything = false;
            hit_record temp_rec;


            // find a closest object that this ray can hit among all the objects. 

            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;

        }

};

#endif