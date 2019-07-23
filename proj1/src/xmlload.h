#ifndef _XMLLOAD_H_INCLUDED_
#define _XMLLOAD_H_INCLUDED_


#include "scene.h"
#include "sphere.h"
#include "tinyxml/tinyxml.h"


//-------------------------------------------------------------------------------

#ifdef WIN32
#define COMPARE(a,b) (_stricmp(a,b)==0)
#else
#define COMPARE(a,b) (strcasecmp(a,b)==0)
#endif

void LoadScene(TiXmlElement *element, Node& rootNode, Sphere& theSphere);
void LoadNode(Node *node, TiXmlElement *element, Sphere& theSphere, int level=0);
void LoadTransform( Transformation *trans, TiXmlElement *element, int level );
void ReadVector(TiXmlElement *element, Point3 &v);
void ReadFloat (TiXmlElement *element, float  &f, const char *name="value");
int LoadScene(const char *filename, Node& rootNode, Camera& camera, RenderImage& renderImage, Sphere& theSphere);
void PrintIndent(int level);

#endif