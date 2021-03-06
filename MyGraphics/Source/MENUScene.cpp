/******************************************************************************/
/*!
\file	MEUScene.cpp
\author Ricsson
\par	
\brief
This is the MENUScene cpp
*/
/******************************************************************************/
#include "MENUScene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "GLFW\glfw3.h"

using namespace::std;

float MENUScene::inputDelay = 0.f;

/******************************************************************************/
/*!
		MENUScene:
\brief	This is the constructor
*/
/******************************************************************************/
MENUScene::MENUScene()
{
	type = "MENUScene";
}

MENUScene::~MENUScene()
{
}
/******************************************************************************/
/*!
		MENUScene:
\brief	Initializers
*/
/******************************************************************************/
void MENUScene::InitJunk()
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

bool main = true;
float startupDelay = 0.f;

void MENUScene::Init()
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

	meshList[GEO_HOLD] = MeshBuilder::GenerateCubeOnPlane("Cube", Color(1,1,1), 1.f, 1.f, 1.f, 1);
	meshList[GEO_HOLD]->textureID = LoadTGA("Image//hold.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 0.1f, 0.1f, 0.1f, 1);
	meshList[GEO_BIGCUBE] = MeshBuilder::GenerateCube("Cube", Color(1,1,1), 12.f, 12.f, 12.f, 1);

	meshList[GEO_KNIFE] = MeshBuilder::GenerateOBJ("Knife","OBJ//knife.obj");
	//meshList[GEO_KNIFE]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateXYQuad("Text Box", Color(0,0,0), 8, 1, 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateXYQuad("Controls", Color(1,1,1), 16.5f, 16.5f, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//UI//controls.tga");

	meshList[GEO_HEART] = MeshBuilder::GenerateXYQuad("Heart", Color(1,1,1), 1, 1, 1);
	meshList[GEO_HEART]->textureID = LoadTGA("Image//UI//heart.tga");

	meshList[GEO_SPRINT] = MeshBuilder::GenerateXYQuad("Sprint", Color(1,1,1), 1, 1, 1);
	meshList[GEO_SPRINT]->textureID = LoadTGA("Image//UI//sprint.tga");

	meshList[GEO_JUMP] = MeshBuilder::GenerateXYQuad("Jump", Color(1,1,1), 1, 1, 1);
	meshList[GEO_JUMP]->textureID = LoadTGA("Image//UI//jump.tga");

	meshList[GEO_OVERLAY] = MeshBuilder::GenerateXYQuad("Overlay", Color(1,1,1), 32, 18, 1);
	meshList[GEO_OVERLAY]->textureID = LoadTGA("Image//UI//overlay.tga");

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

	soundInit();

	buttons[0].position = Vector3(0,2.5f,0);
	buttons[0].name = "PLAY";

	buttons[1].position = Vector3(0,0,0);
	buttons[1].name = "CHEATS";

	buttons[2].position = Vector3(0,-2.5f,0);
	buttons[2].name = "CONTROLS";

	buttons[3].position = Vector3(0,-5.f,0);
	buttons[3].name = "QUIT";

	Reset();
}

void MENUScene::Reset()
{
	select = 0;
	main = true;
	startupDelay = 0.4f;
}

extern float selectDelay;
extern float pauseDelay;
extern ISoundEngine * engine;

bool menu = true;
bool play = false;
bool quit = false;

bool cheats = false;
string cheat;
bool controls = false;

char lastKey;

bool JEREMIAH = false;
bool JESSICA = false;
bool DARREN = false;
bool RICSSON = false;

