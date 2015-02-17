//#include "PROJECTScene.h"
//#include "GL\glew.h"
//
//#include "shader.hpp"
//#include "Mtx44.h"
//
//#include "Application.h"
//#include "MeshBuilder.h"
//
//using namespace::std;
//
//void InitSuperMarket(void);
//
//void InitSuperMarket()
//{
//	Mesh* tempMesh;
//	Mesh* cube;
//	Vector3 hitBox;
//	float size = 0;
//
//	tempMesh = MeshBuilder::GenerateOBJ("Floor", "OBJ//BuildingOBJ//Floor&Ceiling.obj"); tempMesh->textureID = LoadTGA("Image//CharTGA//Floor&Ceiling.tga");
//	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
//	object.push_back( new Object(Vector3(0,0,0), Vector3(0,0,0), hitBox, tempMesh, cube) );
//}