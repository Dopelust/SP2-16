#include "collision.h"

Object::Object(Vector3 p, Vector3 c, Vector3 h)
{
	position = p;
	collision.hitbox = h;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = true; 
	mesh = NULL;
}
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* b)
{
	position = p;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = true;
	mesh = NULL;
}
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = false;
}

Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = o; size = s;
	ignoreCollision = i;
}

void Garfield::Init(Vector3 p, Vector3 c, Vector3 h, Mesh* b)
{
	position = p;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0;
	size = 1;

	meshList[GEO_HEMI] = MeshBuilder::GenerateHemi("hemi", Color(1.0f,0.62f,0.0f), 36,36, 10);
	meshList[GEO_FOOT] = MeshBuilder::GenerateHemi("hemi", Color(0.3f,0,0), 36,36, 10);
	meshList[GEO_EAR] = MeshBuilder::GenerateHemi("sphere", Color(0.9f,0.59f,0.64f), 36,36, 10);
	meshList[GEO_EAR]->material.kAmbient.Set(0.5f,0.5f,0.5f);
	meshList[GEO_EAR]->material.kDiffuse.Set(0.6f,0.6f,0.6f);
	meshList[GEO_MOUTH] = MeshBuilder::GenerateHemi("mouth", Color(0.95f,0.92f,0.53f), 36,36, 10);
	meshList[GEO_CHEEK] = MeshBuilder::GenerateSphere("cheek", Color(0.95f,0.92f,0.53f), 36,36, 10);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1.0f,0.62f,0.0f), 36,36, 10);
	meshList[GEO_JOINT] = MeshBuilder::GenerateSphere("joint", Color(1.0f,0.62f,0.0f), 36,36, 1.75);
	meshList[GEO_BSPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0,0,0), 36,36, 10);
	meshList[GEO_EYE] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 36,36, 10);
	meshList[GEO_EYE]->material.kAmbient.Set(0.8f,0.8f,0.8f);
	meshList[GEO_NOSE] = MeshBuilder::GenerateSphere("sphere", Color(1.0f,0.69f,0.74f), 12,36, 10);
	meshList[GEO_NOSE]->material.kShininess = 20.f;
	meshList[GEO_ARM] = MeshBuilder::GenerateCylinder("arm",  Color(1.0f,0.62f,0.0f), 36, 10, 1.75);
	meshList[GEO_CAPSULE] = MeshBuilder::GenerateCapsule("capsule", Color(1.0f,0.62f,0.0f), 36,36, 15, 1.75f);
	meshList[GEO_FINGER] = MeshBuilder::GenerateCapsule("finger", Color(1.0f,0.62f,0.0f), 36,36, 2.5f, 1.4f);
}

void Garfield::altInit(Vector3 p, Vector3 c, Vector3 h, Mesh* b)
{
	position = p;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0;
	size = 1;

	meshList[GEO_HEMI] = MeshBuilder::GenerateHemi("hemi", Color(0.1f,0.1f,0.1f), 36,36, 10);
	meshList[GEO_FOOT] = MeshBuilder::GenerateHemi("hemi", Color(1,1,1), 36,36, 10);
	meshList[GEO_EAR] = MeshBuilder::GenerateHemi("sphere", Color(0.9f,0,0), 36,36, 10);
	meshList[GEO_EAR]->material.kAmbient.Set(0.5f,0.5f,0.5f);
	meshList[GEO_EAR]->material.kDiffuse.Set(0.6f,0.6f,0.6f);
	meshList[GEO_MOUTH] = MeshBuilder::GenerateHemi("mouth", Color(1,1,1), 36,36, 10);
	meshList[GEO_CHEEK] = MeshBuilder::GenerateSphere("cheek", Color(1,1,1), 36,36, 10);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0.1f,0.1f,0.1f), 36,36, 10);
	meshList[GEO_JOINT] = MeshBuilder::GenerateSphere("joint", Color(0.1f,0.1f,0.1f), 36,36, 1.75);
	meshList[GEO_BSPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0,0,0), 36,36, 10);
	meshList[GEO_EYE] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 36,36, 10);
	meshList[GEO_EYE]->material.kAmbient.Set(0.8f,0.8f,0.8f);
	meshList[GEO_NOSE] = MeshBuilder::GenerateSphere("sphere", Color(0.9f,0,0), 12,36, 10);
	meshList[GEO_NOSE]->material.kShininess = 20.f;
	meshList[GEO_ARM] = MeshBuilder::GenerateCylinder("arm",  Color(0.1f,0.1f,0.1f), 36, 10, 1.75);
	meshList[GEO_CAPSULE] = MeshBuilder::GenerateCapsule("capsule", Color(0.1f,0.1f,0.1f), 36,36, 15, 1.75f);
	meshList[GEO_FINGER] = MeshBuilder::GenerateCapsule("finger", Color(0.1f,0.1f,0.1f), 36,36, 2.5f, 1.4f);
}

