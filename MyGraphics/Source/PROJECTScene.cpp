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

void PROJECTScene::Init()
{
	// Init VBO here
	
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDisable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	//Load vertex and fragment shaders
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

	camera.Init(Vector3(85,12,-10), Vector3(80, 12, -10), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Skybox//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Skybox//back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Skybox//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Skybox//right.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox//bottom.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Skybox//top.tga");

	Mesh* tempMesh;
	Mesh* cube;
	Vector3 hitBox;
	float size;

	hitBox = player.collision.hitbox; player.collision.boundingBox =  MeshBuilder::GenerateCube("PlayerHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);

	tempMesh = MeshBuilder::GenerateQuad("", Color(1, 1, 1), 200.f, 84.875f, 10); tempMesh->textureID = LoadTGA("Image//floor.tga");
	hitBox = Vector3(200, 0.1f, 85); cube = MeshBuilder::GenerateCube("FloorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,-22.4375), Vector3(0,0,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateQuad("Garfield", Color(1, 1, 1), 0, 0, 0);
	hitBox = Vector3(5, 9, 4.5f); cube =  MeshBuilder::GenerateCube("GarfieldHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	realGarfield.Init(Vector3(0,5.5f,60.25f), Vector3(0,2,-0.3f), hitBox, cube);
	object.push_back( new Object(realGarfield.position, realGarfield.collision.centre, hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateQuad("Garfield", Color(1, 1, 1), 0, 0, 0);
	hitBox = Vector3(5, 3, 10); cube =  MeshBuilder::GenerateCube("GarfieldHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	garfield.altInit(Vector3(-87.5,11,11), Vector3(0,0,1), hitBox, cube);
	object.push_back( new Object(garfield.position, garfield.collision.centre, hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateCone("Fixture", Color(0,0,0), 18, 18, 3);
	hitBox = Vector3(0,0,0); cube =  MeshBuilder::GenerateCube("Light[0]", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,26,60.25f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateSphere("Light", Color(1,1,0.75f), 18,18, 1);
	object.push_back( new Object(Vector3(0,25.5f,60.25f), Vector3(0,0,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("FishTank", "OBJ//fishtank.obj"); tempMesh->textureID = LoadTGA("Image//fishtank.tga");
	hitBox = Vector3(15, 14, 10); cube = MeshBuilder::GenerateCube("FishTankHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	fishtank.Init(Vector3(58.5,0,15), Vector3(0,7,0), hitBox, tempMesh, cube);
	object.push_back( &fishtank );

	hitBox = Vector3(15,2,10); light[2].position.Set(58.5, 14.75f,15.f); 
	tempMesh = MeshBuilder::GenerateOBJ("FishTank", "OBJ//fishtanktop.obj"); tempMesh->textureID = LoadTGA("Image//fishtank.tga");
	cube = MeshBuilder::GenerateCube("FishTankHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(light[2].position.x,light[2].position.y,light[2].position.z), Vector3(0,0,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Explosive", "OBJ//explosivebarrel.obj"); tempMesh->textureID = LoadTGA("Image//explosivebarrel.tga");
	hitBox = Vector3(4,7,4); cube = MeshBuilder::GenerateCube("ExplosiveHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(32.5,0,16), Vector3(0,3.5f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(68.75f,0,16), Vector3(0,3.5f,0), hitBox, tempMesh, cube, 1.f, 90.f) );
	object.push_back( new Object(Vector3(89.f,0,10), Vector3(0,3.5f,0), hitBox, tempMesh, cube, 1.f, 45.f) );

	tempMesh = MeshBuilder::GenerateOBJ("Crate", "OBJ//crate.obj"); tempMesh->textureID = LoadTGA("Image//crate.tga");
	hitBox = Vector3(6,6,6); cube = MeshBuilder::GenerateCube("CrateHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(46.5,0,15), Vector3(0,3,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(39.5,0,15.5), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, -15.f) );
	object.push_back( new Object(Vector3(43.5,6,15.5), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, 25.f) );
	object.push_back( new Object(Vector3(79.f,7.5f,15), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, -25.f) );
	object.push_back( new Object(Vector3(79.f,13.5f,15), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, 25.f) );
	object.push_back( new Object(Vector3(82.f,0,7), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, -25.f) );
	object.push_back( new Object(Vector3(73.f,0,7), Vector3(0,3,0), hitBox, tempMesh, cube, 1.f, 5.f) );

	tempMesh = MeshBuilder::GenerateOBJ("Steps", "OBJ//steps.obj"); tempMesh->textureID = LoadTGA("Image//whitefloor.tga");
	hitBox = Vector3(1.75f,1.75f,43.75f); cube = MeshBuilder::GenerateCube("StepsHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-38.5f,0,-2.5f), Vector3(0,0.875f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-40.25f,1.75f,-2.5f), Vector3(0,0.875f,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Floor", "OBJ//floor2.obj"); tempMesh->textureID = LoadTGA("Image//whitefloor.tga");
	hitBox = Vector3(53.375f,1.75f,43.75f); cube = MeshBuilder::GenerateCube("Floor2Hitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-67.8125f,3.5f,-2.5f), Vector3(0,0.875f,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Bed", "OBJ//bed.obj"); tempMesh->textureID = LoadTGA("Image//bed.tga");
	hitBox = Vector3(7, 4.5, 13); cube = MeshBuilder::GenerateCube("BedHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-87.5f,5.25f,12.75f), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateOBJ("Bedside Table", "OBJ//bedsidetable.obj"); tempMesh->textureID = LoadTGA("Image//bedsidetable.tga");
	size = 0.15f;
	hitBox = Vector3(45, 40, 35); hitBox *= size; cube = MeshBuilder::GenerateCube("BedHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-83,5.25f,13.5f), Vector3(22.25f * size,hitBox.y/2, 17.25f*size), hitBox, tempMesh, cube, size, 180) );

	tempMesh = MeshBuilder::GenerateOBJ("Chair", "OBJ//chair.obj"); tempMesh->textureID = LoadTGA("Image//wood.tga");
	hitBox = Vector3(5, 7.5f, 5); cube = MeshBuilder::GenerateCube("ChairHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-60,5.25f, 10), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube) );
	hitBox = Vector3(0,0,0); cube = MeshBuilder::GenerateCube("ChairHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,0,60), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube, 1.f, 180.f) );

	tempMesh = MeshBuilder::GenerateOBJ("Table", "OBJ//table.obj"); tempMesh->textureID = LoadTGA("Image//blackwood.tga");
	hitBox = Vector3(15, 7.5f, 9); cube = MeshBuilder::GenerateCube("TableHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-60,5.25f, 15), Vector3(0,3.75f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(79,0,14), Vector3(0,3.75f,0), hitBox, tempMesh, cube, 1.f, 5.f) );

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Border", "OBJ//elevatorborder.obj"); tempMesh->textureID = LoadTGA("Image//silver.tga");
	hitBox = Vector3(1.75f, 40.25f, 3.5f); cube = MeshBuilder::GenerateCube("ElevatorBorderHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(9.625,0,20), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );
	object.push_back( new Object(Vector3(-9.625,0,20), Vector3(0,20.125f,0), hitBox, tempMesh, cube) );

	tempMesh = MeshBuilder::GenerateTorus("Rope", Color(0,0,0), 8, 8, 1.75f, 1.5f);
	hitBox = Vector3(0,0,0); cube = MeshBuilder::GenerateCube("RopeHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(0,5.5,61), Vector3(0,hitBox.y/2, 0), hitBox, tempMesh, cube, true, 0) );

	tempMesh = MeshBuilder::GenerateOBJ("Laptop", "OBJ//laptop.obj"); tempMesh->textureID = LoadTGA("Image//laptop.tga");
	hitBox = Vector3(4, 3, 3); cube = MeshBuilder::GenerateCube("LaptopHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-60,12.75f, 15), Vector3(0,1.5f,-1), hitBox, tempMesh, cube, true, 0) );
	object.push_back( new Object(Vector3(82,6,8), Vector3(0,1.5f,-1), hitBox, tempMesh, cube, true, 20) );

	tempMesh = MeshBuilder::GenerateOBJ("Mountain Dew", "OBJ//can.obj"); tempMesh->textureID = LoadTGA("Image//MountainDew.tga");
	hitBox = Vector3(0.75f,1.2f,0.75f); cube = MeshBuilder::GenerateCube("CanHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-57,12.75f, 13), Vector3(0,0.6f, 0), hitBox, tempMesh, cube, true, 0) );
	object.push_back( new Object(Vector3(74,7.5f, 13), Vector3(0,0.6f, 0), hitBox, tempMesh, cube, true, 0) );
	object.push_back( new Object(Vector3(48,6, 14), Vector3(0,0.6f, 0), hitBox, tempMesh, cube, true, 0) );

	tempMesh = MeshBuilder::GenerateOBJ("Doritos", "OBJ//doritos.obj"); tempMesh->textureID = LoadTGA("Image//doritos.tga");
	hitBox = Vector3(1.5, 0.5, 2); cube = MeshBuilder::GenerateCube("DoritosHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-64,12.75f, 14), Vector3(0,0.25f, 0), hitBox, tempMesh, cube, true, 180) );
	object.push_back( new Object(Vector3(62, 15.625,17.f), Vector3(0,0.25f, 0), hitBox, tempMesh, cube, true, 180) );
	object.push_back( new Object(Vector3(68, 0, 11), Vector3(0,0.25f, 0), hitBox, tempMesh, cube, true, 160) );

	tempMesh = MeshBuilder::GenerateOBJ("Cereal Box", "OBJ//cereal.obj"); tempMesh->textureID = LoadTGA("Image//cerealbox.tga");
	hitBox = Vector3(1.5f, 2, 0.5f); cube = MeshBuilder::GenerateCube("CerealHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(-63,12.75f, 17), Vector3(0,1,0), hitBox, tempMesh, cube, true, 180) );
	object.push_back( new Object(Vector3(39,6, 17), Vector3(0,1,0), hitBox, tempMesh, cube, true, 200) );

	tempMesh = MeshBuilder::GenerateOBJ("Button", "OBJ//button.obj"); tempMesh->textureID = LoadTGA("Image//buttonOff.tga");
	hitBox = Vector3(1.4f, 2.1f, 0.4f); cube = MeshBuilder::GenerateCube("ButtonHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object ButtonIn(Vector3(-13,8,-1), Vector3(0,1.05f,0), hitBox, tempMesh, cube, true, 0);
	Object ButtonOut(Vector3(-13,8,1), Vector3(0,1.05f,0), hitBox, tempMesh, cube, true, 180);

	tempMesh = MeshBuilder::GenerateOBJ("Elevator Door", "OBJ//elevatordoor.obj"); tempMesh->textureID = LoadTGA("Image//elevatordoor.tga");
	hitBox = Vector3(8.8f,18,1.2f); cube = MeshBuilder::GenerateCube("ElevatorHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	Object Door(Vector3(0,0,0), Vector3(0,9,0), hitBox, tempMesh, cube);
	doorway.Init(Vector3(0,0,20), Door, ButtonIn, ButtonOut);

	object.push_back( &doorway.Door[0] );
	object.push_back( &doorway.Door[1] );
	object.push_back( &doorway.Button[0] );
	object.push_back( &doorway.Button[1] );
	
	hitBox = Vector3(2,4,2); light[1].position.Set(-79.625, 10.9f + hitBox.y/2 ,16.f); 
	tempMesh = MeshBuilder::GenerateOBJ("Lamp", "OBJ//lamp.obj"); tempMesh->textureID = LoadTGA("Image//lamp.tga");
	tempMesh->material.kAmbient.Set(0.5f,0.5f,0.5f);
	cube = MeshBuilder::GenerateCube("LampHitbox", Color(1,1,1), hitBox.x, hitBox.y, hitBox.z, 0);
	object.push_back( new Object(Vector3(light[1].position.x,light[1].position.y - hitBox.y/2,light[1].position.z), Vector3(0,hitBox.y/2,0), hitBox, tempMesh, cube) );

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1,1,1), 36, 36, 2);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 0.1f, 0.1f, 0.1f, 1);
	meshList[GEO_BIGCUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 36.f, 36.f, 36.f, 1);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	
	meshList[GEO_KNIFE] = MeshBuilder::GenerateOBJ("knife", "OBJ//arm2.obj");
	meshList[GEO_KNIFE]->textureID = LoadTGA("Image//arm.tga");

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

//static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

long double x;
std::string fps;

void PROJECTScene::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		hideKnife = false;
	if(Application::IsKeyPressed('2'))
		hideKnife = true;

	if(Application::IsKeyPressed('R'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	camera.lookAt = camera.lookingAt(object);
	
	if ((Application::IsKeyPressed('E')))
	{
		if (object[camera.lookAt]->ignoreCollision)
		{
			if (object[camera.lookAt]->mesh->name != "Button")
			{
				Vector3 tPos = object[camera.lookAt]->position; 
				tPos += object[camera.lookAt]->collision.centre;
				player.loot.push_back( new Loot(object[camera.lookAt]->mesh->name,tPos) );

				player.inventory.slots[player.inventory.emptySlot()].item.push_back(object[camera.lookAt]);
				object.erase(object.begin()+camera.lookAt);
			}

			else if (object[camera.lookAt]->mesh->name == "Button")
			{
				doorway.open = true; doorway.close = false;
				doorway.Button[0].mesh = doorway.buttonStatus[1]; doorway.Button[1].mesh = doorway.buttonStatus[1];
				doorway.elapsedTime = 0;
			}
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
	fishtank.Update(dt);
	doorway.Update(dt);

	Vector3 view = (camera.target - camera.position).Normalized(); view/=10;
	Vector3 intersectionPoint;

	for (unsigned int i = 0; i < object.size(); i++)
	{
		int reach = 0;

		if (object[i]->mesh->name == "Garfield")
		{
			for (Vector3 p = camera.position; reach <= 180; p += view)
			{
				Object o(p, Vector3(0,0,0), Vector3(0.1f,0.1f,0.1f));
				if ( PROJECTScene::checkCollision(object[i], &o) )
				{
					intersectionPoint = o.position;
					break;
				}
				reach++;
			}
		}
	}
	if (player.value[player.knifeStrike] == 1 && intersectionPoint != Vector3(0,0,0))
	{
		int r = rand() % 30 + 10;
		for (int i = 0; i < r; i++)
		{
			blood.push_back(new Particles(intersectionPoint));
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
	fps += "FPS: ";
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
	
	modelStack.PushMatrix();
	modelStack.Translate(garfield.position);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-80,1,0,0);
	modelStack.Scale(0.35f);
	RenderGarfield(garfield, -5, -2);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(realGarfield.position);
	modelStack.Rotate(180,0,1,0);
	modelStack.Scale(0.35f);
	RenderGarfield(realGarfield, -90, 2);
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

			if (object[i]->mesh->name == "Tile Wall")
				modelStack.Rotate(90, 1, 0, 0); 

			modelStack.Scale(object[i]->size);

			if (object[i]->mesh->name == "Light")
				RenderMesh(object[i]->mesh, false);
			else
				RenderMesh(object[i]->mesh, true);
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

	RenderFishtank();

	if(Application::IsKeyPressed('Q'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

		for (unsigned int i = 0; i < object.size(); i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(object[i]->position);
			modelStack.Translate(object[i]->collision.centre);
			RenderMesh(object[i]->collision.boundingBox, false);
			modelStack.PopMatrix();
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

	for (unsigned int i = 0; i < player.loot.size(); i++)
	{
		std::string text = "+1 "; text += player.loot[i]->name;

		modelStack.PushMatrix();
		modelStack.Translate(player.loot[i]->textPos);
		modelStack.Rotate(camera.orientation, 0,1,0);
		modelStack.Rotate(-camera.look,1,0,0);
		modelStack.Rotate(180, 0,1,0);
		float textLength = float(text.size());
		modelStack.Translate(-textLength/2, 0, 0);
		RenderText(meshList[GEO_TEXT], text, Color(1, 1, 1));
		modelStack.PopMatrix();
	}
	//2D

	glDisable(GL_DEPTH_TEST);

	modelStack.PushMatrix();
	viewStack.PushMatrix();
	projectionStack.PushMatrix();

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	projectionStack.Ortho(-10, 10, -10, 10, 0, 10);
	
	modelStack.PushMatrix();
	modelStack.Translate(1 + player.value[player.bobbing],-1,-10);
	modelStack.Scale(1,1,1);
	RenderMesh(meshList[GEO_KNIFE], true);
	modelStack.PopMatrix();

	RenderCrosshair();

	projection.SetToOrtho(-16, 16, -12, 12, -10, 10);
	projectionStack.LoadMatrix(projection);

	for (int i = 0; i < 9; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(player.inventory.slots[i].position);
		RenderMesh(player.inventory.slots[i].mesh, false);
		modelStack.PopMatrix();
		if (!player.inventory.slots[i].item.empty())
		{
			modelStack.PushMatrix();
			modelStack.Translate(player.inventory.slots[i].position - Vector3(0,0.5f,0));
			modelStack.Rotate(0, 0,1,0);
			modelStack.Rotate(-5, 1,0,0);
			//modelStack.Scale(0.5f,0.5f,0.5f);
			RenderMesh(player.inventory.slots[i].item[0]->mesh, false);
			modelStack.PopMatrix();
		}

		
	}

	modelStack.PushMatrix();
	modelStack.Translate(player.inventory.selector->position);
	RenderMesh(player.inventory.selector->mesh, false);
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
	
	if (object[camera.lookAt]->ignoreCollision)
	{
		string tooltip;
		if (object[camera.lookAt]->mesh->name == "Button")
			tooltip += "'E' to push ";
		else
			tooltip += "'E' to loot ";
		tooltip += object[camera.lookAt]->mesh->name;

		modelStack.PushMatrix();
		modelStack.Translate(-7,-5,0);
		modelStack.Scale(0.5f,0.5f,0.5f);
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
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void PROJECTScene::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
void PROJECTScene::RenderFishtank()
{
	for (int i = 0; i < 3; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fishtank.fish[i].position);
		modelStack.Translate(fishtank.fish[i].collision.centre);
		modelStack.Rotate(fishtank.fish[i].swim,0,1,0);
		modelStack.Rotate(fishtank.fish[i].orientation,0,1,0);
		modelStack.Scale(0.005f,0.005f,0.005f);
		RenderMesh(fishtank.fish[i].mesh, false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(fishtank.position);
		modelStack.PushMatrix();
		modelStack.Translate(0,0,fishtank.Front.location);
		modelStack.Translate(0,fishtank.collision.centre.y + fishtank.collision.hitbox.y/4,0);
		modelStack.Rotate(-90,1,0,0);
		RenderMesh(fishtank.Front.boundingBox, false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0,0,fishtank.Back.location);
		modelStack.Translate(0,fishtank.collision.centre.y + fishtank.collision.hitbox.y/4,0);
		modelStack.Rotate(90,1,0,0);
		RenderMesh(fishtank.Back.boundingBox, false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(fishtank.Left.location,0,0);
		modelStack.Translate(0,fishtank.collision.centre.y + fishtank.collision.hitbox.y/4,0);
		modelStack.Rotate(90,0,0,1);
		RenderMesh(fishtank.Left.boundingBox, false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(fishtank.Right.location,0,0);
		modelStack.Translate(0,fishtank.collision.centre.y + fishtank.collision.hitbox.y/4,0);
		modelStack.Rotate(90,0,0,1);
		RenderMesh(fishtank.Right.boundingBox, false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}
void PROJECTScene::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0,0,-0.499f);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0, 0.499f);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.499f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.499f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(-90,0,0,1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0.499f,0);
	modelStack.Rotate(180,1,0,0);
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,-0.499f,0);
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
void PROJECTScene::RenderGarfield(Garfield garfield, float legR, float legT)
{
	modelStack.Scale(0.55f,0.55f,0.45f);
	RenderMesh(garfield.meshList[garfield.GEO_SPHERE],true);
		modelStack.PushMatrix();
		modelStack.Translate(-8,5.3f,0);
		modelStack.Rotate(10,1,0,0);
		modelStack.Rotate(-25,0,0,1);
		RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-5,0);
			RenderMesh(garfield.meshList[garfield.GEO_ARM], true);
				modelStack.PushMatrix();
				modelStack.Translate(0,-4.9f,0);
				modelStack.Rotate(6,0,0,1);
				modelStack.Scale(1,1.65f,2);
				RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
				modelStack.Scale(1,1/1.65f,1/2.0f);
					modelStack.PushMatrix();
					modelStack.Rotate(15,0,0,1);
					modelStack.Translate(0.1f,-2.6f,1.75);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(15,0,0,1);
					modelStack.Translate(0,-2.8f,0);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(25,0,1,0);
					modelStack.Rotate(-75,1,0,0);
					modelStack.Translate(0,-3,0);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(15,0,0,1);
					modelStack.Translate(-0.1f,-2.3f,-1.75f);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(8,5.3f,0);
		modelStack.Rotate(10,1,0,0);
		modelStack.Rotate(25,0,0,1);
		RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-5,0);
			RenderMesh(garfield.meshList[garfield.GEO_ARM], true);
				modelStack.PushMatrix();
				modelStack.Translate(0,-4.9f,0);
				modelStack.Rotate(-6,0,0,1);
				modelStack.Scale(1,1.65f,2);
				RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
				modelStack.Scale(1,1/1.65f,1/2.0f);

					modelStack.PushMatrix();
					modelStack.Rotate(-15,0,0,1);
					modelStack.Translate(-0.1f,-2.6f,1.75);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(-15,0,0,1);
					modelStack.Translate(0,-2.8f,0);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(-25,0,1,0);
					modelStack.Rotate(-75,1,0,0);
					modelStack.Translate(0,-3,0);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Rotate(-15,0,0,1);
					modelStack.Translate(0.1f,-2.3f,-1.75f);
					RenderMesh(garfield.meshList[garfield.GEO_FINGER], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-4.5f,-6.8f,legT);
		modelStack.Rotate(legR,1,0,0);
		RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-7,0);
			modelStack.Scale(1,1.4f,1);
			RenderMesh(garfield.meshList[garfield.GEO_ARM], true);
			modelStack.Scale(1,1/1.4f,1);
				modelStack.PushMatrix();
				modelStack.Translate(-0.25f,-7.2f,3);
				modelStack.Scale(0.4f,0.35f,0.65f);
				RenderMesh(garfield.meshList[garfield.GEO_HEMI], true);
					modelStack.PushMatrix();
					modelStack.Translate(-2.5f,0.05f,0.8f);
					modelStack.Scale(0.25f,0.91f,0.9f);
					RenderMesh(garfield.meshList[garfield.GEO_FOOT], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(2.5f,0.05f,0.8f);
					modelStack.Scale(0.25f,0.91f,0.9f);
					RenderMesh(garfield.meshList[garfield.GEO_FOOT], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4.5f,-6.8f,legT);
		modelStack.Rotate(legR,1,0,0);
		RenderMesh(garfield.meshList[garfield.GEO_JOINT], true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-7,0);
			modelStack.Scale(1,1.4f,1);
			RenderMesh(garfield.meshList[garfield.GEO_ARM], true);
			modelStack.Scale(1,1/1.4f,1);
				modelStack.PushMatrix();
				modelStack.Translate(0.25f,-7.2f,3);
				modelStack.Scale(0.4f,0.35f,0.65f);
				RenderMesh(garfield.meshList[garfield.GEO_HEMI], true);
					modelStack.PushMatrix();
					modelStack.Translate(-2.5f,0.05f,0.8f);
					modelStack.Scale(0.25f,0.91f,0.9f);
					RenderMesh(garfield.meshList[garfield.GEO_FOOT], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(2.5f,0.05f,0.8f);
					modelStack.Scale(0.25f,0.91f,0.9f);
					RenderMesh(garfield.meshList[garfield.GEO_FOOT], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(1/0.55f, 1/0.55f, 1/0.45f);
	modelStack.Translate(0,10,0);
	modelStack.Scale(0.75f,0.75f,0.5f);
	RenderMesh(garfield.meshList[garfield.GEO_SPHERE],true);
		modelStack.PushMatrix();
		modelStack.Translate(-1.25f,0.7f,3);
		modelStack.Scale(0.6f,0.8f,0.75f);
		RenderMesh(garfield.meshList[garfield.GEO_SPHERE],true);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(1.25f,0.7f,3);
		modelStack.Scale(0.6f,0.8f,0.75f);
		RenderMesh(garfield.meshList[garfield.GEO_SPHERE],true);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(0,-3.2f,9);
		modelStack.Scale(0.15f,0.11f,0.15f);
		RenderMesh(garfield.meshList[garfield.GEO_NOSE],true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-3,-3,6.5f);
		modelStack.Rotate(-15,0,0,1);
		modelStack.Rotate(-25,0,1,0);
		modelStack.Rotate(180,1,0,0);
		modelStack.Scale(0.45f,0.25f,0.25f);
		RenderMesh(garfield.meshList[garfield.GEO_MOUTH],true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(3,-3,6.5);
		modelStack.Rotate(15,0,0,1);
		modelStack.Rotate(25,0,1,0);
		modelStack.Rotate(180,1,0,0);
		modelStack.Scale(0.45f,0.25f,0.25f);
		RenderMesh(garfield.meshList[garfield.GEO_MOUTH],true);
		modelStack.PopMatrix();
	
		modelStack.Scale(1/0.75f, 1/0.75f, 1/0.5f);
		
		modelStack.PushMatrix();
		modelStack.Translate(3.3f,-1.6f,2.4f);
		modelStack.Scale(0.2f,0.2f,0.2f);
		RenderMesh(garfield.meshList[garfield.GEO_CHEEK],true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-3.3f,-1.6f,2.4f);
		modelStack.Scale(0.2f,0.2f,0.2f);
		RenderMesh(garfield.meshList[garfield.GEO_CHEEK],true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-2,5,2);
		modelStack.Rotate(-90, 1,0,0);
		modelStack.Scale(0.25f,0.25f,0.4f);
		RenderMesh(garfield.meshList[garfield.GEO_HEMI],true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-0.1f,0.5f);
			modelStack.Scale(0.75f,0.75f,0.75f);
			RenderMesh(garfield.meshList[garfield.GEO_EAR],true);
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(2,5,2);
		modelStack.Rotate(-90,1,0,0);
		modelStack.Scale(0.25f,0.25f,0.4f);
		RenderMesh(garfield.meshList[garfield.GEO_HEMI],true);
			modelStack.PushMatrix();
			modelStack.Translate(0,-0.1f,0.5f);
			modelStack.Scale(0.75f,0.75f,0.75f);
			RenderMesh(garfield.meshList[garfield.GEO_EAR],true);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
}

void Object::Update(double dt)
{
}

void FishTank::Update(double dt)
{
	for (int i = 0; i < 3; i++)
	{
		if (fish[i].position.y + fish[i].collision.hitbox.y > Top.location)
		{
			fish[i].position.y = Top.location - fish[i].collision.hitbox.y;
			fish[i].direction.y = -fish[i].direction.y;
		}
		if (fish[i].position.y < Bottom.location)
		{
			fish[i].position.y = Bottom.location;
			fish[i].direction.y = -fish[i].direction.y;
		}

		if (fish[i].position.z + fish[i].collision.hitbox.z/2 > Front.location + position.z)
		{
			fish[i].position.z = Front.location + position.z - fish[i].collision.hitbox.z/2;
			fish[i].direction.z = -fish[i].direction.z;
		}
		if (fish[i].position.z - fish[i].collision.hitbox.z/2 < Back.location + position.z)
		{
			fish[i].position.z = Back.location + position.z + fish[i].collision.hitbox.z/2;
			fish[i].direction.z = -fish[i].direction.z;
		}

		if (fish[i].position.x + fish[i].collision.hitbox.x/2 > Right.location + position.x)
		{
			fish[i].position.x = Right.location + position.x - fish[i].collision.hitbox.z/2;
			fish[i].direction.x = -fish[i].direction.x;
		}
		if (fish[i].position.x - fish[i].collision.hitbox.z/2 < Left.location + position.x)
		{
			fish[i].position.x = Left.location + position.x + fish[i].collision.hitbox.z/2;
			fish[i].direction.x = -fish[i].direction.x;
		}
		fish[i].Update(dt);
	}
}
static int swimDir = 1;
void Fish::Update(double dt)
{
	position += direction/100;

	if(swim * swimDir > 15.f)
			swimDir = -swimDir;
		swim += (float)(swimDir * 25.f * dt);

	if(direction.x > 0) //+
	{
		orientation+=2;

		if (orientation >= 90)
			orientation = 90;
	}
	else if(direction.x < 0) //-
	{
		orientation-=2;

		if (orientation <= -90)
			orientation = -90;
	}
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

bool PROJECTScene::checkCollision(Object* a, Object* b)
{
	Vector3 CubeA = a->collision.hitbox/2; CubeA += a->collision.centre; 
	Vector3 CubeB = b->collision.hitbox/2; CubeB += b->collision.centre;

	Vector3 maxCubeA = CubeA ; maxCubeA += a->position;
	Vector3 minCubeA = CubeA - a->collision.hitbox; minCubeA += a->position;

	Vector3 maxCubeB = CubeB; maxCubeB += b->position;
	Vector3 minCubeB = CubeB - b->collision.hitbox; minCubeB += b->position;

	return(maxCubeA.x > minCubeB.x && 
    minCubeA.x < maxCubeB.x &&
    maxCubeA.y > minCubeB.y &&
    minCubeA.y < maxCubeB.y &&
    maxCubeA.z > minCubeB.z &&
    minCubeA.z < maxCubeB.z);
} 