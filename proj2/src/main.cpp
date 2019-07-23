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
#include "unistd.h"

#include "scene.h"
#include "sphere.h"
#include "xmlload.h"

Node rootNode;
Camera camera;
RenderImage renderImage;
MaterialList materials;
LightList lights;
Sphere theSphere;
std::shared_ptr<std::thread> td;


bool trace(const Ray& ray, const Node* node, HitInfo& hInfo) {
    const Object* obj = node->GetNodeObj();

    Ray local_ray = node->ToNodeCoords(ray);
    bool res = false;

	if (obj) {
		if (obj->IntersectRay(local_ray, hInfo)) {
			hInfo.node = node;
            node->FromNodeCoords(hInfo);
			res |= true;
		}
	}
	for (int i = 0; i < node->GetNumChild(); ++i) {
		res |= trace(local_ray, node->GetChild(i), hInfo);
	}

    return res;
}

void render() {
    int w = renderImage.GetWidth();
    int h = renderImage.GetHeight();
    Point3 left = camera.up.Cross(camera.dir).GetNormalized();
    float plane_h = tanf(camera.fov * 0.5 * M_PI / 180.0f) * 2.0f;
    float plane_w = plane_h / h * w;
    Point3 base = camera.pos + camera.dir + plane_w * 0.5 * left + plane_h * 0.5 * camera.up;
    Point3 dx = -plane_w / w * left;
    Point3 dy = -plane_h / h * camera.up;

    Color24* img = renderImage.GetPixels();
	float* zbuffer = renderImage.GetZBuffer();
    renderImage.ResetNumRenderedPixels();

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            Ray ray;
            HitInfo info;
            ray.p = camera.pos;
            ray.dir = base + (0.5f + i) * dx + (0.5f + j) * dy - camera.pos;
            ray.Normalize();
            if (trace(ray, &rootNode, info)) {
                img[i + j * w] = Color24(info.node->GetMaterial()->Shade(ray, info, lights));
                // img[i + j * w] = Color24{255, 255, 255};
            }
            else {
                img[i + j * w] = Color24{0, 0, 0};
            }
			zbuffer[i + j * w] = info.z;
            renderImage.IncrementNumRenderPixel(1);
        }
    }
	renderImage.ComputeZBufferImage();
}

void monitor() {
    while (!renderImage.IsRenderDone()) {
        int n = (long long)renderImage.GetNumRenderedPixels() * 100 / renderImage.GetHeight() / renderImage.GetWidth();
        std::cout << "\r|";
        for (int i = 0; i < n; ++i) {
            std::cout << "=";
        }
        std::cout << ">";
        for (int i = n; i < 98; ++i) {
            std::cout << " ";
        }
        std::cout << "|";
    } 
    std::cout << "\n";   
}

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
    LoadScene("scene.xml");
    RayTracing();
    renderImage.SaveImage("render.png");
    renderImage.SaveZImage("render_z.png");
    return 0;    
}