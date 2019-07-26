#include "materials.h"
#include "render.h"

Color MtlBlinn::Shade(const Ray &ray, const HitInfo &hInfo, const LightList &lights, int bounceCount) const {
    Color res(0, 0, 0);

    float kr = fresnel_reflection(ray, hInfo.N, ior, hInfo.front);

    if (bounceCount > 0) {
        // reflection
        Ray reflect_ray = reflect(ray, hInfo);
        HitInfo reflect_info;
        trace(reflect_ray, &rootNode, reflect_info, HIT_FRONT_AND_BACK);
        if (reflect_info.node != nullptr) {
            res += reflect_info.node->GetMaterial()->Shade(reflect_ray, reflect_info, lights, bounceCount - 1) * (reflection.GetColor() + kr);
        }

        // refrection
        if (kr < 1) {
            Ray refract_ray = refract(ray, hInfo, ior);
            HitInfo refract_info;
            trace(refract_ray, &rootNode, refract_info, HIT_BACK);
            if (refract_info.node != nullptr) {
                Color absorped = Color::White();
                if (!hInfo.front) {
                    float h = (ray.p - hInfo.p).Length();
                    absorped.r = pow(M_E, -h * absorption.r);
                    absorped.g = pow(M_E, -h * absorption.g);
                    absorped.b = pow(M_E, -h * absorption.b);
                }
                absorped = absorped * refraction.GetColor() - kr;
                absorped.r = max(absorped.r, 0.0f);
                absorped.g = max(absorped.g, 0.0f);
                absorped.b = max(absorped.b, 0.0f);
                res += refract_info.node->GetMaterial()->Shade(refract_ray, refract_info, lights, bounceCount - 1) * absorped;
            }
        }
    }

    // material
    for (const auto& light : lights) {
        Color lightColor = light->Illuminate(hInfo.p, hInfo.N);
        if (light->IsAmbient()) {
            res += lightColor * diffuse.GetColor();
        }
        else {
            Point3 L = -light->Direction(hInfo.p);
            Point3 R = -ray.dir;
            Point3 H = (L + R).GetNormalized();
            float s = pow(max(H.Dot(hInfo.N), 0.0f), glossiness);
            float lamb = max(L.Dot(hInfo.N), 0.0f);
            res += diffuse.GetColor() * lamb * lightColor + specular.GetColor() * lamb * s * lightColor;
        }
    }
    res.ClampMin();
    res.ClampMax();
    return res;
}