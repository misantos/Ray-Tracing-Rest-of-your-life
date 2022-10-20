#include "rtweekend.hpp"
#include "aarect.hpp"
#include "box.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

#include <iostream>
#include <string>
#include <fstream>


std::string generateFileName (int num) {
    std::string filename = "imagem";
    filename += (char)('0' + num);
    filename += ".ppm";
    return filename;
}

int escolheCamera() {
    int num;
    std::cout << "Por favor digite o numero da camera desejada: ";
    std::cin >> num;
    return num;
}

camera generateCamera(int num) {
    if (num == 1) {
        // camera 1 - padrao
        point3 lookfrom(278, 278, -1200);
        point3 lookat(278, 278, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 40.0;
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }else if (num == 2) {
        // camera 2 - diagonal
        point3 lookfrom(700, 278, 0);
        point3 lookat(278,278,500); //278,278,500
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 90.0; //90
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }else if (num == 3) {
        // camera 3 - visao do espelho
        point3 lookfrom(277, 277, 270);
        point3 lookat(-200, 277, 270);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 40.0;
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }else if (num == 4) {
        // camera 4 - visao de cima
        point3 lookfrom(650, 140, 200);
        point3 lookat(500, 140, 400);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 50.0;
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }else if (num == 5) {
        // camera 5
        point3 lookfrom(-31, 414, 128);
        point3 lookat(-31, 165, 335);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 60.0;
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }else {
        // camera 6
        point3 lookfrom(277, 180, 260);
        point3 lookat(-31, 165, 400);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;
        auto vfov = 60.0;
        auto t0 = 0.0;
        auto t1 = 1.0;
        camera cam(lookfrom, lookat, vup, vfov, 1.0, aperture, dist_to_focus, t0, t1);
        return cam;
    }
    
}


color ray_color(
    const ray& r,
    const color& background,
    const hittable& world,
    shared_ptr<hittable> lights,
    int depth
) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
             * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
         + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                            * ray_color(scattered, background, world, lights, depth-1)
                            / pdf_val;
}


hittable_list cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.64, .64, .64));
    
    auto texturaPiso = make_shared<image_texture>("pisoMadeira2.jpeg");
    auto piso = make_shared<lambertian>(texturaPiso);

    auto texturaParede = make_shared<image_texture>("textura2.jpg");
    auto paredetextura = make_shared<lambertian>(texturaParede);

    auto light = make_shared<diffuse_light>(color(70, 70, 70));
    auto espelho = make_shared<dielectric>(4);

    world.add(make_shared<yz_rect>(0, 555, 0, 1000, 755, red)); //parede da esquerda
    world.add(make_shared<yz_rect>(0, 555, 0, 1000, -200, red)); // parede da direita
    world.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));  // lampada
    world.add(make_shared<xz_rect>(-200, 755, 0, 1000, 555, white)); //teto
    world.add(make_shared<xz_rect>(-200, 755, 0, 1000, 0, piso)); // chao
    world.add(make_shared<xy_rect>(-200, 755, 0, 555, 1000, red)); // parede de tras
    world.add(make_shared<xy_rect>(-200, 277, 0, 555, 500, paredetextura)); // parede do meio
    world.add(make_shared<yz_rect>(250, 414, 128, 322, -200, espelho));
    world.add(make_shared<yz_rect>(0, 555, 500, 1000, 277, paredetextura)); // parede que fecha canto

    //caixa verde
    auto mesa1 = make_shared<image_texture>("texturamesa.jpg");
    auto mesatextura1 = make_shared<lambertian>(mesa1);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,165,320), mesatextura1);
    box1 = make_shared<rotate_y>(box1, 0);
    box1 = make_shared<translate>(box1, vec3(-31,0,335));
    world.add(box1);
    
    //caixa branca
    auto mesa2 = make_shared<image_texture>("texturamesa.jpg");
    auto mesatextura2 = make_shared<lambertian>(mesa2);
    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,320), mesatextura2);
    box2 = make_shared<rotate_y>(box2, 0);
    box2 = make_shared<translate>(box2, vec3(-196,0,335));
    world.add(box2);

    //esfera 1
    auto material1 = make_shared<dielectricModified>(.5);
    world.add(make_shared<sphere>(point3(98,205,400), 40 , material1));

    //esfera 2
    auto material2 = make_shared<noise_texture>(.30);
    world.add(make_shared<sphere>(point3(-31, 195, 410), 30, make_shared<lambertianModified>(material2)));

    //esfera 3
    auto material3 = make_shared<checker_texture>(color(1, 0, 0), color(1,1,1));
    world.add(make_shared<sphere>(point3(-150, 210, 400), 45, make_shared<lambertianModified>(material3)));

    //esfera 4
    auto material4 = make_shared<metalModified>(color(0.9, 0.3, 0.5),3.0);
    world.add(make_shared<sphere>(point3(-130, 55, 280), 50 , material4));
    // esfera 5
    auto material5 = make_shared<dielectric>(2);
    world.add(make_shared<sphere>(point3(500, 95, 450), 80 , material5));

    return world;
}


int main() {
    // Image

    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1250;
    const int max_depth = 50;

    // World

    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

    auto world = cornell_box();

    color background(0,0,0);

    // Camera
    // camera 1 - padrao
    /*point3 lookfrom(278, 278, -1200);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/
    
    // camera 2 - diagonal
    /*point3 lookfrom(700, 278, 0);
    point3 lookat(277, 278, 0);*/

    /*point3 lookfrom(700, 278, 0);
    point3 lookat(277, 278, 500);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 90.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/

    // camera 3 - visao do espelho
    /*point3 lookfrom(277, 277, 270);
    point3 lookat(-200, 277, 270);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/
    // camera 4 - visao de cima
    /*point3 lookfrom(278, 540, 500);
    point3 lookat(278, 20, 510);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 100.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/
    // camera 5
    /*point3 lookfrom(-31, 414, 128);
    point3 lookat(-31, 165, 335);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 60.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/

    // camera 6
    /*point3 lookfrom(277, 180, 400);
    point3 lookat(-31, 165, 400);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 60.0;
    auto t0 = 0.0;
    auto t1 = 1.0;*/
    int num = escolheCamera();
    camera cam = generateCamera(num);
    
    // Render
    
    std::ofstream nome_arq;
    nome_arq.open(generateFileName(num));

    nome_arq << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, lights, max_depth);
            }
            
            write_color(nome_arq, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}