/******************************************************************************/
/*!
		MENUScene:
\brief	Update
*/
/******************************************************************************/
void MENUScene::Update(double dt)
{
	if (!engine->isCurrentlyPlaying("Media/bgm.mp3"))
		engine->play2D("Media/bgm.mp3");

	if (main && startupDelay == 0)
	{
		if (selectDelay == 0)
		{
			if (Application::IsKeyPressed(VK_UP))
			{
				if (select > 0)
				{
					selectDelay = 0.14f;
					select--;
					engine->play2D("Media/select.mp3");
				}
			}
			else if (Application::IsKeyPressed(VK_DOWN))
			{
				if (select < 3)
				{
					selectDelay = 0.14f;
					select++;

					engine->play2D("Media/select.mp3");
				}
			}
		}
	
		if (Application::IsKeyPressed(VK_RETURN) || Application::IsKeyPressed(VK_SPACE) || Application::IsKeyPressed('E'))
		{
			main = false;

			if (select == 0)
			{
				play = true;
				engine->stopAllSounds();
			}
			else if (select == 1)
				cheats = true;
			else if (select == 2)
				controls = true;
			else if (select == 3)
				quit = true;
		}

	}

	if (cheats)
	{
		if (cheat.size() < 10)
		{
			for (int i = 65; i < 91; i++)
			{
				if (i == lastKey && inputDelay != 0)
					continue;

				if (Application::IsKeyPressed(i) && inputDelay < 0.06f)
				{
					lastKey = i;
					cheat += i;
					inputDelay = 0.12f;
					break;
				}
			}
		}

		if (Application::IsKeyPressed(VK_BACK) && inputDelay == 0)
		{
			if (cheat.size() > 0)
			{
				cheat.pop_back();
				inputDelay = 0.12;
			}
			else
			{
				cheats = false;
				cheat = "";
				main = true;
			}
		}

		if (Application::IsKeyPressed(VK_RETURN))
		{
			if (cheat == "RICSSON")
			{
				RICSSON = true;
				cheat = "";
			}
			else if(cheat == "JEREMIAH")
			{
				JEREMIAH = true;
				cheat = "";
			}
			else if(cheat == "JESSICA")
			{
				JESSICA = true;
				cheat = "";
			}
			else if(cheat == "DARREN")
			{
				DARREN = true;
				cheat = "";
			}
		}
	}

	if (controls)
	{
		if (Application::IsKeyPressed(VK_BACK))
		{
			controls = false;
			main = true;
		}
	}

	camera->orientation -=float( 5 * dt);
	camera->Update(dt);

	if (selectDelay > 0)
		selectDelay -= float(dt);
	else
		selectDelay = 0;

	if (inputDelay > 0)
		inputDelay -= float(dt);
	else
		inputDelay = 0;

	if (startupDelay > 0)
		startupDelay -= float(dt);
	else
		startupDelay = 0;

	for (int i = 65; i < 91; i++)
	{
		Application::IsKeyPressed(i);
	}
	Application::IsKeyPressed(VK_ESCAPE);
	Application::IsKeyPressed(VK_UP);
	Application::IsKeyPressed(VK_DOWN);
	Application::IsKeyPressed(VK_SPACE);
	Application::IsKeyPressed(VK_BACK);
	Application::IsKeyPressed('1');
	Application::IsKeyPressed('2');
	Application::IsKeyPressed('3');
	Application::IsKeyPressed('4');
	Application::mouseScroll = 0;
}

/******************************************************************************/
/*!
		MENUScene:
\brief	Render
*/
/******************************************************************************/
void MENUScene::RenderScene()
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

	modelStack.PushMatrix();
	modelStack.Translate(camera->position);
	modelStack.Scale(500);
	RenderSkybox();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
