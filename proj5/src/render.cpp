#include <iostream>
#include <chrono>
#include "render.h"

float fresnel_reflection(const Ray& ray, const Point3& N, const float& ior, bool front) {
    float cos_i = abs((-ray.dir).Dot(N));
    float sin_i = sqrt(1 - cos_i * cos_i);
    float ior_i = 1.0f;
    float ior_t = ior;
    if (!front) {
        std::swap(ior_i, ior_t);
    }
    float sin_t = ior_i * sin_i / ior_t;
    if (sin_t >= 1.0f) {
        // total reflection
        return 1.0f;
    }
    else {
        float cos_t = sqrt(1 - sin_t * sin_t);
        float r1 = (ior_t * cos_i - ior_i * cos_t) / (ior_t * cos_i + ior_i * cos_t);
        float r2 = (ior_i * cos_i - ior_t * cos_t) / (ior_i * cos_i + ior_t * cos_t);
        return 0.5 * (r1 * r1 + r2 * r2);
    }
}

Ray reflect(const Ray& ray, const HitInfo &hInfo) {
    Ray reflect_ray;
    reflect_ray.p = hInfo.p;
    reflect_ray.dir = ray.dir - 2.0f * (hInfo.N.Dot(ray.dir)) * hInfo.N;
    reflect_ray.Normalize();
    return reflect_ray;
}

Ray refract(const Ray& ray, const HitInfo& hInfo, const float& ior) {
    Point3 N = hInfo.front ? hInfo.N : -hInfo.N;
    float cos_i = abs((-ray.dir).Dot(N));
    float sin_i = sqrt(1 - cos_i * cos_i);
    float ior_i = 1.0f;
    float ior_t = ior;
    if (!hInfo.front) {
        std::swap(ior_i, ior_t);
    }
    float sin_t = ior_i * sin_i / ior_t;
    assert(sin_t < 1.0f);
    float cos_t = sqrt(1 - sin_t * sin_t);
    N = -N;
    
    Ray refract_ray;
    refract_ray.p = hInfo.p;
    refract_ray.dir = N * cos_t + N.Cross(ray.dir.Cross(N)).GetNormalized() * sin_t;
    refract_ray.Normalize();
    return refract_ray;
}

void trace(const Ray& ray, Node* node, HitInfo& hInfo, bool hitBack) {
    const Object* obj = node->GetNodeObj();

    Ray local_ray = node->ToNodeCoords(ray);
    bool res = false;

	if (obj) {
        float z = hInfo.z;
        if(!use_bb || obj->GetBoundBox().IntersectRay(local_ray, BIGFLOAT)) {
            obj->IntersectRay(local_ray, hInfo, hitBack ? HIT_FRONT_AND_BACK : HIT_FRONT);
            if (hInfo.z < z) {
                hInfo.node = node;
            }
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
                img[i + j * w] = Color24(info.node->GetMaterial()->Shade(ray, info, lights, 50));
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
    auto start = std::chrono::steady_clock::now();
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
        std::cout << "|" << n << "%";
    }
    std::chrono::duration<double> time_spend = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start);
    
    std::cout << "\nRendering time: " << time_spend.count() << " sec" << "\n";   
}