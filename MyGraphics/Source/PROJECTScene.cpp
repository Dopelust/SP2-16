#include "PROJECTScene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

using namespace::std;

PROJECTScene::PROJECTScene()
{
}

PROJECTScene::~PROJECTScene()
{
}

bool hideKnife = false;

void PROJECTScene::InitJunk()
{
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
	
	tempMesh = MeshBuilder::GenerateOBJ("Shelf", "OBJ//shelf-2.obj"); tempMesh->textureID = LoadTGA("Image//shelf-blue.tga");
	hitBox = Vector3(8,6,3); cube = MeshBuilder::GenerateCube("ShelfHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int x = -11; x <= 11; x+=11)
	{	
		for (int z = -30; z >= -54; z-=12)
		{
			object.push_back( new Object(Vector3(x,0,z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube));
		}
	}

	tempMesh = MeshBuilder::GenerateOBJ("Chocolate", "OBJ//Food//chocolate.obj"); tempMesh->textureID = LoadTGA("Image//Food//chocolate.tga");
	hitBox = Vector3(1.5f, 0.25f, 1.5f); cube = MeshBuilder::GenerateCube("Chocolate", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(0,y,-20), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Cigarettes", "OBJ//Food//marlbororeds.obj"); tempMesh->textureID = LoadTGA("Image//Food//marlbororeds.tga");
	hitBox = Vector3(0.2f, 0.325f, 0.075f); cube = MeshBuilder::GenerateCube("Cigarettes", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(3,y,-20), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Cereal Box", "OBJ//Food//cerealbox.obj"); tempMesh->textureID = LoadTGA("Image//Food//cerealbox.tga");
	hitBox = Vector3(0.6f, 0.8f, 0.2f); cube = MeshBuilder::GenerateCube("CerealBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(-3,y,-20), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	
	tempMesh = MeshBuilder::GenerateQuad("", Color(1, 1, 1), 160.f, 130.f, 30); tempMesh->textureID = LoadTGA("Image//floor.tga");
	hitBox = Vector3(160, 0.1f, 130.f); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,-22.5), Vector3(0,0,0), hitBox, tempMesh, cube) );
}

void PROJECTScene::JeremiahInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	Vector3 hitBox2;
	float size = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Hobo Character~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	character.push_back( new Hobo());
	character.push_back( new Thug());

	for (unsigned int i = 0; i < character.size(); i++)
	{
		object.push_back( character[i] );
	}
	


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	hitBox = Vector3(2, 25, 132); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(81,hitBox.y/2,-22.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-81,hitBox.y/2,-22.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(72, 25, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(44,hitBox.y/2,42.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-44,hitBox.y/2,42.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	hitBox = Vector3(60, 25, 2); 
	tempMesh = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 100); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Wall1.tga");
	cube = MeshBuilder::GenerateCube("Wall", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(50,hitBox.y/2,-87.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-50,hitBox.y/2,-87.5f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	//~~~~~~~~~~~~~~~~~~~~~~~Tentative height of second floor~~~~~~~

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Super Market(Inside of SuperMarket)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	tempMesh = MeshBuilder::GenerateOBJ("SideWalk", "OBJ//BuildingOBJ//S_Walk.obj"); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Road&Pave.tga");
	hitBox = Vector3(130, 2, 26); cube = MeshBuilder::GenerateCube("SideWalk", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,-1.6,-100.5), Vector3(-0.1,hitBox.y/2,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("SideWalk", "OBJ//BuildingOBJ//Road.obj"); tempMesh->textureID = LoadTGA("Image//BuildingTGA//Road&Pave.tga");
	hitBox = Vector3(130, 2, 78); cube = MeshBuilder::GenerateCube("Road", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,-2,-152), Vector3(-0.1,hitBox.y/2,0), hitBox, tempMesh, cube) );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Cash Table~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	tempMesh = MeshBuilder::GenerateOBJ("SideWalk", "OBJ//cashtable.obj"); tempMesh->textureID = LoadTGA("Image//cashtable.tga");
	hitBox = Vector3(3, 3.69, 9); cube = MeshBuilder::GenerateCube("Cash Table", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(5,0,5), Vector3(-1.4,hitBox.y/2,-0.59), hitBox, tempMesh, cube) );
}

void PROJECTScene::JessicaInit()
{
	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	tempMesh = MeshBuilder::GenerateOBJ("ChocolateBar", "OBJ//Food//choco_bar.obj"); tempMesh->textureID = LoadTGA("Image//Food//Chocolate Bar texture.tga");
	hitBox = Vector3(0.8f, 0.2f, 0.8f); cube = MeshBuilder::GenerateCube("ChocoBoxHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(10,y,10), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	
	tempMesh = MeshBuilder::GenerateOBJ("Ice Cream", "OBJ//Food//ice.obj"); tempMesh->textureID = LoadTGA("Image//Food//ice_cream_box.tga");
	hitBox = Vector3(2.0f, 0.8f, 0.4f); cube = MeshBuilder::GenerateCube("IceHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(11,y,11), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	
	tempMesh = MeshBuilder::GenerateOBJ("FrozenPizza", "OBJ//Food//pizza.obj"); tempMesh->textureID = LoadTGA("Image//Food//frozen_pizza.tga");
	hitBox = Vector3(2.0f, 0.3f, 2.0f); cube = MeshBuilder::GenerateCube("PizzaHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(12,y,12), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}
	
	tempMesh = MeshBuilder::GenerateOBJ("MountianDEW", "OBJ//Food//mountaindew.obj"); tempMesh->textureID = LoadTGA("Image//Food//mountaindew.tga");
	hitBox = Vector3(0.3f, 0.5f, 0.3f); cube = MeshBuilder::GenerateCube("MDHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(13,y,13), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Coke", "OBJ//Food//can_drink.obj"); tempMesh->textureID = LoadTGA("Image//Food//Coke.tga");
	hitBox = Vector3(0.3f, 0.5f, 0.3f); cube = MeshBuilder::GenerateCube("C_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(2,y,2), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Sardines", "OBJ//Food//sardines.obj"); tempMesh->textureID = LoadTGA("Image//Food//Sarsinde.tga");
	hitBox = Vector3(0.7f, 0.4f, 0.6f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(15,y,15), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

	tempMesh = MeshBuilder::GenerateOBJ("Soup", "OBJ//Food//soup.obj"); tempMesh->textureID = LoadTGA("Image//Food//soup.tga");
	hitBox = Vector3(0.7f, 0.9f, 0.7f); cube = MeshBuilder::GenerateCube("S_Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	for (int y = 0; y <= 10; y++)
	{
		object.push_back( new Item(Vector3(9,y,9), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1, 0, true) );
	}

}

void PROJECTScene::DarrenInit()
{	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size = 0;

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Border", "OBJ//elevatorborder.obj"); tempMesh->textureID = LoadTGA("Image//silver.tga");
	hitBox = Vector3(1.75f, 40.25f, 3.5f); cube = MeshBuilder::GenerateCube("ElevatorBorderHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(6,0,20), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-6,0,20), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Button", "OBJ//button.obj"); tempMesh->textureID = LoadTGA("Image//buttonOff.tga");
	hitBox = Vector3(1, 2.1f, 1.4f); cube = MeshBuilder::GenerateCube("ButtonHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object ButtonIn(Vector3(-10,4,-1), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 0, true);
	Object ButtonOut(Vector3(5.5,4,3), Vector3(0,1.05f,0), hitBox, tempMesh, cube, 1, 90, true);

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Door", "OBJ//elevatordoor.obj"); tempMesh->textureID = LoadTGA("Image//elevatordoor.tga");
	hitBox = Vector3(5.3,8.5,1.2f); cube = MeshBuilder::GenerateCube("ElevatorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object Door(Vector3(0,0,0), Vector3(0,4.25,0), hitBox, tempMesh, cube);
	doorway.Init(Vector3(0,0,20), Door, ButtonIn, ButtonOut);
	object.push_back( &doorway.Door[0] );
	object.push_back( &doorway.Door[1] );
	object.push_back( &doorway.Button[0] );
	object.push_back( &doorway.Button[1] );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorright.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-13,0,25), Vector3(6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorleft.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(1, 9, 11); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(13,0,25), Vector3(-6.5,4.5f,-0.55f), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorbottom.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,25), Vector3(0,0,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatortop.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 1, 9); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,26), Vector3(0,9,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevatorback.obj"); tempMesh->textureID = LoadTGA("Image//white.tga");
	hitBox = Vector3(11, 9, 1); cube = MeshBuilder::GenerateCube("Elevator", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,34), Vector3(0,4,-5), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("atm", "OBJ//atm.obj"); tempMesh->textureID = LoadTGA("Image//atm.tga");
	hitBox = Vector3(2,7,3); cube = MeshBuilder::GenerateCube("atm", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(5, 0 ,0), Vector3(-0.1,hitBox.y/2,0.3), hitBox, tempMesh, cube) );

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

	camera.Init(Vector3(85,12,-10), Vector3(80, 12, -10), Vector3(0, 1, 0));

	RicssonInit();
	JeremiahInit();
	JessicaInit();
	DarrenInit();

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 0.1f, 0.1f, 0.1f, 1);
	meshList[GEO_BIGCUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 12.f, 12.f, 12.f, 1);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16.f, 16.f);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");
	
	meshList[GEO_ARM] = MeshBuilder::GenerateOBJ("Arm", "OBJ//CharOBJ//fpArm.obj");
	meshList[GEO_ARM]->textureID = LoadTGA("Image//CharTGA//Steve_Hobo.tga");

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
	if(Application::IsKeyPressed('R'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	camera.lookAt = camera.lookingAt(object);

	if ((Application::IsKeyPressed('E')) && object[camera.lookAt]->mesh != NULL)
	{
		if (object[camera.lookAt]->type == "Item")
		{
			if (player.inventory.Insert(object[camera.lookAt]))
			{				
				Vector3 tPos = object[camera.lookAt]->position + object[camera.lookAt]->collision.centre;
				lootedText.push_back( new OnScreenText(object[camera.lookAt]->mesh->name,tPos) );
				object.erase(object.begin()+camera.lookAt);

			}
		}

		else if (object[camera.lookAt]->mesh->name == "Button")
		{
			doorway.open = true; doorway.close = false;
			doorway.Button[0].mesh = doorway.buttonStatus[1]; doorway.Button[1].mesh = doorway.buttonStatus[1];
			doorway.elapsedTime = 0;
		}

	}
	
	if ((player.checkCollision(&doorway.Door[0]) || player.checkCollision(&doorway.Door[1])) && doorway.close == false)
	{
		doorway.open = true; doorway.close = false;
		doorway.Button[0].mesh = doorway.buttonStatus[1]; doorway.Button[1].mesh = doorway.buttonStatus[1];
		doorway.elapsedTime = 0;
	}

	player.Update(dt, object);
	camera.Update(dt, &player, object);
	doorway.Update(dt);

	for (unsigned int i = 0; i < character.size(); i++)
	{
		character[i]->Update(dt, object, &player);
	}


	for (unsigned int i = 0; i < lootedText.size(); i++)
	{
		lootedText[i]->Update(dt);

		if (lootedText[i]->elapsedTime > 0.8f)
		{
			delete lootedText[i];
			lootedText.erase(lootedText.begin()+i);
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
}

void PROJECTScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
	camera.position.z, camera.target.x, camera.target.y,
	camera.target.z, camera.up.x, camera.up.y, camera.up.z);
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
		modelStack.Translate(camera.target);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(camera.position);
	modelStack.Scale(500);
	RenderSkybox();
	modelStack.PopMatrix();

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

	for (unsigned int i = 0; i < character.size(); i++)
	{
		for (unsigned j = 0; j < character[i]->NUM_BODYPARTS; j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(character[i]->position);
			modelStack.Rotate(character[i]->orientation, 0, 1, 0); 
			RenderMesh(character[i]->bodyParts[j].mesh, true);
			modelStack.PopMatrix();
		}
	}
	for (unsigned int i = 0; i < blood.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(blood[i]->position);
		modelStack.Rotate(camera.orientation, 0, 1, 0); 
		modelStack.Rotate(-camera.look, 1, 0, 0); 
		modelStack.Rotate(90, 1, 0, 0); 
		RenderMesh(blood[i]->mesh, false);
		modelStack.PopMatrix();
	}
	
	if(Application::IsKeyPressed('Q'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

		for (unsigned int i = 0; i < object.size(); i++)
		{
			if (object[i]->type != "Bodypart")
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

	glDisable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < lootedText.size(); i++)
	{
		std::string text = "+1 "; text += lootedText[i]->name;

		modelStack.PushMatrix();
		modelStack.Translate(lootedText[i]->textPos);
		modelStack.Rotate(camera.orientation, 0,1,0);
		modelStack.Rotate(-camera.look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(0.5f);
		float textLength = getTextWidth(text);
		modelStack.Translate(-textLength/2, 0, 0);
		RenderText(meshList[GEO_TEXT], text, Color(1, 1, 1));
		modelStack.PopMatrix();
	}

	if (object[camera.lookAt]->type == "NPC")
	{
		modelStack.PushMatrix();
		modelStack.Translate(object[camera.lookAt]->position);
		modelStack.Translate(0,7.f,0);
		modelStack.Rotate(camera.orientation, 0,1,0);
		modelStack.Rotate(-camera.look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		modelStack.Scale(0.5f);
		float textLength = getTextWidth(object[camera.lookAt]->getIdentity());
		modelStack.Translate(-textLength/2 + 0.1f, 0, 0);
		RenderText(meshList[GEO_TEXT], object[camera.lookAt]->getIdentity(), Color(1, 1, 1));
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

	string x = to_string(long double(camera.position.x));
	string y = to_string(long double(camera.position.y));
	string z = to_string(long double(camera.position.z));

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
	
	if (object[camera.lookAt]->ignoreCollision && object[camera.lookAt]->mesh != NULL)
	{
		string tooltip;
		if (object[camera.lookAt]->mesh->name == "Button")
			tooltip += "E to push ";
		else
			tooltip += "E to loot ";
		tooltip += object[camera.lookAt]->mesh->name;

		modelStack.PushMatrix();
		modelStack.Translate(0.35f,-5,0);
		float textLength = getTextWidth(tooltip);
		modelStack.Translate(-textLength/2, 0, 0);
		modelStack.Scale(1,1,1);
		RenderText(meshList[GEO_TEXT],tooltip , Color(1, 1, 1));
		modelStack.PopMatrix();
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
			if (text[i] == 'l' || text[i] == 'i')
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
		
		textWidth += 0.8f;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
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
			if (text[i] == 'l' || text[i] == 'i')
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

void Object::Update(double dt)
{
}
void Doorway::Update(double dt)
{
	if (open == true)
	{
		elapsedTime += float(dt);

		if (elapsedTime > 3.f)
		{
			open = false;
			Button[0].mesh = buttonStatus[0]; Button[1].mesh = buttonStatus[0];
			elapsedTime = 0;
		}
		
		Door[0].position.x -= float(10.f * dt);
		Door[1].position.x += float(10.f * dt);

		if(Door[0].position.x <= doorPosition[0].x - Door[0].collision.hitbox.x)
			Door[0].position.x = doorPosition[0].x - Door[0].collision.hitbox.x;
		if(Door[1].position.x >= doorPosition[1].x + Door[1].collision.hitbox.x)
			Door[1].position.x = doorPosition[1].x + Door[1].collision.hitbox.x;
	}

	else
	{
		Door[0].position.x += float(10.f * dt);
		Door[1].position.x -= float(10.f * dt);

		if (Door[0].position.x >= doorPosition[0].x)
		{
			Door[0].position.x = doorPosition[0].x; close = true;
		}
		if (Door[1].position.x <= doorPosition[1].x)
		{
			Door[1].position.x = doorPosition[1].x; close = true;
		}
	}
}
void Particles::Update(double dt)
{
	position += direction * float(dt);
	elapsedTime += float(dt);
}
void OnScreenText::Update(double dt)
{
	elapsedTime += float(dt);

	if (elapsedTime < 0.8f)
	{
		textPos.y += 0.03f;
	}
}