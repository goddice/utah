#ifndef _RENDER_H_INCLUDED_
#define _RENDER_H_INCLUDED_

#include "unistd.h"

#include "scene.h"
#include "sphere.h"
#include "xmlload.h"

bool trace(const Ray& ray, const Node* node, HitInfo& hInfo);
void render();
void monitor();

extern Node rootNode;
extern Camera camera;
extern RenderImage renderImage;
extern MaterialList materials;
extern LightList lights;
extern Sphere theSphere;

#endif