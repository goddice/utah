
//-------------------------------------------------------------------------------
///
/// \file       materials.h 
/// \author     Cem Yuksel (www.cemyuksel.com)
/// \version    4.0
/// \date       September 17, 2015
///
/// \brief Example source for CS 6620 - University of Utah.
///
//-------------------------------------------------------------------------------
 
#ifndef _MATERIALS_H_INCLUDED_
#define _MATERIALS_H_INCLUDED_

#include <cmath>
#include "scene.h"
#include "render.h"

extern Node rootNode;

Ray reflect(const Ray& ray, const HitInfo &hInfo);
Ray refract(const Ray& ray, const HitInfo& hInfo, const float& ior);
float fresnel_reflection(const Ray& ray, const Point3& N, const float& ior, bool front);
void trace(const Ray& ray, Node* node, HitInfo& hInfo, bool hitBack);

//-------------------------------------------------------------------------------
 
class MtlBlinn : public Material
{
public:
    MtlBlinn() : diffuse(0.5f,0.5f,0.5f), specular(0.7f,0.7f,0.7f), glossiness(20.0f), 
                 reflection(0,0,0), refraction(0,0,0), absorption(0,0,0), ior(1) {}
    virtual Color Shade(const Ray &ray, const HitInfo &hInfo, const LightList &lights, int bounceCount) const {
        Color res(0, 0, 0);

        float kr = fresnel_reflection(ray, hInfo.N, ior, hInfo.front);

        if (bounceCount > 0) {
            // reflection
            if (reflection != Color::Black()) {
                Ray reflect_ray = reflect(ray, hInfo);
                HitInfo reflect_info;
                trace(reflect_ray, &rootNode, reflect_info, HIT_FRONT_AND_BACK);
                if (reflect_info.node != nullptr) {
                    res += reflect_info.node->GetMaterial()->Shade(reflect_ray, reflect_info, lights, bounceCount - 1) * (reflection + kr);
                }
            }
            // refrection
            if (refraction != Color::Black() && kr < 1) {
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
                    absorped = absorped * refraction - kr;
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
                res += lightColor * diffuse;
            }
            else {
                Point3 L = -light->Direction(hInfo.p);
                Point3 R = -ray.dir;
                Point3 H = (L + R).GetNormalized();
                float s = pow(max(H.Dot(hInfo.N), 0.0f), glossiness);
                float lamb = max(L.Dot(hInfo.N), 0.0f);
                res += diffuse * lamb * lightColor + specular * lamb * s * lightColor;
            }
        }
        res.ClampMin();
        res.ClampMax();
        return res;
    }
 
    void SetDiffuse(Color dif) { diffuse = dif; }
    void SetSpecular(Color spec) { specular = spec; }
    void SetGlossiness(float gloss) { glossiness = gloss; }
 
    void SetReflection(Color reflect) { reflection = reflect; }
    void SetRefraction(Color refract) { refraction = refract; }
    void SetAbsorption(Color absorp ) { absorption = absorp; }
    void SetRefractionIndex(float _ior) { ior = _ior; }
 
    // virtual void SetViewportMaterial(int subMtlID=0) const; // used for OpenGL display
 
private:
    Color diffuse, specular, reflection, refraction;
    float glossiness;
    Color absorption;
    float ior;  // index of refraction
};
 
//-------------------------------------------------------------------------------
 
#endif