#include "NPC.h"

void NPC::Init()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	tempMesh = MeshBuilder::GenerateOBJ("Head", "OBJ//CharOBJ//Head.obj");
	hitBox = Vector3(1.5f, 1.5f, 1.5f); cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[HEAD] = Bodypart(position, Vector3(0,5.25f,0), hitBox, tempMesh, cube);

	tempMesh = MeshBuilder::GenerateOBJ("Body", "OBJ//CharOBJ//Body.obj");
	hitBox = Vector3(1.5f, 2.25f, 0.75f); cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[BODY] = Bodypart(position, Vector3(0,3.375f,0), hitBox, tempMesh, cube);

	tempMesh = MeshBuilder::GenerateOBJ("Left Arm", "OBJ//CharOBJ//LeftArm.obj");
	hitBox = Vector3(0.75f, 2.25f, 0.75f); cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[L_ARM] = Bodypart(position, Vector3(-1.125f,3.375f,0), hitBox, tempMesh, cube);

	tempMesh = MeshBuilder::GenerateOBJ("Right Arm", "OBJ//CharOBJ//RightArm.obj");
	cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[R_ARM] = Bodypart(position, Vector3(1.125f,3.375f,0), hitBox, tempMesh, cube);

	tempMesh = MeshBuilder::GenerateOBJ("Left Leg", "OBJ//CharOBJ//LeftLeg.obj");
	cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[L_LEG] = Bodypart(position, Vector3(-0.375f,1.125f,0), hitBox, tempMesh, cube);

	tempMesh = MeshBuilder::GenerateOBJ("Right Leg", "OBJ//CharOBJ//RightLeg.obj");
	cube = MeshBuilder::GenerateCube("Head", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	bodyParts[R_LEG] = Bodypart(position, Vector3(0.375f,1.125f,0), hitBox, tempMesh, cube);
}

void Hobo::Init()
{
	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//hobo.tga");
		bodyParts[i].identity = "Homeless Man";
		bodyParts[i].position = position;
	}
}