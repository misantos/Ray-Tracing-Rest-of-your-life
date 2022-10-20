# Ray Tracing Rest of your life

## Descrição do terceiro trabalho da matéria de Computação Gráfica

Cenário: O cenário foi baseado na última imagem gerada do Ray Tracing: The Rest of Your Life, o cenário se assemelha a uma sala. 

Foi aumentado a largura entre as paredes e o teto e chão. Foi adicionado duas paredes no meio da sala e adicionado uma textura de imagem extraída do Google nelas, no chão e na mesa (duas boxes) também foram adicionados uma textura de imagem extraída do Google parecida com a madeira. Na parede vermelha do lado direito, foi colocado um espelho implementado do mesmo jeito que as paredes.

Ex:
```
	auto red   = make_shared<lambertian>(color(.65, .05, .05));
	auto espelho = make_shared<dielectric>(4);

	world.add(make_shared<yz_rect>(0, 555, 0, 1000, 755, red)); //parede da esquerda
	world.add(make_shared<yz_rect>(0, 555, 0, 1000, -200, red)); // parede da direita
	world.add(make_shared<yz_rect>(250, 414, 128, 322, -200, espelho));

```

Câmeras: Foram adicionadas 6 câmeras. 
- A câmera 1 é a câmera padrão definida pelo autor, frontal. 
- A câmera 2 tem uma visão diagonal do cenário. 
- A câmera 3 é uma câmera focada no espelho. 
- A câmera 4 está focada na maior esfera. 
- A câmera 5 é focada na mesa e nas esferas. 
- A câmera 6 também focada na mesa e esferas, porém uma visão lateral.

Ex: câmera 5
```
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
```
Esferas: Foram adicionadas 5 esferas com tamanhos e materiais diferentes. As 4 esferas próximas da mesa tem o seu material modificado pela classe `lambertianModified`, `metalModified` e `dielectricModified`. Para alterar esses materiais algumas variáveis foram alteradas, como também o arquivo *texture.hpp*.
Ex:
```
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
```
