
//-------------------------------------------------------------------------------
///
/// \file       materials.h 
/// \author     Cem Yuksel (www.cemyuksel.com)
/// \version    2.0
/// \date       September 3, 2015
///
/// \brief Example source for CS 6620 - University of Utah.
///
//-------------------------------------------------------------------------------
 
#ifndef _MATERIALS_H_INCLUDED_
#define _MATERIALS_H_INCLUDED_
 
#include "scene.h"
 
//-------------------------------------------------------------------------------
 
class MtlBlinn : public Material
{
public:
    MtlBlinn() : diffuse(0.5f,0.5f,0.5f), specular(0.7f,0.7f,0.7f), glossiness(20.0f) {}
    virtual Color Shade(const Ray &ray, const HitInfo &hInfo, const LightList &lights) const {
        Color res(0, 0, 0);
        for (const auto& light : lights) {
            if (light->IsAmbient()) {
                res += light->Illuminate(hInfo.p, hInfo.N);
            }
            else {
                Point3 L = -light->Direction(hInfo.p);
                Point3 R = -ray.dir;
                if (L.Dot(hInfo.N) > 0 && R.Dot(hInfo.N) > 0) {
                    Point3 H = (L + R).GetNormalized();
                    float s = pow(max(H.Dot(hInfo.N), 0), glossiness);
                    float lamb = L.Dot(hInfo.N);
                    Color lightColor = light->Illuminate(hInfo.p, hInfo.N);
                    res += diffuse * lamb * lightColor + specular * s * lightColor;
                }
            }
        }
        res.ClampMin();
        res.ClampMax();
        return res;
    }
 
    void SetDiffuse(Color dif) { diffuse = dif; }
    void SetSpecular(Color spec) { specular = spec; }
    void SetGlossiness(float gloss) { glossiness = gloss; }
 
    // virtual void SetViewportMaterial(int subMtlID=0) const; // used for OpenGL display
 
private:
    Color diffuse, specular;
    float glossiness;
};
 
//-------------------------------------------------------------------------------
 
#endif