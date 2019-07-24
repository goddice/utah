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
        float t2 = (-b + sqrt(det)) / a;
        if (t1 > t2) {
            std::swap(t1, t2);
        }
        float tmin = min(t1, t2);
        float tmax = max(t1, t2);

        if (hitSide != 1) {
			if (hInfo.z > tmax && tmax > 0.001) {
				hInfo.z = tmax;
				hInfo.p = ray.p + tmax * ray.dir;
				hInfo.N = hInfo.p;
			}
		}
		if (hitSide != 0) {
			if (hInfo.z > tmin && tmin > 0.001) {
				hInfo.z = tmin;
				hInfo.p = ray.p + tmin * ray.dir;
				hInfo.N = hInfo.p;
			}
		}
		return true;
    }
}

//-------------------------------------------------------------------------------
// Viewport Methods for various classes
//-------------------------------------------------------------------------------
void Sphere::ViewportDisplay(const Material *mtl) const {
}