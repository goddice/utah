#ifndef _XMLLOAD_H_INCLUDED_
#define _XMLLOAD_H_INCLUDED_


#include "scene.h"
#include "sphere.h"
#include "materials.h"
#include "lights.h"
#include "tinyxml/tinyxml.h"

//-------------------------------------------------------------------------------
 
#ifdef WIN32
#define COMPARE(a,b) (_stricmp(a,b)==0)
#else
#define COMPARE(a,b) (strcasecmp(a,b)==0)
#endif
 
 
struct NodeMtl
{
    Node *node;
    const char *mtlName;
};

extern Node rootNode;
extern Camera camera;
extern RenderImage renderImage;
extern MaterialList materials;
extern LightList lights;
extern Sphere theSphere;

//-------------------------------------------------------------------------------
int LoadScene(const char *filename);
void LoadScene(TiXmlElement *element);
void LoadNode(Node *node, TiXmlElement *element, int level=0);
void LoadTransform( Transformation *trans, TiXmlElement *element, int level );
void LoadMaterial(TiXmlElement *element);
void LoadLight(TiXmlElement *element);
void ReadVector(TiXmlElement *element, Point3 &v);
void ReadColor (TiXmlElement *element, Color  &c);
void ReadFloat (TiXmlElement *element, float  &f, const char *name="value");
//-------------------------------------------------------------------------------

#endif