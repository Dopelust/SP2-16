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
	
	tempMesh = MeshBuilder::GenerateOBJ("Shelf", "OBJ//LowPoly//shelf.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//Shelf.tga");
	hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float z = 0; z >= -45; z-=22.5f)
	{
		hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(0,0,z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));
		hitBox = Vector3(0.5f,6,3.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(10.25f,0,z), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-10.25f,0,z), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(20,0.25f,1.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(0,0,z + 1), Vector3(0,0.375,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,2,z + 1), Vector3(0,0.375,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,4,z + 1), Vector3(0,0.375,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,0,z - 1), Vector3(0,0.375,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,2,z - 1), Vector3(0,0.375,0), hitBox, cube));
		object.push_back( new Storage(Vector3(0,4,z - 1), Vector3(0,0.375,0), hitBox, cube));
	}

	for (int x = 0; x < 2; x++)
	{
		for (int z = -1; z < 5; z++)
		{
			Vector3 p(-50 + x * 18,0, z * -15);

			tempMesh = MeshBuilder::GenerateOBJ("Display Table", "OBJ//LowPoly//display.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//display.tga");
			hitBox = Vector3(6, 3.25f, 6); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			object.push_back( new Storage(p, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));

			p.y += hitBox.y;

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
					object.push_back( new Item(p + P, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
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
		object.push_back( new dynamicObject(Vector3(15,y,39), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube) );
	}
	for ( int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			Vector3 p(30 + x * 18,0, z * -15);

			tempMesh = MeshBuilder::GenerateOBJ("Display Table", "OBJ//LowPoly//display.obj"); tempMesh->textureID = LoadTGA("Image//LowPoly//display.tga");
			hitBox = Vector3(6, 3.25f, 6); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
			object.push_back( new Storage(p, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));

			p.y += hitBox.y;

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
					object.push_back( new Item(p + P, Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
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
	character.push_back( new Blindman() );
	character.push_back( new Detective () );
	for(int x = 72; x > 20; x -= 15)
	{
		character.push_back( new Cashier(Vector3(x,2,-55)) );
	}

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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market Entrance~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	
	tempMesh = MeshBuilder::GenerateOBJ("Auto Door", "OBJ//glass_d.obj"); tempMesh->textureID = LoadTGA("Image//GlassDoor.tga");
	hitBox = Vector3(20,25,1.25); cube = MeshBuilder::GenerateCube("AutoDoorHit_B", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object autoDoor(Vector3(0,0,0), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube);
	hitBox = Vector3(40,25,60); cube = MeshBuilder::GenerateCube("AutoDoorHit_B", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object autoRange(Vector3(0,0,0), Vector3(0,hitBox.y/2,0), hitBox, cube);
	AutoDoor.Init(Vector3(0,0,-87.5), autoDoor, autoRange);
	object.push_back( &AutoDoor.Door[0] );
	object.push_back( &AutoDoor.Door[1] );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Elevator~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
}
void PROJECTScene::JessicaInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	//~~~~~~~~~~~FOOD~~~~~~~~~~~~~~
	tempMesh = MeshBuilder::GenerateOBJ("FrozenPizza", "OBJ//Food//pizza.obj"); tempMesh->textureID = LoadTGA("Image//Food//frozen_pizza.tga");
	hitBox = Vector3(2.0f, 0.3f, 2.0f); cube = MeshBuilder::GenerateCube("PizzaHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,4.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,2.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	for (int x = -4; x <= 4; x+=2)
	{
		object.push_back( new Item(Vector3(x*2,0.5,-43.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Soup", "OBJ//Food//soup.obj"); tempMesh->textureID = LoadTGA("Image//Food//soup.tga");
	hitBox = Vector3(0.3f, 0.5f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,4.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,2.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	for (float x = -7.775; x <= 7.775; x+=3.f)
	{
		object.push_back( new Item(Vector3(x,0.5,-46.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-23.75), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Blue Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox2.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-21.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Green Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox3.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,-1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Brown Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox4.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,4.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,2.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}
	for (float x = -9; x <= 9; x+=4.5f)
	{
		object.push_back( new Item(Vector3(x,0.5,1.25), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	//~~~~~~~~~~ICE CREAM~~~~~~~~~~~

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Oreo", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//oreo.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,5,-85), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,2,-85), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Lime", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//lime.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -75; x <= -64; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}
	for (int x = -75; x <= -64; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}

	
	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Cheese", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//cheese.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -56; x <= -45; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 360, true) );
	}
	for (int x = -56; x <= -45; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Strawberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//strawberry.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = 0; x <= 11; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (int x = 0; x <= 11; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Blueberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//blueberry.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = 19; x <= 30.5; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,5,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (int x = 19; x <= 30.5; x+=2.f)
	{
		object.push_back( new Item(Vector3(-78,2,x), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream - Raspberry", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//Ice Cream//raspberry.tga");
	hitBox = Vector3(1.f, 0.7f, 0.3f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,5,40), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	for (int x = -68; x <= -57; x+=2.f)
	{
		object.push_back( new Item(Vector3(x,2,40), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, rand() % 180, true) );
	}
	//~~~~~~~~~~SHELF~~~~~~~~~~~~
	//Dimensions = 5 x 8.3 x 14
	tempMesh = MeshBuilder::GenerateOBJ("Shelf", "OBJ//NewShelf//coolershelf.obj"); tempMesh->textureID = LoadTGA("Image//NewShelf//Shelf_Cooler.tga");
	hitBox = Vector3(20,6,0.5f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int z = 0; z < 2; z++)
	{
		hitBox = Vector3(5,2.05f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,0,-69.45f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));
		hitBox = Vector3(5,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,4.65f + 0.3f,-69.45f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5.f,8.3f,0.3f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,0,-69.45f + 6.85f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-77.6f,0,-69.45f - 6.85f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5,1.f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,7.3f,-69.45f + z * 18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
	}

	for (int z = 0; z < 2; z++)
	{
		hitBox = Vector3(5,2.05f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,0,24.45f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));
		hitBox = Vector3(5,0.1f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Storage(Vector3(-77.6f,4.65f + 0.3f,24.45f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5.f,8.3f,0.3f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,0,24.45f + 6.85f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		object.push_back( new Object(Vector3(-77.6f,0,24.45f - 6.85f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
		hitBox = Vector3(5,1.f,13.4f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
		object.push_back( new Object(Vector3(-77.6f,7.3f,24.45f + z * -18.75f), Vector3(0,hitBox.y/2,0), hitBox, cube));
	}
	hitBox = Vector3(13.4f,2.05f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,0,39.1), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 90));
	hitBox = Vector3(13.4f,0.1f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,4.65f + 0.3f,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(0.3f,8.3f,5.f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f+6.8,0,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	object.push_back( new Object(Vector3(-62.5f-6.8,0,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(13.4f,1.f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f,7.3f,39.1), Vector3(0,hitBox.y/2,0), hitBox, cube));

	hitBox = Vector3(13.4f,2.05f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,0,-84.1), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 270));
	hitBox = Vector3(13.4f,0.1f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Storage(Vector3(-62.5f,4.65f + 0.3f,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(0.3f,8.3f,5.f); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f+6.8,0,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	object.push_back( new Object(Vector3(-62.5f-6.8,0,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	hitBox = Vector3(13.4f,1.f,5); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-62.5f,7.3f,-84.1), Vector3(0,hitBox.y/2,0), hitBox, cube));
	
	//~~~~~~POSTER~~~~~
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 7, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.55,5.5,-36), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.55,5.5,-25), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.55,15,-60), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 10, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice_cream.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.55,15,15), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//ice2.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.54,6.5,-15), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 7, 1); tempMesh->textureID = LoadTGA("Image//Poster//poster_icecream.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-79.55,5,-8), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 90, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//Golden.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-42,6,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 7, 9, 1); tempMesh->textureID = LoadTGA("Image//Poster//Golden.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-20,6,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//rocher.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-30,9,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//pizza.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(35,13,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );
	
	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 19, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//pepperoni.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(49,12,41.46), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//soup.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(19,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 15, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//soup.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(71,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 8, 1); tempMesh->textureID = LoadTGA("Image//Poster//new_soup.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(64,5,41.465), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 29, 20, 1); tempMesh->textureID = LoadTGA("Image//Poster//member.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(48,13,-86), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 ,0, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//shop-theft3.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(28,13,-88.6), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 12, 1); tempMesh->textureID = LoadTGA("Image//Poster//shop-theft3.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-28,13,-88.6), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 10, 8, 1); tempMesh->textureID = LoadTGA("Image//Poster//Teaser.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,15,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 20, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//milo.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-25,38,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

	tempMesh = MeshBuilder::GenerateXYQuad("", Color(1, 1, 1), 20, 15, 1); tempMesh->textureID = LoadTGA("Image//Poster//veg.tga");
	tempMesh->material.kShininess = 20.f;
	tempMesh->material.kSpecular.Set(0.8f,0.8f,0.8f);
	hitBox = Vector3(0, 0, 0); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(25,38,41.47), Vector3(0,-0.0001f,0), hitBox, tempMesh, cube, 1 , 180, true) );

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
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(12,0,46.8), Vector3(-6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorbottom.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,47), Vector3(0,-0.5,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatortop.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
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



	tempMesh = MeshBuilder::GenerateOBJ("atm", "OBJ//atm.obj"); tempMesh->textureID = LoadTGA("Image//atm.tga");
	hitBox = Vector3(2,7,3); cube = MeshBuilder::GenerateCube("atm", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-50, 0.5 ,-89.5), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false) );
	object.push_back( new Object(Vector3(50, 0.5 ,-89.5), Vector3(-0.1f,hitBox.y/2,0.3f), hitBox, tempMesh, cube,1,90,false) );

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

	meshList[GEO_LINE] = MeshBuilder::GenerateLine("crosshair", Color(0,1,0), 0.15f);

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

}

long double x;
std::string fps;

void PROJECTScene::Update(double dt)
{
	if (!pause)
	{

	if(Application::IsKeyPressed('R'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	camera->lookAt = camera->lookingAt(object);

	if (player.holding < 0)
	if ((Application::IsKeyPressed('E')) && object[camera->lookAt]->mesh != NULL && inputDelay == 0)
	{
		if (object[camera->lookAt]->type == "Item")
		{
			if (player.inventory.Insert(object[camera->lookAt]))
			{	
				inputDelay = 0.2f;

				Vector3 tPos = object[camera->lookAt]->position + object[camera->lookAt]->collision.centre;
				text.push_back( new OnScreenText("+1 " + object[camera->lookAt]->mesh->name,tPos) );
				object.erase(object.begin()+camera->lookAt);

				camera->lookAt = camera->lookingAt(object);
			}
		}
	}

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i]->type == "Storage")
		{
			if (camera->lookAt == i)
			{
				if (Application::IsKeyPressed('E') && inputDelay == 0)
				{
					Object * newObject;
					if (!player.inventory.selector.selectedSlot->item.empty())
						newObject = player.inventory.selector.selectedSlot->item.back();

					if (player.inventory.Remove())
					{	
						inputDelay = 0.2f;

						newObject->position = object[i]->getStorePos(&player); newObject->position.y = object[i]->collision.hitbox.y/2 +  object[i]->collision.centre.y + object[i]->position.y;
						object.push_back(newObject);

						camera->lookAt = camera->lookingAt(object);

						Vector3 tPos = object[camera->lookAt]->position + object[camera->lookAt]->collision.centre;
						text.push_back( new OnScreenText("-1 " + object[camera->lookAt]->mesh->name,tPos) );
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
	
	//bool lvl = 0;

	if(player.checkCollision(&doorway.Range) == true && doorway.close == true)
	{
		player.position.y = 27;
		doorway2.open = true;
		doorway2.close = false;
	}
	else if(player.checkCollision(&doorway2.Range) == true && doorway2.close == true)
	{
		player.position.y = 0;
		doorway.open = true;
		doorway.close = false;
	}


	AutoDoor.RangeUpdate(dt, object, &player);
	AutoDoor.Update(dt, object, &player);
	
	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i]->type == "Dynamic")
			object[i]->Update(dt, object, &player);
	}

	for (unsigned int i = 0; i < character.size(); i++)
	{
		character[i]->Update(dt, object, &player);
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
	
	if (inputDelay > 0)
		inputDelay -= float(dt);
	else
		inputDelay = 0;

	}
}
void PROJECTScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	viewStack.LookAt(camera->position.x, camera->position.y,
	camera->position.z, camera->target.x, camera->target.y,
	camera->target.z, camera->up.x, camera->up.y, camera->up.z);
	Mtx44 projection;
	projection.SetToPerspective(60.f, 4.f / 3.f, 0.1f, 100000.f);
	projectionStack.LoadMatrix(projection);
	
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

	if(!Application::IsKeyPressed('Q'))
	{
	for (unsigned int i = 0; i < object.size(); i++)
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
		RenderText(meshList[GEO_TEXT], object[camera->lookAt]->getIdentity(), Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	glEnable(GL_DEPTH_TEST);

	//2D
	modelStack.PushMatrix();
	viewStack.PushMatrix();
	projectionStack.PushMatrix();

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	projectionStack.Ortho(-10, 10, -10, 10, 0, 10);

	RenderCrosshair();

	projection.SetToOrtho(-16, 16, -12, 12, -20, 20);
	projectionStack.LoadMatrix(projection);

	if (!player.inventory.selector.selectedSlot->item.empty())
	{
		modelStack.PushMatrix();
		modelStack.Translate(10 + player.value[player.bobbingX],-12 + player.value[player.bobbingY],-2);
		modelStack.Rotate(-45,0,1,0);
		modelStack.Rotate(-10,0,0,1);
		modelStack.Scale(10,10,10);
		RenderMesh(player.inventory.selector.selectedSlot->item[0]->mesh, true);
		modelStack.PopMatrix();
	}
	else
	{/*
		modelStack.PushMatrix();
		modelStack.Translate(1 + player.value[player.bobbingX], -1 + player.value[player.bobbingY], -4);
		//modelStack.Rotate(-5,1,0,0);
		modelStack.Rotate(-95,0,1,0);
		//modelStack.Rotate(20,0,0,1);
		//modelStack.Scale(7.5,8,10);
		RenderMesh(meshList[GEO_ARM], true);
		modelStack.PopMatrix();*/
	}

	glDisable(GL_DEPTH_TEST);

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
	
	if (player.holding < 0)
	if (object[camera->lookAt]->mesh != NULL)
	{
		if (object[camera->lookAt]->type == "Item" || object[camera->lookAt]->mesh->name == "Button")
		{
			string tooltip;
			if (object[camera->lookAt]->mesh->name == "Button")
				tooltip += "E to push ";
			else if(object[camera->lookAt]->type == "Item")
				tooltip += "E to loot ";

			tooltip += object[camera->lookAt]->mesh->name;

			modelStack.PushMatrix();
			modelStack.Translate(0.35f,-5,0);
			float textLength = getTextWidth(tooltip);
			modelStack.Translate(-textLength/2, 0, 0);
			modelStack.Scale(1,1,1);
			RenderText(meshList[GEO_TEXT],tooltip , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
	}

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
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
		if (text[i] == '.')
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
				textWidth -= 0.5f;
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

		if (text[i] == ' ')
			textWidth -= 0.5f;
		else if (text[i] == '.')
			textWidth -= 0.4f;
		else if (text[i] == ':')
			textWidth -= 0.25f;
		else if (text[i] == '-')
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
		if (text[i] == '.')
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
				textWidth -= 0.5f;
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

		if (text[i] == ' ')
			textWidth -= 0.5f;
		else if (text[i] == '.')
			textWidth -= 0.4f;
		else if (text[i] == ':')
			textWidth -= 0.25f;
		else if (text[i] == '-')
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

	if (elapsedTime < 0.8f)
	{
		textPos.y += 0.03f;
	}
}
