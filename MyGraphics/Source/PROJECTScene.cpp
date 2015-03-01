#include "PROJECTScene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "GLFW\glfw3.h"
using namespace::std;

float PROJECTScene::inputDelay = 0.f;

PROJECTScene::PROJECTScene()
{
}
PROJECTScene::~PROJECTScene()
{
}
void PROJECTScene::InitJunk()
{
	pause = false;

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//MultiLight.fragmentshader" );
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");

	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	glUseProgram(m_programID);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Skybox//frontmc.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Skybox//backmc.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Skybox//leftmc.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Skybox//rightmc.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox//floormc.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Skybox//topmc.tga");
}
void PROJECTScene::RicssonInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	hitBox = player.collision.hitbox; player.collision.boundingBox =  MeshBuilder::GenerateCube("PlayerHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( &player );
	
	tempMesh = MeshBuilder::GenerateOBJ("Shelf", "OBJ//LowPoly//shelf.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//Shelf.tga");
	hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float z = 0; z >= -45; z-=22.5f)
	{
		hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(0,0,z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));
		hitBox = Vector3(0.5f,6,3.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(10.25f,0,z), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-10.25f,0,z), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(20,0.1f,1.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(0,0,z + 1), Vector3(0,0.45,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,2,z + 1), Vector3(0,0.45,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,4,z + 1), Vector3(0,0.45,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,0,z - 1), Vector3(0,0.45,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,2,z - 1), Vector3(0,0.45,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,4,z - 1), Vector3(0,0.45,0), hitBox, cube));
	}

	for (int x = 0; x < 2; x++)
	{
		for (int z = -1; z < 5; z++)
		{
			Vector3 p(-50 + x * 18,0, z * -15);

			tempMesh = MeshBuilder::GenerateOBJ("Display Table", "OBJ//LowPoly//display.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//display.tga");
			hitBox = Vector3(6, 0.1f, 6); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			object.push_back( new Storage(p, Vector3(0,3.2f,0), hitBox, tempMesh, cube));

			p.y += 3.25f;

			int r = rand () % 2;

			if (r == 0)
			{
				tempMesh = MeshBuilder::GenerateOBJ("Chocolate", "OBJ//Food//chocolate.obj"); tempMesh->textureID = LoadTGA("Image//Food//chocolate.tga");
				hitBox = Vector3(1.5f, 0.25f, 1.5f); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			}
			else 
			{
				tempMesh = MeshBuilder::GenerateOBJ("Hersheys", "OBJ//Food//choco_bar.obj"); tempMesh->textureID = LoadTGA("Image//Food//Chocolate Bar texture.tga");
				hitBox = Vector3(0.8f, 0.2f, 0.8f); cube = MeshBuilder::GenerateCube("ChocoBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			}

			for (int X = -1; X < 2; X++)
			{
				for (int Z = -1; Z < 2; Z++)
				{
					float rX = rand () % 11 - 5; rX /= 10;
					float rZ = rand () % 11 - 5; rZ /= 10;
					Vector3 P(rX + X * 2.f, 0, rZ + Z * 2.f);
					object.push_back( new Item(p + P, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true, 10) );
				}
			}
		}
	}
	/*
	tempMesh = MeshBuilder::GenerateOBJ("Cigarettes", "OBJ//Food//marlbororeds.obj"); tempMesh->textureID = LoadTGA("Image//Food//marlbororeds.tga");
	hitBox = Vector3(0.2f, 0.325f, 0.075f); cube = MeshBuilder::GenerateCube("Cigarettes", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = 0; x <= 18; x++)
	{
		object.push_back( new Item(Vector3(x-9,0.5,-54.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	
	*/
	tempMesh = MeshBuilder::GenerateQuad("", Color(1, 1, 1), 160.f, 160, 25); tempMesh->textureID = LoadTGA("Image//floor.tga");
	hitBox = Vector3(160, 0.1f, 160.f); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,-7.5), Vector3(0,-0.05f,0), hitBox, tempMesh, cube) );

	for (int y = 0; y <= 6; y+=3)
	{
		hitBox = Vector3(3, 3, 3);
		tempMesh = MeshBuilder::GenerateCubeOnPlane("Crate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//crate.tga");
		cube = MeshBuilder::GenerateCube("CrateHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new dynamicObject(Vector3(15,y,39), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, rand() % 21 - 10) );
	}
	for ( int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			Vector3 p(30 + x * 18,0, z * -15);

			tempMesh = MeshBuilder::GenerateOBJ("Display Table", "OBJ//LowPoly//display.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//display.tga");
			hitBox = Vector3(6, 0.1f, 6); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			object.push_back( new Storage(p, Vector3(0,3.2f,0), hitBox, tempMesh, cube));

			p.y += 3.25f;

			int r = rand () % 2;

			if (r == 0)
			{
				tempMesh = MeshBuilder::GenerateOBJ("Chocolate", "OBJ//Food//chocolate.obj"); tempMesh->textureID = LoadTGA("Image//Food//chocolate.tga");
				hitBox = Vector3(1.5f, 0.25f, 1.5f); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			}
			else 
			{
				tempMesh = MeshBuilder::GenerateOBJ("Hersheys", "OBJ//Food//choco_bar.obj"); tempMesh->textureID = LoadTGA("Image//Food//Chocolate Bar texture.tga");
				hitBox = Vector3(0.8f, 0.2f, 0.8f); cube = MeshBuilder::GenerateCube("ChocoBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			}

			for (int X = -1; X < 1; X++)
			{
				for (int Z = -1; Z < 1; Z++)
				{
					float rX = rand () % 11 - 5; rX /= 10;
					float rZ = rand () % 11 - 5; rZ /= 10;
					Vector3 P(rX + X * 2.f, 0, rZ + Z * 2.f);
					object.push_back( new Item(p + P, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true , 5) );
				}
			}
		}
	}
}
void PROJECTScene::JeremiahInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	Vector3 hitBox2;
	float size = 0;
	
	character.push_back( new Hobo() );
	character.push_back( new Thug() );

	vector<NPCTarget> path;
	for (int z = 0; z < 2; z++)
	{
		path.push_back(NPCTarget(Vector3(-71,2,-69.45f + z * 18.75f), -90.f));
		path.push_back(NPCTarget(Vector3(-71,2,24.45f + z * -18.75f), -90.f));
	}
	character.push_back( new Customer(path, "Customer-chan", LoadTGA("Image//CharTGA//customer.tga"), 10.f) );
	path.clear();
	for (int x = 0; x < 2; x++)
	{
		for (int z = -1; z < 5; z++)
		{
			Vector3 p(-45 + x * 8,0, z * -15);
			if (x == 0)
				path.push_back(NPCTarget(p, -90.f));
			else
				path.push_back(NPCTarget(p, 90.f));
		}
	}
	character.push_back( new Customer(path, "Customer-san", LoadTGA("Image//CharTGA//customer-2.tga"), 5.f) );
	path.clear();
	cout << path.size();
	path.push_back(NPCTarget(Vector3(-8,0,-41), 180.f));
	path.push_back(NPCTarget(Vector3(0,0,-41), 180.f));
	path.push_back(NPCTarget(Vector3(8,0,-41), 180.f));
	path.push_back(NPCTarget(Vector3(-8,0,-27), 0.f));
	path.push_back(NPCTarget(Vector3(0,0,-27), 0.f));
	path.push_back(NPCTarget(Vector3(8,0,-27), 0.f));
	character.push_back( new Customer(path, "Harem-king", LoadTGA("Image//CharTGA//C_Boy.tga"),1.f));
	path.clear();

	path.push_back(NPCTarget(Vector3(-8,0,-18.5), 180.f));
	path.push_back(NPCTarget(Vector3(0,0,-18.5), 180.f));
	path.push_back(NPCTarget(Vector3(8,0,-18.5), 180.f));
	path.push_back(NPCTarget(Vector3(-8,0,-4.5), 0.f));
	path.push_back(NPCTarget(Vector3(0,0,-4.5), 0.f));
	path.push_back(NPCTarget(Vector3(8,0,-4.5), 0.f));
	character.push_back( new Customer(path, "Chunni-kun", LoadTGA("Image//CharTGA//Thief.tga"),1.f));
	path.clear();

	character.push_back( new Blindman() );
	character.push_back( new Detective () );
	character.push_back( new Manager () );
	for(int x = 72; x > 20; x -= 15)
	{
		character.push_back( new Cashier(Vector3(x,2,-55)) );
	}
	character.push_back( new S_Guard (Vector3(18, 2, -84)) );
	character.push_back( new S_Guard (Vector3(-18, 2, -84)) );
	for (unsigned int i = 0; i < character.size(); i++)
	{
		object.push_back( character[i] );
	}
		
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	hitBox = Vector3(10, 25, 10); 
	tempMesh = MeshBuilder::GenerateCube("Pillar", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); 
	cube = MeshBuilder::GenerateCube("Pilar", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-75,hitBox.y/2,-81.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-75,hitBox.y/2,36.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(2, 50, 132); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(81,hitBox.y/2,-22.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-81,hitBox.y/2,-22.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(73, 50, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(44,hitBox.y/2,42.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-44,hitBox.y/2,42.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	
	hitBox = Vector3(17, 16.5, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,hitBox.y/2+8.5,42.5), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox= Vector3(17,14.5,2);
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,hitBox.y/2+35.5,42.5), Vector3(0,0,0), hitBox, tempMesh, cube) );
	
	hitBox = Vector3(40,25.1,2);
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,hitBox.y/2+24.9,-87.5), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(60, 50, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(50,hitBox.y/2,-87.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-50,hitBox.y/2,-87.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market(Outside of Supermarkte)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	hitBox = Vector3(160, 0.1f, 25.f); 
	tempMesh = MeshBuilder::GenerateQuad("", Color(1, 1, 1), hitBox.x, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Road&Pave.tga");
	cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,-100), Vector3(0,-0.05f,0), hitBox, tempMesh, cube) );
	tempMesh->textureID = LoadTGA("Image//BuildingTGA//Road&Pave.tga");
	object.push_back( new Object(Vector3(0,0,-125), Vector3(0,-0.05f,0), hitBox, tempMesh, cube) );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Cash Table~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	
	for( int x = 70; x > 20; x-=15)
	{
		tempMesh = MeshBuilder::GenerateOBJ("CashierTable", "OBJ//LowPoly//cashiertable.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//cashiertable.tga");
		hitBox = Vector3(3, 3.5f, 10.f); cube = MeshBuilder::GenerateCube("CashierTable Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(x,0,-55), Vector3(-1.5f,hitBox.y/2,0), hitBox, tempMesh, cube) );
		hitBox = Vector3(3,3.5f,3); cube = MeshBuilder::GenerateCube("CashierTable HitBox", Color(1,1,1), hitBox.x,hitBox.y,hitBox.z,0);
		object.push_back( new Object(Vector3(x+3,0,-58.5f),Vector3(-1.5f,hitBox.y/2,0), hitBox, cube) );

		tempMesh = MeshBuilder::GenerateOBJ("CashierTable", "OBJ//LowPoly//cashregister.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//cashregister.tga");
		hitBox = Vector3(0,0,0); cube = MeshBuilder::GenerateCube("CashierTable Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(x+1.5f,3.5f,-58.5f), Vector3(0,0,0), hitBox, tempMesh, cube, 1, -90, true) );
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~2nd Floor~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	hitBox = Vector3(35, 2, 130); 
	tempMesh = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Floor&Ceiling.tga");
	cube = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(62.5,hitBox.y/2+25,-22.5), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-62.5,hitBox.y/2+25,-22.5), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(90, 2, 35); 
	tempMesh = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Floor&Ceiling.tga");
	cube = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,hitBox.y/2+25,47.5-22.5), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(0,hitBox.y/2+25,-47.5-22.5), Vector3(0,0,0), hitBox, tempMesh, cube) );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Crime Scene~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	tempMesh = MeshBuilder::GenerateQuad("", Color(1, 1, 1), 7, 7, 1); tempMesh->textureID = LoadTGA("Image//ChalkOutline.tga");
	object.push_back( new Object(Vector3(63,0.09,27), Vector3(0,0,0), hitBox, tempMesh, NULL, 1 , 54, true) );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Railing~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	tempMesh = MeshBuilder::GenerateOBJ("Railing near counter", "OBJ//railing.obj"); tempMesh->textureID = LoadTGA("Image//railing.tga");
	hitBox = Vector3(0.5f, 3.5f, 8.0f); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for(int x = 17; x <= 68; x += 15 )
	{
		object.push_back( new Object(Vector3(x,0,-53), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, false) );
	}
	tempMesh = MeshBuilder::GenerateOBJ("Railing 2nd floor", "OBJ//railing2.obj"); tempMesh->textureID = LoadTGA("Image//railing.tga");
	hitBox = Vector3(92, 3.5f, 1); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,26.8,-22.5), Vector3(0,hitBox.y/2,30.5), hitBox, tempMesh, cube, 1, 0, false) );
	object.push_back( new Object(Vector3(0,26.8,-22.5), Vector3(0,hitBox.y/2,-30.5), hitBox, cube) );

	hitBox = Vector3(1, 3.5f, 62); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,26.8,-22.5), Vector3(45.5,hitBox.y/2,0), hitBox, cube) );
	object.push_back( new Object(Vector3(0,26.8,-22.5), Vector3(-45.5,hitBox.y/2,0), hitBox, cube) );


}
void PROJECTScene::JessicaInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	//~~~~~~~~~~~FOOD~~~~~~~~~~~~~~
	tempMesh = MeshBuilder::GenerateOBJ("Pizza", "OBJ//Food//pizza.obj"); tempMesh->textureID = LoadTGA("Image//Food//frozen_pizza.tga");
	hitBox = Vector3(2.0f, 0.3f, 2.0f); cube = MeshBuilder::GenerateCube("PizzaHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,4.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true, 20) );
	}
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,2.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true, 20) );
	}
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,0.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true, 20) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Soup", "OBJ//Food//soup.obj"); tempMesh->textureID = LoadTGA("Image//Food//soup.tga");
	hitBox = Vector3(0.3f, 0.5f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,4.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 5) );
	}

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,2.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 5) );
	}

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,0.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 5) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Blue Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox2.tga");
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Green Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox3.tga");
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 10) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Brown Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox4.tga");
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 10) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 10) );
	}

	//~~~~~~~~~~ICE CREAM~~~~~~~~~~~

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Oreo", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//oreo.tga");
	hitBox = Vector3(1.f, 1.f, 0.3f); cube = MeshBuilder::GenerateCube("Ice_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,5,-85), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 9) );
	}
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,2,-85), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 9) );
	}
	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Raspberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//raspberry.tga");
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,5,40), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,2,40), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Lime", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//lime.tga");
	hitBox = Vector3(0.3f, 1.f, 1.f); cube = MeshBuilder::GenerateCube("Ice_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -75; x <= -64; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 9) );
	}
	for (int x = -75; x <= -64; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 9) );
	}

	
	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Cheese", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//cheese.tga");
	for (int x = -56; x <= -45; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true, 9) );
	}
	for (int x = -56; x <= -45; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Strawberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//strawberry.tga");
	for (int x = 0; x <= 11; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}
	for (int x = 0; x <= 11; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Blueberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//blueberry.tga");
	for (int x = 19; x <= 30.5; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}
	for (int x = 19; x <= 30.5; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true, 9) );
	}
	//~~~~~~~~~~SHELF~~~~~~~~~~~~
	//Dimensions = 5 x 8.3 x 14
	tempMesh = MeshBuilder::GenerateOBJ("Shelf", "OBJ//NewShelf//coolershelf.obj"); tempMesh->textureID = LoadTGA("Image//NewShelf//Shelf_Cooler.tga");
	hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int z = 0; z < 2; z++)
	{
		hitBox = Vector3(4.4f,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,0,-69.45f + z * 18.75f), Vector3(0.3f,2.f,0), hitBox, tempMesh, cube));
		hitBox = Vector3(4.4f,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,4.65f + 0.3f,-69.45f + z * 18.75f), Vector3(0.3f,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5.f,8.3f,0.3f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,0,-69.45f + 6.85f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-77.6f,0,-69.45f - 6.85f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5,1.f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,7.3f,-69.45f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
	}

	for (int z = 0; z < 2; z++)
	{
		hitBox = Vector3(4.4f,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,0,24.45f + z * -18.75f), Vector3(0.3f,2.f,0), hitBox, tempMesh, cube));
		hitBox = Vector3(4.4f,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,4.65f + 0.3f,24.45f + z * -18.75f), Vector3(0.3f,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5.f,8.3f,0.3f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,0,24.45f + 6.85f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-77.6f,0,24.45f - 6.85f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5,1.f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,7.3f,24.45f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
	}
	hitBox = Vector3(13.4f,0.1f,4.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,0,39.1), Vector3(0,2.f,0.3f), hitBox, tempMesh, cube, 90));
	hitBox = Vector3(13.4f,0.1f,4.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,4.65f + 0.3f,39.1), Vector3(0,hitBox.y/2,0.3f), hitBox, cube));
	hitBox = Vector3(0.3f,8.3f,5.f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f+6.8,0,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	object.push_back( new Object(Vector3(-62.5f-6.8,0,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(13.4f,1.f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f,7.3f,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));

	hitBox = Vector3(13.4f,0.1f,4.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,0,-84.1), Vector3(0,2.f,0.3f), hitBox, tempMesh, cube, 270));
	hitBox = Vector3(13.4f,0.1f,4.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,4.65f + 0.3f,-84.1), Vector3(0,hitBox.y/2,0.3f), hitBox, cube));
	hitBox = Vector3(0.3f,8.3f,5.f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f+6.8,0,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	object.push_back( new Object(Vector3(-62.5f-6.8,0,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(13.4f,1.f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f,7.3f,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	
	//~~~~~~POSTER~~~~~
	hitBox = Vector3();
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 7, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice.tga");
	object.push_back( new Object(Vector3(-79.55,5.5,-36), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	object.push_back( new Object(Vector3(-79.55,5.5,-25), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	object.push_back( new Object(Vector3(-79.55,15,-60), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	object.push_back( new Object(Vector3(-79.55,15,15), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice2.tga");
	object.push_back( new Object(Vector3(-79.54,6.5,-15), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 7, 1); tempMesh->textureID = LoadTGA("Image//Poster//poster_icecream.tga");
	object.push_back( new Object(Vector3(-79.55,5,-8), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//Golden.tga");
	object.push_back( new Object(Vector3(-42,6,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//Golden.tga");
	object.push_back( new Object(Vector3(-20,6,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//rocher.tga");
	object.push_back( new Object(Vector3(-30,9,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//pizza.tga");
	object.push_back( new Object(Vector3(35,13,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 19, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//pepperoni.tga");
	object.push_back( new Object(Vector3(49,12,41.46), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//soup.tga");
	object.push_back( new Object(Vector3(19,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//soup.tga");
	object.push_back( new Object(Vector3(71,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 8, 1); tempMesh->textureID = LoadTGA("Image//Poster//new_soup.tga");
	object.push_back( new Object(Vector3(64,5,41.465), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 29, 20, 1); tempMesh->textureID = LoadTGA("Image//Poster//member.tga");
	object.push_back( new Object(Vector3(48,13,-86), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 ,0, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//shop-theft3.tga");
	object.push_back( new Object(Vector3(28,13,-88.6), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//shop-theft3.tga");
	object.push_back( new Object(Vector3(-28,13,-88.6), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 8, 1); tempMesh->textureID = LoadTGA("Image//Poster//Teaser.tga");
	object.push_back( new Object(Vector3(0,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 20, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//milo.tga");
	object.push_back( new Object(Vector3(-25,38,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 20, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//veg.tga");
	object.push_back( new Object(Vector3(25,38,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, NULL, 1 , 180, true) );

	//~~~~~~MONEY~~~~~~~

	tempMesh = MeshBuilder::GenerateOBJ("10 Dollar", "OBJ//Money.obj"); tempMesh->textureID = LoadTGA("Image//Money10.tga");
	hitBox = Vector3(0.3f, 0.1f, 0.4f); cube = MeshBuilder::GenerateCube("Money_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float y = 0; y <= 0.25f; y+= 0.05f)
	{
		object.push_back( new Money(Vector3(52,y,-89), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1,0, 10) );
	}
	for (float y = 0; y <= 0.25f; y+= 0.05f)
	{
		object.push_back( new Money(Vector3(-55,y,-89), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, 10) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Dollar", "OBJ//Money.obj"); tempMesh->textureID = LoadTGA("Image//Money1.tga");
	object.push_back( new Money(Vector3(53,3.5,-51), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1,0, 1) );
	object.push_back( new Money(Vector3(48,3.3,-13), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, 1) );
	for (float z = -58; z <= 17.f; z+= 15)
	{
		object.push_back( new Money(Vector3(-30,3.3,z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, 1) );
	}
	for (float z = -58; z <= 17.f; z+= 15)
	{
		object.push_back( new Money(Vector3(-50,3.3,z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, 1) );
	}
	for (float y = 0; y <= 0.25f; y+= 0.05f)
	{
		object.push_back( new Money(Vector3(12.5,y,40.5), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, 1) );
	}
	for (float y = 27.25f; y <= 27.5f; y+= 0.05f)
	{
		object.push_back( new Money(Vector3(8.85,y,40.5), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, 1) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("50 Dollar", "OBJ//Money.obj"); tempMesh->textureID = LoadTGA("Image//Money50.tga");
	object.push_back( new Money(Vector3(-69,5.05,38), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1,0, 50) );
	for (float y = 0; y <= 0.20f; y+= 0.05f)
	{
		object.push_back( new Money(Vector3(-79,y,16.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 90, 50) );
	}
	for (float y = 0.5; y <= 4.75f; y+= 2.f)
	{
			object.push_back( new Money(Vector3(-9.5,y,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, 50) );
	}

	//~~~~~VENDING~~~~~

	tempMesh = MeshBuilder::GenerateOBJ("Vending Machine", "OBJ//vending.obj"); tempMesh->textureID = LoadTGA("Image//vending.tga");
	hitBox = Vector3(5.f, 10.0f, 5.0f); cube = MeshBuilder::GenerateCube("VM_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object D(Vector3(26,0,39.5), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false);
	Machine = Vending(D);
	object.push_back( &Machine.Drink );

	//~~~~~SECURITY ROOM~~~~~~~~
	hitBox = Vector3(28, 23, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-95, 38.5,8.f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(29, 23, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-95, 38.5,42.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	
	hitBox = Vector3(2, 23, 36); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-110, 38.5,25.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(30, 2, 35); 
	tempMesh = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Floor&Ceiling.tga");
	cube = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-95,hitBox.y/2+25,25.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	/*hitBox = Vector3(27, 2, 33); 
	tempMesh = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 1); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Floor&Ceiling.tga");
	cube = MeshBuilder::GenerateCube("2ndFloor", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-95.5,hitBox.y/2+48,25.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	*/

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Securiuty Door~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	hitBox = Vector3();
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 5.5f, 10, 1); tempMesh->textureID = LoadTGA("Image//MetalDoor.tga");
	object.push_back( new Object(Vector3(-79.95f,32.f,25.f), Vector3(0,0,0), hitBox, tempMesh, NULL, 1 , 90, true) );

	//~~~~SECURITY ITEMS~~~~~

	tempMesh = MeshBuilder::GenerateOBJ("Display Table", "OBJ//LowPoly//display.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//display.tga");
	hitBox = Vector3(6, 4.f, 6); cube = MeshBuilder::GenerateCube("table", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float y = -102; y <= -87; y+= 5)
	{
		object.push_back( new Object(Vector3(y , 27.25, 38.45f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("CCTV - TV", "OBJ//tv.obj"); tempMesh->textureID = LoadTGA("Image//tv.tga");
	hitBox = Vector3(2.f, 2.f, 2.0f); cube = MeshBuilder::GenerateCube("TV_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object L(Vector3( -90, 30.5, 38.45f), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false);
	Object K(Vector3( -97, 30.5, 38.45f), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false);
	Camera = Security(L, K);
	object.push_back( &Camera.Look );
	object.push_back( &Camera.Looks );


}
void PROJECTScene::DarrenInit()
{	
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Border", "OBJ//elevatorborder.obj"); tempMesh->textureID = LoadTGA("Image//silver.tga");
	hitBox = Vector3(2.75f, 40.25f, 3.5f); cube = MeshBuilder::GenerateCube("ElevatorBorderHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(6.5,0,42), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-6.7,0,42), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Button", "OBJ//button.obj"); tempMesh->textureID = LoadTGA("Image//buttonOff.tga");
	hitBox = Vector3(1, 2.1f, 1.4f); cube = MeshBuilder::GenerateCube("ButtonHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object ButtonIn(Vector3(-10,4,-1), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 0, true);
	Object ButtonOut(Vector3(4.25,4,5), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 90, true);

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Door", "OBJ//elevatordoor.obj"); tempMesh->textureID = LoadTGA("Image//elevatordoor.tga");
	hitBox = Vector3(5.3,8.5,1.2f); cube = MeshBuilder::GenerateCube("ElevatorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object Door(Vector3(0,0,0), Vector3(0,4.25,0), hitBox, tempMesh, cube);
	hitBox = Vector3(10,8.5,11); cube = MeshBuilder::GenerateCube("ElevatorINSIDEHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object lvlRange1(Vector3(0,hitBox.y/2,hitBox.z/2), Vector3(0,0,0), hitBox, cube);
	doorway.Init(Vector3(0,0,42), Door, ButtonIn, ButtonOut, lvlRange1);
	object.push_back( &doorway.Door[0] );
	object.push_back( &doorway.Door[1] );
	object.push_back( &doorway.Button[0] );
	object.push_back( &doorway.Button[1] );
	
	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorright.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-12,0,46.8), Vector3(6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorleft.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	object.push_back( new Object(Vector3(12,0,46.8), Vector3(-6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorbottom.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,47), Vector3(0,-0.5,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatortop.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	object.push_back( new Object(Vector3(0,-0.5,47), Vector3(0,9,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorback.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 9, 1); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,56), Vector3(0,4,-5), hitBox, tempMesh, cube) );
	
	// 2nd elevator

	tempMesh = MeshBuilder::GenerateOBJ("Button", "OBJ//button.obj"); tempMesh->textureID = LoadTGA("Image//buttonOff.tga");
	hitBox = Vector3(1, 2.1f, 1.4f); cube = MeshBuilder::GenerateCube("ButtonHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object ButtonIn2(Vector3(-10,5,-1), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 0, true);
	Object ButtonOut2(Vector3(4.25,5,5), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 90, true);

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Door", "OBJ//elevatordoor.obj"); tempMesh->textureID = LoadTGA("Image//elevatordoor.tga");
	hitBox = Vector3(5.3,8.5,1.2f); cube = MeshBuilder::GenerateCube("ElevatorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object Door2(Vector3(0,25,0), Vector3(0,4.25,0), hitBox, tempMesh, cube);
	hitBox = Vector3(10,8.5,11); cube = MeshBuilder::GenerateCube("ElevatorINSIDEHitbox2", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object lvlRange2(Vector3(0,hitBox.y/2,hitBox.z/2), Vector3(0,0,0), hitBox, cube);
	doorway2.Init(Vector3(0,27,42), Door2, ButtonIn2, ButtonOut2, lvlRange2);
	object.push_back( &doorway2.Door[0] );
	object.push_back( &doorway2.Door[1] );
	object.push_back( &doorway2.Button[0] );
	object.push_back( &doorway2.Button[1] );
	

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorright.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-12,27,47), Vector3(6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorleft.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(12,27,47), Vector3(-6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorbottom.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,27,47), Vector3(0,-0.5,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatortop.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,26.5,47), Vector3(0,9,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorback.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 9, 1); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,27,56), Vector3(0,4,-5), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("ATM", "OBJ//atm.obj"); tempMesh->textureID = LoadTGA("Image//atm.tga");
	hitBox = Vector3(2,7,3); cube = MeshBuilder::GenerateCube("ATMHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object D(Vector3(-50, 0.5 ,-89.5), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false);
	Object W(Vector3(50, 0.5 ,-89.5), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false);
	Bank = ATM( D, W);
	object.push_back( &Bank.Deposit );
	object.push_back( &Bank.Withdraw );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Plantsss~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	tempMesh = MeshBuilder::GenerateXYQuad("Potted Plant", Color(1,1,1),7,7.5,1); tempMesh->textureID = LoadTGA("Image//plant.tga");
	hitBox = Vector3(5,7.5,5); cube = MeshBuilder::GenerateCube("Plant Hitbox",Color(1,1,1),hitBox.x,hitBox.y,hitBox.z,0);
	for(int z = -55.5; z <= 10; z +=65.5)
	{
		for(int x = -43; x <= 43; x += 43)
		{
		object.push_back( new Object(Vector3(x,31,z), Vector3(0,-0.5,0),hitBox,tempMesh,cube,1,45,false));
		object.push_back( new Object(Vector3(x,31,z), Vector3(0,-0.5,0),hitBox,tempMesh,cube,1,315,false));
		}
	}
	for(int x = 47; x >= -47; x -= 94)
	{
		object.push_back( new Object(Vector3(x,31,-22.5), Vector3(0,-0.5,0),hitBox,tempMesh,cube,1,45,false));
		object.push_back( new Object(Vector3(x,31,-22.5), Vector3(0,-0.5,0),hitBox,tempMesh,cube,1,315,false));
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market Entrance~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	
	tempMesh = MeshBuilder::GenerateOBJ("Auto Door", "OBJ//glass_d.obj"); tempMesh->textureID = LoadTGA("Image//GlassDoor.tga");
	hitBox = Vector3(20,25,1.25); cube = MeshBuilder::GenerateCube("AutoDoorHit_B", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object autoDoor(Vector3(0,0,0), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube);
	hitBox = Vector3(33.5f,25,30); cube = MeshBuilder::GenerateCube("AutoDoorHit_B", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object autoRange(Vector3(0,0,0), Vector3(0,hitBox.y/2,0), hitBox, cube);
	AutoDoor.Init(Vector3(0,0,-87.5), autoDoor, autoRange);
	object.push_back( &AutoDoor.Door[0] );
	object.push_back( &AutoDoor.Door[1] );
}
void PROJECTScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	InitJunk();

	CCTVs[0].Init(Vector3(79,22,-85), -45, 0);
	CCTVs[1].Init(Vector3(79,22,40.5f), -135, 0);
	CCTVs[2].Init(Vector3(-69,22,30.5f), 135, 0);
	CCTVs[3].Init(Vector3(-69,22,-75.5f), 45, 0);

	camera = &player.camera;

	RicssonInit();
	JeremiahInit();
	JessicaInit();
	DarrenInit();

	meshList[GEO_HOLD] = MeshBuilder::GenerateCubeOnPlane("Cube", Color(1,1,1), 1.f, 1.f, 1.f, 1);
	meshList[GEO_HOLD]->textureID = LoadTGA("Image//hold.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 0.1f, 0.1f, 0.1f, 1);
	meshList[GEO_BIGCUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 12.f, 12.f, 12.f, 1);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateXYQuad("Text Box", Color(1,1,1), 32, 8.5f, 1);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//textbox.tga");

	meshList[GEO_QUAD] = MeshBuilder::GenerateXYQuad("Overlay", Color(1,1,1), 8, 1.5f, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//UI//ui_overlay.tga");

	meshList[GEO_HEART] = MeshBuilder::GenerateXYQuad("Heart", Color(1,1,1), 1, 1, 1);
	meshList[GEO_HEART]->textureID = LoadTGA("Image//UI//heart.tga");

	meshList[GEO_CCTV] = MeshBuilder::GenerateXYQuad("CCTV Overlay", Color(1,1,1), 4, 3, 1);
	meshList[GEO_CCTV]->textureID = LoadTGA("Image//UI//cctv.tga");

	meshList[GEO_CCTV_CORNER] = MeshBuilder::GenerateXYQuad("CCTV Overlay", Color(1,1,1), 4, 3, 1);
	meshList[GEO_CCTV_CORNER]->textureID = LoadTGA("Image//UI//cctv_corner.tga");

	meshList[GEO_LINE] = MeshBuilder::GenerateLine("crosshair", Color(0,1,0), 0.4f);

	Mtx44 projection;
	projection.SetToPerspective(60.f, 4.f / 3.f, 0.1f, 100000.f);
	projectionStack.LoadMatrix(projection);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0,50,0);
	light[0].power = 0.5f;

	light[1].type = Light::LIGHT_POINT;
	light[2].type = Light::LIGHT_POINT;
	light[2].kC = 0.05f;

	glUniform1i(m_parameters[U_NUMLIGHTS], 3);

	for (int i = 0; i < 3; i++)
	{
		glUniform3fv(m_parameters[U_LIGHT0_COLOR + 11*i], 1, &light[i].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER + 11*i], light[i].power);
		glUniform1f(m_parameters[U_LIGHT0_KC + 11*i], light[i].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL + 11*i], light[i].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ + 11*i], light[i].kQ);
		glUniform1i(m_parameters[U_LIGHT0_TYPE + 11*i], light[i].type);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF + 11*i], light[i].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER + 11*i], light[i].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT + 11*i], light[i].exponent);
	}

	textbox = NULL;
}

long double x;
std::string fps;
float animateHeart = 0.f;
static int animateHeartDir = 1.f;

Vector3 Select;
float selectDelay;
extern float pauseDelay;

bool CCTV = false;
bool stopCamera = false;

void PROJECTScene::Update(double dt)
{
	if(pauseDelay > 0)
		pauseDelay -= float(dt);
	else
		pauseDelay = 0;

	if (!pause)
	{
	if(Application::IsKeyPressed('R'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (camera == &player.camera)
	{

	camera->lookAt = camera->lookingAt(object, 120);

	if (player.holding < 0)
	if ((Application::IsKeyPressed('E')) && inputDelay == 0)
	{
		if (object[camera->lookAt]->type == "Item")
		{
			if (player.inventory.Insert(object[camera->lookAt]))
			{	
				inputDelay = 0.2f;

				Vector3 tPos = object[camera->lookAt]->position + object[camera->lookAt]->collision.centre;
				text.push_back( new OnScreenText("+1 " + object[camera->lookAt]->mesh->name,tPos) );
				object.erase(object.begin()+camera->lookAt);
			}
		}
		else if (object[camera->lookAt]->type == "Storage")
		{
			Object * newObject;
			if (!player.inventory.selector.selectedSlot->item.empty())
				newObject = player.inventory.selector.selectedSlot->item.back();

			if (player.inventory.Remove())
			{	
				inputDelay = 0.2f;

				newObject->position = object[camera->lookAt]->getStorePos(&player); newObject->position.y = object[camera->lookAt]->collision.hitbox.y/2 +  object[camera->lookAt]->collision.centre.y + object[camera->lookAt]->position.y;
				object.push_back(newObject);

				Vector3 tPos = newObject->position + newObject->collision.centre;
				text.push_back( new OnScreenText("-1 " + newObject->mesh->name,tPos) );
			}
		}
		else if (object[camera->lookAt]->type == "Vending Machine")
		{
			
			inputDelay = 0.2f;

			if (Machine.drink())
				player.inventory.wallet.trueValue -= 2.f;
				
		}

		else if (object[camera->lookAt] == &Camera.Look)
		{

			CCTV = true;
			stopCamera = true;

			inputDelay = 0.2f;
		}

		else if (object[camera->lookAt] == &Camera.Looks)
		{

			CCTV = true;
			stopCamera = true;

			inputDelay = 0.2f;
		}

		else if (object[camera->lookAt]->type == "Money")
		{
			player.inventory.wallet.trueValue += object[camera->lookAt]->getValue();

			inputDelay = 0.2f;
			Vector3 tPos = Vector3(-15.f, 2.5f, 0);
			string add = "+$";
			add += to_string(long double(object[camera->lookAt]->getValue()));
			text2D.push_back( new OnScreenText(add, tPos) );

			delete object[camera->lookAt];
			object.erase(object.begin()+camera->lookAt);
		}
		else if(object[camera->lookAt] == &Bank.Withdraw)
		{
			inputDelay = 0.2f;

			if (Bank.withdraw(player.inventory.wallet))
			{
				Vector3 tPos = Vector3(-15.f, 2.5f, 0);
				string add = "+$1";
				text2D.push_back( new OnScreenText(add, tPos) );
			}
		}
		else if(object[camera->lookAt] == &Bank.Deposit)
		{
			inputDelay = 0.2f;

			if (Bank.deposit(player.inventory.wallet))
			{
				Vector3 tPos = Vector3(-15.f, 1.5f, 0);
				string add = "-$1";
				text2D.push_back( new OnScreenText(add, tPos, true) );
			}
		}
		camera->lookAt = camera->lookingAt(object, 120);
	}

	if (textbox != NULL)
	{
		textbox->Update();
	
		if ( (textbox->type == "Quest" || textbox->type == "Checkout" ) && selectDelay == 0)
		{
			if(Application::IsKeyPressed(VK_LEFT))
			{
				selectDelay = 0.1f;
				Select = textbox->getAccept().position;
			}
			else if (Application::IsKeyPressed(VK_RIGHT))
			{
				selectDelay = 0.1f;
				Select = textbox->getDecline().position;
			}
		}
		
		if ((Application::IsKeyPressed('E') || Application::IsKeyPressed(VK_RETURN)) && inputDelay == 0 )
		{
			inputDelay = 0.2f;

			if (textbox->apparentext.size() < textbox->text.size())
			{
				textbox->apparentext = textbox->text;

				if (textbox->type == "Quest" || textbox->type == "Checkout" )
					Select = textbox->getAccept().position;
			}
			else
			{
				if (textbox->type == "Quest")
				{
					textbox->apparentext.clear();

					if (Select == textbox->getAccept().position)
					{
						textbox->triggerQuest();
						textbox = textbox->getAccept().next;
					}
					else
						textbox = textbox->getDecline().next;
				}
				else if (textbox->type == "Checkout")
				{
					textbox->apparentext.clear();

					if (Select == textbox->getAccept().position)
					{
						float totalPrice = player.inventory.checkPrice();

						if (player.inventory.Checkout())
						{
							textbox = textbox->getAccept().next;

							Vector3 tPos = Vector3(-15.f, 1.5f, 0);
							string add = "-$"; add += to_string (long double (totalPrice) );
							text2D.push_back( new OnScreenText(add, tPos, true) );
						}
						else
						{
							textbox = textbox->getAccept().altNext;
						}
					}
					else
						textbox = textbox->getDecline().next;
				}
				else
				{
					textbox->apparentext.clear();
					textbox = NULL;
					player.inConversation = false;
					stopCamera = false;
				}
			}
		}
	}
	
	}

	player.Update(dt, object);

	doorway.ButtonUpdate(dt, object, &player);
	doorway2.ButtonUpdate(dt, object, &player);
	doorway.Update(dt, object, &player);
	doorway2.Update(dt, object, &player);

	AutoDoor.RangeUpdate(dt, object, &player);
	AutoDoor.Update(dt, object, &player);

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i]->type == "Dynamic" || object[i]->type == "NPC" || object[i]->type == "Player")
		{
			if(Object::checkCollision(&doorway.Range, object[i]) == true && doorway.close == true)
			{
				object[i]->position.y = 27;
				doorway2.open = true;
				doorway2.close = false;
			}
			else if(Object::checkCollision(&doorway2.Range, object[i]) == true && doorway2.close == true)
			{
				object[i]->position.y = 0;
				doorway.open = true;
				doorway.close = false;
			}

			if (object[i]->type == "Dynamic")
				object[i]->Update(dt, object, &player);
		}
	}

	for (unsigned int i = 0; i < character.size(); i++)
	{
		character[i]->Update(dt, object, &player);

		if (textbox == NULL)
			character[i]->inConversation = false;

		if (camera == &player.camera)
		if (object[camera->lookAt] == character[i])
			if (Application::IsKeyPressed('E') && inputDelay == 0)
			{
				if (!character[i]->inConversation)
				{
					inputDelay = 0.2f;
					textbox = character[i]->getConversation(&player);

					if (textbox != NULL)
					{
						player.inConversation = true;
						stopCamera = true;

						if (textbox->type == "Checkout")
						{
							character[i]->InitQuest("Filestream//Quests//cashier.txt");
							textbox->text = textbox->GenerateText(player.inventory);
						}
					}
				}
			}
	}

	for (unsigned int i = 0; i < text.size(); i++)
	{
		text[i]->Update(dt);

		if (text[i]->elapsedTime > 0.8f)
		{
			delete text[i];
			text.erase(text.begin()+i);
		}
	}

	for (unsigned int i = 0; i < text2D.size(); i++)
	{
		text2D[i]->Update(dt);

		if (text2D[i]->elapsedTime > 0.7f)
		{
			delete text2D[i];
			text2D.erase(text2D.begin()+i);
		}
	}

	for (unsigned int i = 0; i < blood.size(); i++)
	{
		blood[i]->Update(dt);
		if (blood[i]->elapsedTime > 0.5f)
		{
			delete blood[i];
			blood.erase(blood.begin() + i);
		}
	}

	fps = '\0';
	x = 1/dt;
	fps += "FPS:";
	fps += to_string(x);
	
	if (selectDelay > 0)
		selectDelay -= float(dt);
	else
		selectDelay = 0;

	if (inputDelay > 0)
		inputDelay -= float(dt);
	else
		inputDelay = 0;

	float animateHeartSpeed = 0;
	
	if (player.getHealth() > 0)
		animateHeartSpeed = 0.3f * (100/player.getHealth());
	else
		animateHeart = 0;

	if(animateHeart * animateHeartDir > 0.25f)
		animateHeartDir = -animateHeartDir;
	animateHeart += (float)(animateHeartDir * animateHeartSpeed * dt);

	if (!CCTV)
	{
		if (camera == &CCTVs[0] || camera == &CCTVs[1] || camera == &CCTVs[2] || camera == &CCTVs[3])
		{
			camera->Zoom(dt);
			camera->Move(dt);
			camera->Update(dt);

			if(Application::IsKeyPressed(VK_BACK) && inputDelay == 0)
			{
				CCTV = true;
				inputDelay = 0.2f;
			}
		}
	}
	else if (CCTV && inputDelay == 0)
	{
		if (Application::IsKeyPressed('1'))
		{
			glViewport(0, 0, 880, 660);
			camera = &CCTVs[0];
			CCTV = false;
			inputDelay = 0.2f;
		}
		else if (Application::IsKeyPressed('2'))
		{
			glViewport(0, 0, 880, 660);
			camera = &CCTVs[1];
			CCTV = false;
			inputDelay = 0.2f;
		}
		else if (Application::IsKeyPressed('3'))
		{
			glViewport(0, 0, 880, 660);
			camera = &CCTVs[2];
			CCTV = false;
			inputDelay = 0.2f;
		}
		else if (Application::IsKeyPressed('4'))
		{
			glViewport(0, 0, 880, 660);
			camera = &CCTVs[3];
			CCTV = false;
			inputDelay = 0.2f;
		}

		else if (Application::IsKeyPressed(VK_BACK))
		{
			glViewport(0, 0, 880, 660);
			camera = &player.camera;
			CCTV = false;
			stopCamera = false;

			inputDelay = 0.2f;
		}
	}

	}
}

void PROJECTScene::RenderScene()
{
	for (int i = 0; i < 3; i++)
	{
		if(light[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[i].position.x, light[i].position.y, light[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i*11], 1, &lightDirection_cameraspace.x);
		}
		else if(light[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i*11], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + i*11], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i*11], 1, &lightPosition_cameraspace.x);
		}
	}

	if(Application::IsKeyPressed('T'))
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera->target);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(camera->position);
	modelStack.Scale(500);
	RenderSkybox();
	modelStack.PopMatrix();

	for (unsigned int i = 0; i < character.size(); i++)
	{
		for (int j = 0; j < character[i]->NUM_BODYPARTS; j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(character[i]->position);
			modelStack.Rotate(character[i]->orientation, 0, 1, 0); 

			modelStack.Translate(character[i]->bodyParts[j].collision.centre);
			if (j != character[i]->HEAD)
			{
				modelStack.Translate(0,character[i]->bodyParts[j].collision.hitbox.y/2, 0);
				modelStack.Rotate(character[i]->rotation[j], 1, 0, 0); 
				modelStack.Translate(0,-character[i]->bodyParts[j].collision.hitbox.y/2, 0);
			}
			else
			{
				modelStack.Translate(0,-character[i]->bodyParts[j].collision.hitbox.y/2, 0);
				modelStack.Rotate(character[i]->rotation[j], 1, 0, 0); 
				modelStack.Translate(0,character[i]->bodyParts[j].collision.hitbox.y/2, 0);
			}
			modelStack.Translate(-(character[i]->bodyParts[j].collision.centre));

			RenderMesh(character[i]->bodyParts[j].mesh, true);
			modelStack.PopMatrix();
		}
	}

	if(!Application::IsKeyPressed('Q'))
	{
		for (unsigned int i = 1; i < object.size(); i++)
		{
			if (object[i]->mesh != NULL)
			{
				modelStack.PushMatrix();
				modelStack.Translate(object[i]->position);

				modelStack.Translate(object[i]->collision.centre);
				modelStack.Rotate(object[i]->orientation, 0, 1, 0); 
				modelStack.Translate(-object[i]->collision.centre);

				RenderMesh(object[i]->mesh, true);
				modelStack.PopMatrix();
			}
		}
	}

	for (unsigned int i = 0; i < blood.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(blood[i]->position);
		modelStack.Rotate(camera->orientation, 0, 1, 0); 
		modelStack.Rotate(-camera->look, 1, 0, 0); 
		modelStack.Rotate(90, 1, 0, 0); 
		RenderMesh(blood[i]->mesh, false);
		modelStack.PopMatrix();
	}
	
	if(Application::IsKeyPressed('Q'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

		for (unsigned int i = 0; i < object.size(); i++)
		{
			if (object[i]->type != "Bodypart" && object[i]->collision.boundingBox != NULL)
			{
				modelStack.PushMatrix();
				modelStack.Translate(object[i]->position);
				modelStack.Translate(object[i]->collision.centre);
				RenderMesh(object[i]->collision.boundingBox, false);
				modelStack.PopMatrix();
			}
		}
		modelStack.PushMatrix();
		modelStack.Translate(player.position);
		modelStack.Translate(player.collision.centre);
		RenderMesh(player.collision.boundingBox, false);
		modelStack.PopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y,
	light[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	if (player.holding >= 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0,-0.025f,0);
		modelStack.Translate(object[player.holding]->position);
		modelStack.Rotate(object[player.holding]->orientation,0,1,0);
		modelStack.Scale(object[player.holding]->collision.hitbox.x + 0.05f, object[player.holding]->collision.hitbox.y + 0.05f, object[player.holding]->collision.hitbox.z + 0.05f);
		RenderMesh(meshList[GEO_HOLD], false);
		modelStack.PopMatrix();
	}

	glDisable(GL_DEPTH_TEST);

	for (unsigned int i = 0; i < text.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(text[i]->textPos);
		modelStack.Rotate(camera->orientation, 0,1,0);
		modelStack.Rotate(-camera->look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(0.5f);
		float textLength = getTextWidth(text[i]->name);
		modelStack.Translate(-textLength/2, 0, 0);
		RenderText(meshList[GEO_TEXT], text[i]->name, Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	if (camera->lookAt > 0)
	{
	if (object[camera->lookAt]->type == "NPC")
	{
		modelStack.PushMatrix();
		modelStack.Translate(object[camera->lookAt]->position);
		modelStack.Translate(0,7.f,0);
		modelStack.Rotate(camera->orientation, 0,1,0);
		modelStack.Rotate(-camera->look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(0.5f);
		float textLength = getTextWidth(object[camera->lookAt]->getIdentity());
		modelStack.Translate(-textLength/2 + 0.1f, 0, 0);
		RenderText(meshList[GEO_TEXT], object[camera->lookAt]->getIdentity(), Color(1, object[camera->lookAt]->getHealth()/100, object[camera->lookAt]->getHealth()/100));
		modelStack.PopMatrix();
	}
	else if (object[camera->lookAt]->type == "ATM")
	{
		string P_Money;
		long double m = Bank.getSavings();
		P_Money += "Savings:";
		P_Money += to_string(m);

		modelStack.PushMatrix();
		modelStack.Translate(object[camera->lookAt]->position);
		modelStack.Translate(0,7.f,0);
		modelStack.Rotate(camera->orientation, 0,1,0);
		modelStack.Rotate(-camera->look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(0.5f);
		float textLength = getTextWidth(P_Money);
		modelStack.Translate(-textLength/2 + 0.1f, 0, 0);
		RenderText(meshList[GEO_TEXT], P_Money, Color(1, 1, 1));
		modelStack.PopMatrix();
	}
	else if (object[camera->lookAt]->type == "Vending Machine")
	{
		string P_Drink;
		long double m = Machine.getDrinks();
		P_Drink += "Drink:";
		P_Drink += to_string(m);

		modelStack.PushMatrix();
		modelStack.Translate(object[camera->lookAt]->position);
		modelStack.Translate(0,9.f,0);
		modelStack.Rotate(camera->orientation, 0,1,0);
		modelStack.Rotate(-camera->look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(1.0f);
		float textLength = getTextWidth(P_Drink);
		modelStack.Translate(-textLength/2 + 0.1f, 0, 0);
		RenderText(meshList[GEO_TEXT], P_Drink, Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	}
	glEnable(GL_DEPTH_TEST);
}

void PROJECTScene::RenderCCTVUI(int number)
{
	Mtx44 projection;

	glDisable(GL_DEPTH_TEST);

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	projection.SetToOrtho(-16, 16, -12, 12, -20, 20);
	projectionStack.LoadMatrix(projection);

	modelStack.PushMatrix();
	modelStack.Scale(1.5f);
	RenderMesh(meshList[GEO_CCTV], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-12,9,0);
	modelStack.Scale(1.5f);
	RenderMesh(meshList[GEO_CCTV_CORNER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-12,-9,0);
	modelStack.Rotate(90,0,0,1);
	modelStack.Scale(1.5f);
	RenderMesh(meshList[GEO_CCTV_CORNER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(12,-9,0);
	modelStack.Rotate(180,0,0,1);
	modelStack.Scale(1.5f);
	RenderMesh(meshList[GEO_CCTV_CORNER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(12,9,0);
	modelStack.Rotate(-90,0,0,1);
	modelStack.Scale(1.5f);
	RenderMesh(meshList[GEO_CCTV_CORNER], false);
	modelStack.PopMatrix();

	string Cam = "Camera ";
	Cam += to_string(long double(number));

	modelStack.PushMatrix();
	modelStack.Translate(0,10,0);
	float textLength = getTextWidth(Cam);
	modelStack.Scale(1.5f);
	modelStack.Translate(-textLength/2, 0, 0);
	RenderText(meshList[GEO_TEXT], Cam , Color(1, 1, 1));
	modelStack.PopMatrix();

	if (!CCTV)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0,-10,0);
		float textLength = getTextWidth("Backspace to return");
		modelStack.Scale(0.7f);
		modelStack.Translate(-textLength/2, 0, 0);
		RenderText(meshList[GEO_TEXT], "Backspace to return" , Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	glEnable(GL_DEPTH_TEST);
}

void PROJECTScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Mtx44 projection;

	if (!CCTV)
	{
		modelStack.LoadIdentity();
		viewStack.LoadIdentity();
		viewStack.LookAt(camera->position.x, camera->position.y,
		camera->position.z, camera->target.x, camera->target.y,
		camera->target.z, camera->up.x, camera->up.y, camera->up.z);
		projection.SetToPerspective(camera->fov, 4.f / 3.f, 0.1f, 100000.f);
		projectionStack.LoadMatrix(projection);

		RenderScene();
	}

	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
				glViewport(0, 330, 440, 330);
			if (i == 1)
				glViewport(440, 330, 440, 330);
			if (i == 2)
				glViewport(0, 0, 440, 330);
			if (i == 3)
				glViewport(440, 0, 440, 330);

			camera = &CCTVs[i];

			modelStack.LoadIdentity();
			viewStack.LoadIdentity();
			viewStack.LookAt(camera->position.x, camera->position.y,
				camera->position.z, camera->target.x, camera->target.y,
				camera->target.z, camera->up.x, camera->up.y, camera->up.z);
			projection.SetToPerspective(camera->fov, 4.f / 3.f, 0.1f, 100000.f);
			projectionStack.LoadMatrix(projection);

			RenderScene();
			RenderCCTVUI(i + 1);
		}
		
		glDisable(GL_DEPTH_TEST);

		modelStack.LoadIdentity();
		viewStack.LoadIdentity();
		projection.SetToOrtho(-16, 16, -12, 12, -20, 20);
		projectionStack.LoadMatrix(projection);

		glViewport(0, 0, 880, 660);

		modelStack.PushMatrix();
		modelStack.Translate(0,0,0);
		float textLength = getTextWidth("Backspace to return");
		modelStack.Scale(0.6f);
		modelStack.Translate(-textLength/2, 0, 0);
		RenderText(meshList[GEO_TEXT], "Backspace to return" , Color(1, 1, 1));
		modelStack.PopMatrix();
		
		glEnable(GL_DEPTH_TEST);
	}

	//2D

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	projection.SetToOrtho(-16, 16, -12, 12, -20, 20);
	projectionStack.LoadMatrix(projection);

	if (camera == &player.camera)
	{

	RenderCrosshair();

	if (!player.inventory.selector.selectedSlot->item.empty())
	{
		modelStack.PushMatrix();
		modelStack.Translate(player.value[player.bobbingX],-12 + player.value[player.bobbingY],-2);
		if (!player.isEating())
			modelStack.Translate(10,0,0);
		modelStack.Rotate(-45,0,1,0);
		modelStack.Rotate(-10,0,0,1);
		modelStack.Scale(10,10,10);
		RenderMesh(player.inventory.selector.selectedSlot->item[0]->mesh, true);
		modelStack.PopMatrix();
	}

	glDisable(GL_DEPTH_TEST);

	if (player.holding < 0)
	{
		if (object[camera->lookAt]->type == "Money" || object[camera->lookAt]->type == "Item" || (object[camera->lookAt]->mesh != NULL && object[camera->lookAt]->mesh->name == "Button"))
		{
			string tooltip;
			if (object[camera->lookAt]->mesh->name == "Button")
				tooltip += "E to push ";
			else if(object[camera->lookAt]->type == "Item")
				tooltip += "E to take ";
			else if(object[camera->lookAt]->type == "Money")
				tooltip += "E to loot ";

			if(object[camera->lookAt]->type == "Money")
			{
				tooltip += "$";
				tooltip += to_string(long double (object[camera->lookAt]->getValue()));
			}
			else
				tooltip += object[camera->lookAt]->mesh->name;

			if (object[camera->lookAt]->type == "Item")
			{
				tooltip += " ($";
				tooltip += to_string(long double (object[camera->lookAt]->getValue()));
				tooltip += ")";
			}
			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(tooltip);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],tooltip , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if(object[camera->lookAt]->type == "Storage" && !player.inventory.selector.selectedSlot->item.empty())
		{
			string tooltip = "E to place ";
			tooltip += player.inventory.selector.selectedSlot->item[0]->mesh->name;

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(tooltip);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],tooltip , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if(object[camera->lookAt]->type == "Dynamic")
		{
			string tooltip = "E to carry ";
			tooltip += object[camera->lookAt]->mesh->name;

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(tooltip);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],tooltip , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if(object[camera->lookAt]->type == "ATM")
		{
			string ATM_actions;
			if(object[camera->lookAt] == &Bank.Withdraw)
				ATM_actions = "E to withdraw $1";
			else if(object[camera->lookAt] == &Bank.Deposit)
				ATM_actions = "E to deposit $1";

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(ATM_actions);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],ATM_actions , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if(object[camera->lookAt]->type == "Vending Machine")
		{
			string Vending_actions;
			Vending_actions = "E to buy drink ($2)";

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(Vending_actions);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],Vending_actions , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if(object[camera->lookAt]->type == "CCTV - TV")
		{
			string TV_actions;
			if(object[camera->lookAt] == &Camera.Look || object[camera->lookAt] == &Camera.Looks)
				TV_actions = "E TO VIEW";

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(TV_actions);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],TV_actions , Color(1, 1, 1));
			modelStack.PopMatrix();
		}

	}
	
	if (textbox != NULL)
	{
		string firstline;
		string secondline;
		string lastline;

		for (int i = 0; i < textbox->apparentext.size(); i++)
		{
			if (i < 62)
				firstline += textbox->apparentext[i];
			else if (i < 62 + 62)
			{
				if (i == 62 && textbox->apparentext[i] == ' ')
					continue;
				secondline += textbox->apparentext[i];
			}
			else
			{
				if (i == 62 + 62 && textbox->apparentext[i] == ' ')
					continue;
				lastline += textbox->apparentext[i];
			}
		}

		modelStack.PushMatrix();
		modelStack.Translate(textbox->position);
		RenderMesh(meshList[GEO_TEXTBOX], false);
			modelStack.PushMatrix();
			modelStack.Translate(-14,2.5f,0);
			RenderText(meshList[GEO_TEXT], firstline, Color(1, 1, 1));
				modelStack.PushMatrix();
				modelStack.Translate(0,-1,0);
				RenderText(meshList[GEO_TEXT], secondline, Color(1, 1, 1));
					modelStack.PushMatrix();
					modelStack.Translate(0,-1,0);
					RenderText(meshList[GEO_TEXT], lastline, Color(1, 1, 1));
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		if ( (textbox->type == "Quest" || textbox->type == "Checkout") && textbox->apparentext == textbox->text)
		{
			modelStack.PushMatrix();
			modelStack.Translate(textbox->getAccept().position);
			if (Select == textbox->getAccept().position)
				RenderText(meshList[GEO_TEXT], textbox->getAccept().text, Color(1, 1, 1));
			else
				RenderText(meshList[GEO_TEXT], textbox->getAccept().text, Color(0.4f, 0.4f, 0.4f));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(textbox->getDecline().position);
			if (Select == textbox->getDecline().position)
				RenderText(meshList[GEO_TEXT], textbox->getDecline().text, Color(1, 1, 1));
			else
				RenderText(meshList[GEO_TEXT], textbox->getDecline().text, Color(0.4f, 0.4f, 0.4f));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(Select);
			modelStack.Translate(-0.6f,0,0);
			RenderText(meshList[GEO_TEXT], ">" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
	}

	for (int i = 0; i < 9; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(player.inventory.slots[i].position);
		RenderMesh(player.inventory.slots[i].mesh, false);
		modelStack.PopMatrix();
		if (!player.inventory.slots[i].item.empty())
		{
			string stack = to_string(long double(player.inventory.slots[i].item.size()));
			modelStack.PushMatrix();
			modelStack.Translate(player.inventory.slots[i].position - Vector3(0,0.5f,0));
			modelStack.Rotate(0, 0,1,0);
			modelStack.Rotate(-5, 1,0,0);
			//modelStack.Scale(0.5f,0.5f,0.5f);
			RenderMesh(player.inventory.slots[i].item[0]->mesh, false);
			RenderText(meshList[GEO_TEXT], stack, Color(1, 1, 1));
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(player.inventory.selector.selectedSlot->position);
	RenderMesh(player.inventory.selector.mesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-12,-10.5f,0);
	RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PushMatrix();
		modelStack.Translate(-2.5f,0,0);
		modelStack.Scale(1 + animateHeart);
		RenderMesh(meshList[GEO_HEART], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0,12.5,0);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(12,-10.5f,0);
	modelStack.Rotate(180,0,1,0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	string cash = "$ ";
	cash += to_string(long double(player.getRenderWallet()));
	modelStack.PushMatrix();
	modelStack.Translate(-14.8f,2,0);
	RenderText(meshList[GEO_TEXT], cash, Color(1, 1, 1));
	modelStack.PopMatrix();

	string health = to_string(long long(player.getHealth()));
	modelStack.PushMatrix();
	modelStack.Translate(-13.4f,-10.5f,0);
	RenderText(meshList[GEO_TEXT], health, Color(1, player.getHealth() / 100, player.getHealth() / 100));
	modelStack.PopMatrix();

	for (unsigned int i = 0; i < text2D.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(text2D[i]->textPos);
		RenderText(meshList[GEO_TEXT], text2D[i]->name, Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	string x = to_string(long double(camera->position.x));
	string y = to_string(long double(camera->position.y));
	string z = to_string(long double(camera->position.z));

	modelStack.PushMatrix();
	modelStack.Translate(-15,11,0);
	RenderText(meshList[GEO_TEXT], x, Color(1, 1, 1));
	modelStack.Translate(0,-1,0);
	RenderText(meshList[GEO_TEXT], y, Color(1, 1, 1));
	modelStack.Translate(0,-1,0);
	RenderText(meshList[GEO_TEXT], z, Color(1, 1, 1));
	modelStack.Translate(-1,-1,0);
	RenderText(meshList[GEO_TEXT], fps, Color(1, 1, 1));
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);

	}

	else if (!CCTV)
	{
		for (int i = 0; i < 4; i++)
		{
			if (camera == &CCTVs[i])
			{
				RenderCCTVUI(i + 1);
				break;
			}
		}
	}
}

void PROJECTScene::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() *
		modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1,GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1,&mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}
void PROJECTScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float textWidth = 0.f;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		if (text[i] == '.'|| text[i] == '\'' )
			textWidth -= 0.1f;
		else if (text[i] == 'l' || text[i] == 'i')
			textWidth -= 0.05f;
		else if (text[i] == 't')
			textWidth -= 0.1f;

		Mtx44 characterSpacing; 
		characterSpacing.SetToTranslation(textWidth, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);

		for (int j = '0'; j <= '9'; j++)
		{
			if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}
		
		for (int j = 'A'; j <= 'Z'; j++)
		{
			if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}

		for (int j = 'a'; j <= 'z'; j++)
		{
			if (text[i] == 'l')
			{
				textWidth -= 0.45f;
				break;
			}
			else if (text[i] == 'i')
			{
				textWidth -= 0.4f;
				break;
			}
			else if (text[i] == 't')
			{
				textWidth -= 0.35f;
				break;
			}
			else if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}

		if (text[i] == ' ' || text[i] == ',')
			textWidth -= 0.5f;
		else if (text[i] == '.' || text[i] == '\''|| text[i] == '(' || text[i] == ')')
			textWidth -= 0.4f;
		else if (text[i] == ':')
			textWidth -= 0.25f;
		else if (text[i] == '-' || text[i] == '+' || text[i] == '$')
			textWidth -= 0.3f;

		textWidth += 0.8f;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}
Vector3 Storage::getStorePos(Player* player)
{
	int reach = 0;
	Vector3 view = (player->camera.target - player->camera.position).Normalized(); view/=10;
	Object o(player->camera.position, Vector3(0,0,0), Vector3(0.1f,0.1f,0.1f));

	for (Vector3 p = player->camera.position; reach < 120; p += view)
	{
		o.position = p;
		if ( Object::checkCollision(this, &o) )
		{
			return p;
		}
		reach++;
	}

	return NULL;
}
void dynamicObject::Control(double dt, vector<Object*>object, Player* player)
{
	if (object[player->camera.lookAt] == this)
	{
		if (Application::IsKeyPressed('E') && player->holding < 0 && PROJECTScene::inputDelay == 0)
		{
			player->holding = player->camera.lookAt;
			PROJECTScene::inputDelay = 0.2f;
		}
	}

	if (player->holding >= 0)
	if (object[player->holding] == this)
	{
		position = player->camera.target;
		velocity.y = 0;

		if ((Application::IsKeyPressed('E') && player->camera.lookAt == player->holding && PROJECTScene::inputDelay == 0 ))
		{
			player->holding = -1;
			PROJECTScene::inputDelay = 0.2f;
		}
	}
	
}
float PROJECTScene::getTextWidth(string text)
{
	float textWidth = 0.f;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		if (text[i] == '.' || text[i] == '\'' )
			textWidth -= 0.1f;
		else if (text[i] == 'l' || text[i] == 'i')
			textWidth -= 0.05f;
		else if (text[i] == 't')
			textWidth -= 0.1f;

		for (int j = '0'; j <= '9'; j++)
		{
			if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}
		
		for (int j = 'A'; j <= 'Z'; j++)
		{
			if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}

		for (int j = 'a'; j <= 'z'; j++)
		{
			if (text[i] == 'l')
			{
				textWidth -= 0.45f;
				break;
			}
			else if (text[i] == 'i')
			{
				textWidth -= 0.4f;
				break;
			}
			else if (text[i] == 't')
			{
				textWidth -= 0.35f;
				break;
			}
			else if (text[i] == j)
			{
				textWidth -= 0.3f;
				break;
			}
		}

		if (text[i] == ' ' || text[i] == ',')
			textWidth -= 0.5f;
		else if (text[i] == '.' || text[i] == '\''|| text[i] == '(' || text[i] == ')')
			textWidth -= 0.4f;
		else if (text[i] == ':')
			textWidth -= 0.25f;
		else if (text[i] == '-' || text[i] == '+' || text[i] == '$')
			textWidth -= 0.3f;

		textWidth += 0.8f;
	}
	return textWidth;
}
void PROJECTScene::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void PROJECTScene::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0,0,-0.498f);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0, 0.498f);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.498f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.498f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(-90,0,0,1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0.498f,0);
	modelStack.Rotate(180,1,0,0);
	modelStack.Rotate(180,0,1,0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,-0.498f,0);
	modelStack.Rotate(90,0,1,0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}
void PROJECTScene::RenderCrosshair()
{
	modelStack.PushMatrix();
	modelStack.Translate(0,0.13f,0);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LINE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,-0.13f,0);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LINE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.13f,0,0);
	modelStack.Rotate(90,0,0,1);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LINE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.13f,0,0);
	modelStack.Rotate(90,0,0,1);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LINE], false);
	modelStack.PopMatrix();
}

void OnScreenText::Update(double dt)
{
	elapsedTime += float(dt);

	if (fall)
		textPos.y -= 0.03f;
	else
		textPos.y += 0.03f;
}
