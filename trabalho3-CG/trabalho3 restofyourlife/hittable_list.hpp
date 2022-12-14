#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtweekend.hpp"
#include "hittable.hpp"
#include "aabb.hpp"

#include <memory>
#include <vector>

class hittable_list : public hittable  {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const ray& r, double tmin, double tmax, hit_record& rec) const override;

        virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;
        virtual double pdf_value(const vec3 &o, const vec3 &v) const override;
        virtual vec3 random(const vec3 &o) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


bool hittable_list::bounding_box(double t0, double t1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_true = objects[0]->bounding_box(t0, t1, temp_box);

    if (!first_true)
        return false;

    output_box = temp_box;

    for (const auto& object : objects) {
        if (!object->bounding_box(t0, t1, temp_box))
            return false;
        output_box = surrounding_box(output_box, temp_box);
    }

    return true;
}


double hittable_list::pdf_value(const point3& o, const vec3& v) const {
    auto weight = 1.0/objects.size();
    auto sum = 0.0;

    for (const auto& object : objects)
        sum += weight * object->pdf_value(o, v);

    return sum;
}


vec3 hittable_list::random(const vec3 &o) const {
    auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size-1)]->random(o);
}

#endif