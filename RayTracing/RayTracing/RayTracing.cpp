﻿// RayTracing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <iostream>
#include "moving_sphere.h"
#include "texture.h"
#include "perlin.h"
#include "aabb.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

//double hit_sphere(const point3& center, double radius, const ray& r) {
//    vec3 oc = r.origin() - center;
//    auto a = r.direction().length_squared();
//    auto half_b = dot(oc, r.direction());
//    auto c = oc.length_squared() - radius * radius;
//    auto discriminant = half_b * half_b - a * c;
//    if (discriminant < 0) {
//        return -1.0;
//    }
//    else {
//        return (-half_b - sqrt(discriminant)) / a;
//    }
//}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
        ));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("C:/RayTracingPractice/RayTracing/Debug/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vec3(-100, 270, 395)
        )
    );

    return objects;
}

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    //objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
    //objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);

    return objects;
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("C:/RayTracingPractice/RayTracing/Debug/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list random_scene() {
    hittable_list world;

    //auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    //world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
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
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);
    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

    //if (world.hit(r, 0.001, infinity, rec)) {
    //    ray scattered;
    //    color attenuation;
    //    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    //        return attenuation * ray_color(scattered, world, depth - 1);
    //    return color(0, 0, 0);
    //}
    //vec3 unit_direction = unit_vector(r.direction());
    //auto t = 0.5 * (unit_direction.y() + 1.0);
    //return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

//int main()
//{
//    // Image
//    /*const*/ auto aspect_ratio = 16.0 / 9.0;
//    /*const*/ int image_width = 400;
//    //const int image_height = static_cast<int>(image_width / aspect_ratio);
//    /*const*/ int samples_per_pixel = 100;
//    const int max_depth = 50;
//
//    // World
//    hittable_list world;
//    point3 lookfrom;
//    point3 lookat;
//    auto vfov = 40.0;
//    auto aperture = 0.0;
//    color background(0, 0, 0);
//
//    switch (0) {
//    case 1:
//        world = random_scene();
//        background = color(0.70, 0.80, 1.00);
//        lookfrom = point3(13, 2, 3);
//        lookat = point3(0, 0, 0);
//        vfov = 20.0;
//        aperture = 0.1;
//        break;
//
//    //default:
//    case 2:
//        world = two_spheres();
//        background = color(0.70, 0.80, 1.00);
//        lookfrom = point3(13, 2, 3);
//        lookat = point3(0, 0, 0);
//        vfov = 20.0;
//        break;
//    case 3:
//        world = two_perlin_spheres();
//        background = color(0.70, 0.80, 1.00);
//        lookfrom = point3(13, 2, 3);
//        lookat = point3(0, 0, 0);
//        vfov = 20.0;
//        break;
//    case 4:
//        world = earth();
//        background = color(0.70, 0.80, 1.00);
//        lookfrom = point3(13, 2, 3);
//        lookat = point3(0, 0, 0);
//        vfov = 20.0;
//        break;
//    case 5:
//        world = simple_light();
//        samples_per_pixel = 400;
//        background = color(0, 0, 0);
//        lookfrom = point3(26, 3, 6);
//        lookat = point3(0, 2, 0);
//        vfov = 20.0;
//        break;
//    case 6:
//        world = cornell_box();
//        aspect_ratio = 1.0;
//        image_width = 600;
//        samples_per_pixel = 200;
//        background = color(0, 0, 0);
//        lookfrom = point3(278, 278, -800);
//        lookat = point3(278, 278, 0);
//        vfov = 40.0;
//        break;
//    case 7:
//        world = cornell_smoke();
//        aspect_ratio = 1.0;
//        image_width = 600;
//        samples_per_pixel = 200;
//        lookfrom = point3(278, 278, -800);
//        lookat = point3(278, 278, 0);
//        vfov = 40.0;
//        break;
//    default:
//    case 8:
//        world = final_scene();
//        aspect_ratio = 1.0;
//        image_width = 800;
//        samples_per_pixel = 10000;
//        background = color(0, 0, 0);
//        lookfrom = point3(478, 278, -600);
//        lookat = point3(278, 278, 0);
//        vfov = 40.0;
//        break;
//    }
//
//    //auto world = random_scene();
//
//    //hittable_list world;
//
//    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
//    ////auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
//    ////auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
//    ////auto material_center = make_shared<dielectric>(1.5);
//    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
//    //auto material_left = make_shared<dielectric>(1.5);
//    //auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
//
//    //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
//    //world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
//    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
//    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left)); // 中空的玻璃球
//    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
//
//    //auto R = cos(pi / 4);
//    //hittable_list world;
//
//    //auto material_left = make_shared<lambertian>(color(0, 0, 1));
//    //auto material_right = make_shared<lambertian>(color(1, 0, 0));
//
//    //world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
//    //world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));
//
//    // Camera
//    vec3 vup(0, 1, 0);
//    auto dist_to_focus = 10.0;
//    int image_height = static_cast<int>(image_width / aspect_ratio);
//
//    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
//    //point3 lookfrom(13, 2, 3);
//    //point3 lookat(0, 0, 0);
//    //vec3 vup(0, 1, 0);
//    //auto dist_to_focus = 10.0;
//    //auto aperture = 0.1;
//    //int image_height = static_cast<int>(image_width / aspect_ratio);
//
//    //point3 lookfrom(3, 3, 2);
//    //point3 lookat(0, 0, -1);
//    //vec3 vup(0, 1, 0);
//    //auto dist_to_focus = (lookfrom - lookat).length();
//    //auto aperture = 2.0;
//
//    //camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0); // 看下小孔成像的原理
//
//    // Render
//
//    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
//
//    for (int j = image_height - 1; j >= 0; --j) {
//        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
//        for (int i = 0; i < image_width; ++i) {
//            color pixel_color(0, 0, 0);
//            for (int s = 0; s < samples_per_pixel; ++s) {
//                auto u = (i + random_double()) / (image_width - 1);
//                auto v = (j + random_double()) / (image_height - 1);
//                ray r = cam.get_ray(u, v);
//                pixel_color += ray_color(r, background, world, max_depth);
//            }
//            write_color(std::cout, pixel_color, samples_per_pixel);
//        }
//    }
//    std::cerr << "\nDone.\n";
//}

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

int main() {
    int N = 1000;
    int inside_circle = 0;
    for (int i = 0; i < N; i++) {
        auto x = random_double(-1, 1);
        auto y = random_double(-1, 1);
        if (x * x + y * y < 1)
            inside_circle++;
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Estimate of Pi = " << 4 * double(inside_circle) / N << '\n';
}