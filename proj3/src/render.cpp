#include <iostream>
#include "render.h"

void trace(const Ray& ray, Node* node, HitInfo& hInfo, bool hitBack) {
    const Object* obj = node->GetNodeObj();

    Ray local_ray = node->ToNodeCoords(ray);
    bool res = false;

	if (obj) {
        float z = hInfo.z;
		obj->IntersectRay(local_ray, hInfo, hitBack ? HIT_FRONT_AND_BACK : HIT_FRONT);
        if (hInfo.z < z) {
            hInfo.node = node;
        }
	}
	for (int i = 0; i < node->GetNumChild(); ++i) {
        const Node* prev = hInfo.node;
		trace(local_ray, node->GetChild(i), hInfo, hitBack);
        if (hInfo.node != prev) {
            node->GetChild(i)->FromNodeCoords(hInfo);
        }
	}
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
            trace(ray, &rootNode, info, false);
            if (info.node) {
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