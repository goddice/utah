//-------------------------------------------------------------------------------
///
/// \file       viewport.cpp 
/// \author     Cem Yuksel (www.cemyuksel.com)
/// \version    1.2
/// \date       August 28, 2017
///
/// \brief Example source for CS 6620 - University of Utah.
///
//-------------------------------------------------------------------------------

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <functional>
#include <thread>
#include <memory>

#include "render.h"

Node rootNode;
Camera camera;
RenderImage renderImage;
MaterialList materials;
LightList lights;
ObjFileList objList;
TexturedColor background;
TexturedColor environment;
TextureList textureList;
Plane thePlane;
Sphere theSphere;
 
std::shared_ptr<std::thread> td;

void BeginRender() {
    td = std::make_shared<std::thread>(render);
}

void StopRender() {
    td->join();
}

void MonitorRender() {
    monitor();
}

void RayTracing() {
    BeginRender();
    MonitorRender();
    StopRender();
}

int main(int argc, char** argv) {
/* 	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " scene.xml\n";
		exit(1);
	} */
    LoadScene("proj7/scene.xml");
    
    std::cout << "#lights: " << lights.size() << std::endl;

    RayTracing();
    renderImage.SaveImage("proj7/scene.png");
    renderImage.SaveZImage("proj7/scene_z.png");
    return 0;    
}