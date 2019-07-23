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
        if (t1 > 0) {
            hInfo.z = t1; //abs((ray.p + t1 * ray.dir).GetNormalized().Dot(ray.dir));
            return true;
        }
        else {
            float t2 = (-b + sqrt(det)) / a;
            if (t2 > 0) {
                hInfo.z = t2; // abs((ray.p + t2 * ray.dir).GetNormalized().Dot(ray.dir));
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
void Sphere::ViewportDisplay() const
{
    static GLUquadric *q = nullptr;
    if ( q == nullptr ) {
        q = gluNewQuadric();
    }
    gluSphere(q,1,50,50);
}