/******************************************************************************/
/*!
		MENUScene:
\brief	Render
*/
/******************************************************************************/
void MENUScene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Mtx44 projection;

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	viewStack.LookAt(camera->position.x, camera->position.y,
		camera->position.z, camera->target.x, camera->target.y,
		camera->target.z, camera->up.x, camera->up.y, camera->up.z);
	projection.SetToPerspective(camera->fov, 16.f / 9.f, 0.1f, 100000.f);
	projectionStack.LoadMatrix(projection);

	RenderScene();

	glDisable(GL_DEPTH_TEST);

	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	projection.SetToOrtho(-16, 16, -9, 9, -20, 20);
	projectionStack.LoadMatrix(projection);

	if (main)
	{
		for (int i = 0; i < 4; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(buttons[i].position);
			float textLength = getTextWidth(buttons[i].name);
			if (select == i)
			{
				if (i == 1 || i == 2 || i == 3)
					modelStack.Translate(-textLength * 1.8f, 0, 0);
				else
					modelStack.Translate(-textLength * 1.65f, 0, 0);
				modelStack.Scale(4.f);
				RenderText(meshList[GEO_TEXT], buttons[i].name , Color(1, 1, 1));
			}
			else
			{
				modelStack.Translate(-textLength, 0, 0);
				modelStack.Scale(2.f);
				RenderText(meshList[GEO_TEXT], buttons[i].name , Color(0.35f, 0.35f, 0.35f));
			}
			modelStack.PopMatrix();
		}
	}
	else if (cheats)
	{
		RenderMesh(meshList[GEO_TEXTBOX], false);

		modelStack.PushMatrix();
		modelStack.Translate(0.25f, 1.5f, 0);
		float textLength = getTextWidth("Enter cheat:");
		modelStack.Translate(-textLength / 2, 0, 0);
		RenderText(meshList[GEO_TEXT], "Enter cheat:" , Color(1, 1, 1));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.25f, 0, 0);
		textLength = getTextWidth(cheat);
		modelStack.Translate(-textLength / 2, 0, 0);
		RenderText(meshList[GEO_TEXT], cheat , Color(1, 1, 1));
		modelStack.PopMatrix();

		if (cheat.empty())
		{
			modelStack.PushMatrix();
			modelStack.Translate(0.25f, -2.f, 0);
			float textLength = getTextWidth("Backspace to return");
			modelStack.Scale(0.5f);
			modelStack.Translate(-textLength / 2, 0, 0);
			RenderText(meshList[GEO_TEXT], "Backspace to return" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(0.25f, -2.f, 0);
			float textLength = getTextWidth("Backspace to clear");
			modelStack.Scale(0.5f);
			modelStack.Translate(-textLength / 2, 0, 0);
			RenderText(meshList[GEO_TEXT], "Backspace to clear" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}

		if (RICSSON)
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.f, 3.f, 0);
			RenderText(meshList[GEO_TEXT], "+ 16 STATBOOST DRINKS" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}

		if (JEREMIAH)
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.f, 1.f, 0);
			RenderText(meshList[GEO_TEXT], "+ HIGHJUMP LEVEL 10" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}

		if (JESSICA)
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.f, -1.f, 0);
			RenderText(meshList[GEO_TEXT], "+ SPRINT LEVEL 10" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}

		if (DARREN)
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.f, -3.f, 0);
			RenderText(meshList[GEO_TEXT], "+ GODMODE" , Color(1, 1, 1));
			modelStack.PopMatrix();
		}
	}
	else if (controls)
	{
		modelStack.PushMatrix();
		RenderMesh(meshList[GEO_OVERLAY], false);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Scale(2.f);
		modelStack.Translate(0.25f, 0, 0);
		float textLength = getTextWidth("LOADING...");
		modelStack.Translate(-textLength / 2, 0, 0);
		RenderText(meshList[GEO_TEXT], "LOADING..." , Color(1, 1, 1));
		modelStack.PopMatrix();
	}
	glEnable(GL_DEPTH_TEST);
	//2D
}

/******************************************************************************/
/*!
		MENUScene:
\brief	Render Mesh
*/
/******************************************************************************/
void MENUScene::RenderMesh(Mesh *mesh, bool enableLight)
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

/******************************************************************************/
/*!
		MENUScene:
\brief	RenderText 
*/
/******************************************************************************/
void MENUScene::RenderText(Mesh* mesh, std::string text, Color color)
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
		else if (text[i] == 't' || text[i] == 'I')
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
			if (text[i] == 'I')
			{
				textWidth -= 0.4f;
				break;
			}
			else if (text[i] == j)
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

/******************************************************************************/
/*!
		MENUScene:
\brief	Get text width
*/
/******************************************************************************/
float MENUScene::getTextWidth(string text)
{
	float textWidth = 0.f;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		if (text[i] == '.' || text[i] == '\'' )
			textWidth -= 0.1f;
		else if (text[i] == 'l' || text[i] == 'i')
			textWidth -= 0.05f;
		else if (text[i] == 't' || text[i] == 'I')
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
			if (text[i] == 'I')
			{
				textWidth -= 0.4f;
				break;
			}
			else if (text[i] == j)
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

/******************************************************************************/
/*!
		MENUScene:
\brief	Exit
*/
/******************************************************************************/
void MENUScene::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	engine->stopAllSounds();
}

/******************************************************************************/
/*!
		MENUScene:
\brief	Rendering the skybox
*/
/******************************************************************************/
void MENUScene::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0,0,-0.4985f);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0, 0.4985f);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.4985f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.4985f,0,0);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(-90,0,0,1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0.4985f,0);
	modelStack.Rotate(180,1,0,0);
	modelStack.Rotate(180,0,1,0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,-0.4985f,0);
	modelStack.Rotate(90,0,1,0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

/******************************************************************************/
/*!
		MENUScene:
\brief	Render Crosshair
*/
/******************************************************************************/
void MENUScene::RenderCrosshair()
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

