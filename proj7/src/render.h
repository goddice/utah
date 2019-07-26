#ifndef _RENDER_H_INCLUDED_
#define _RENDER_H_INCLUDED_

#include "scene.h"
#include "objects.h"
#include "xmlload.h"

const bool use_bb = true;
const bool use_bvh = true;

Ray reflect(const Ray& ray, const HitInfo &hInfo);
Ray refract(const Ray& ray, const HitInfo& hInfo, const float& ior);
float fresnel_reflection(const Ray& ray, const Point3& N, const float& ior, bool front);
void trace(const Ray& ray, Node* node, HitInfo& hInfo, bool hitBack);
void render();
void monitor();

extern Node rootNode;
extern Camera camera;
extern RenderImage renderImage;
extern MaterialList materials;
extern LightList lights;
extern ObjFileList objList;

#endif