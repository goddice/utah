#include <GL/glu.h>
#include "sphere.h"

bool Sphere::IntersectRay(const Ray& ray, HitInfo& hInfo, int hitSide) const {
    float a = ray.dir.LengthSquared();
    float b = ray.dir.Dot(ray.p);
    float c = ray.p.LengthSquared() - 1.0f;
    float det = b * b - a * c;
    if (det < 0) {
        return false;
    }
    else {
        float t1 = (-b - sqrt(det)) / a;
        if (t1 > 0 && t1 < hInfo.z) {
            hInfo.z = t1; //abs((ray.p + t1 * ray.dir).GetNormalized().Dot(ray.dir));
            hInfo.front = true;
            hInfo.p = ray.p + t1 * ray.dir;
            hInfo.N = hInfo.p.GetNormalized();
            hInfo.p += 1e-4 * hInfo.N;
            return true;
        }
        else {
            float t2 = (-b + sqrt(det)) / a;
            if (t2 > 0 && t2 < hInfo.z) {
                hInfo.z = t2; // abs((ray.p + t2 * ray.dir).GetNormalized().Dot(ray.dir));
                hInfo.front = true;
                hInfo.p = ray.p + t2 * ray.dir;
                hInfo.N = hInfo.p.GetNormalized();
                hInfo.p += 1e-4 * hInfo.N;
                return true;
            }
            else {
                return false;
            }
        }
    }
}

//-------------------------------------------------------------------------------
// Viewport Methods for various classes
//-------------------------------------------------------------------------------
void Sphere::ViewportDisplay(const Material *mtl) const {
}