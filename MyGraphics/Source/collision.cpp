#include "collision.h"

Object::Object(Vector3 p, Vector3 c, Vector3 h)
{
	position = p;
	collision.hitbox = h;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = true; 
}
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* b)
{
	position = p;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = true;
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

Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = o; size = s;
	ignoreCollision = false;
}

Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, bool i, float o)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = o; size = 1;
	ignoreCollision = i;
}

void Fish::Init(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 90;
	size = 1;

	float x = float(rand() % 21 - 10); //-10 to 10
	float y = float(rand() % 5 - 2); //-2 to 2
	float z = float(rand() % 5 - 2); //-2 to 2

	direction = Vector3(x,y,z); direction.Normalize();
	swim = 0;
}

void FishTank::Init(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0;
	size = 1;

	Material mat;
	mat.kAmbient.Set(0.5f,0.5f,0.5f);
	mat.kDiffuse.Set(0.7f,0.7f,0.7f);
	mat.kSpecular.Set(0.2f,0.2f,0.2f);
	mat.kShininess = 20.f;

	Front.location = (h.z/2) / 1.3f;
	Front.boundingBox = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), collision.hitbox.x/ 1.1f,  collision.hitbox.y/2, 1);
	Front.boundingBox->textureID = LoadTGA("Image//glass.tga");
	Front.boundingBox->material = mat;

	Back.location = (-h.z/2)/ 1.3f;
	Back.boundingBox = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), collision.hitbox.x/ 1.1f,  collision.hitbox.y/2, 1);
	Back.boundingBox->textureID = LoadTGA("Image//glass.tga");
	Back.boundingBox->material = mat;

	Top.location = h.y;
	Bottom.location = c.y;

	Left.location = (-h.x/2)/ 1.1f;
	Left.boundingBox = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), collision.hitbox.y/2,  collision.hitbox.z/ 1.3f, 1);
	Left.boundingBox->textureID = LoadTGA("Image//glass.tga");
	Left.boundingBox->material = mat;

	Right.location = (h.x/2)/ 1.1f;
	Right.boundingBox = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), collision.hitbox.y/2,  collision.hitbox.z/ 1.3f, 1);
	Right.boundingBox->textureID = LoadTGA("Image//glass.tga");
	Right.boundingBox->material = mat;

	Vector3 fishPos = p + c; fishPos.y += h.y/4;
	Mesh * fishMesh[3]; Mesh * fishHitbox[3];

	fishMesh[0] = MeshBuilder::GenerateOBJ("FishTank", "OBJ//doryfish.obj"); fishMesh[0]->textureID = LoadTGA("Image//doryfish.tga");
	fishHitbox[0] = MeshBuilder::GenerateCube("FishHitbox", Color(1,1,1), 2.5f,2.5f,1.f, 0);

	fishMesh[1] = MeshBuilder::GenerateOBJ("FishTank", "OBJ//orangefish.obj"); fishMesh[1]->textureID = LoadTGA("Image//orangefish.tga");
	fishHitbox[1] = MeshBuilder::GenerateCube("FishHitbox", Color(1,1,1), 2.5f,2.5f,1.f, 0);

	fishMesh[2] = MeshBuilder::GenerateOBJ("FishTank", "OBJ//nemofish.obj"); fishMesh[2]->textureID = LoadTGA("Image//nemofish.tga");
	fishHitbox[2] = MeshBuilder::GenerateCube("FishHitbox", Color(1,1,1), 2.5f,2.5f,1.f, 0);

	for (int i = 0; i < 3; i++)
	{
		Vector3 r; //Random Values

		if (i == 1)
			r = Vector3(-5, 5, 1);
		if (i == 2)
			r = Vector3(6, -4, -2);

		fish[i].Init(fishPos + r, Vector3(0,1.25f,0), Vector3(3.f,3.f,3.f), fishMesh[i], fishHitbox[i]);
	}
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

