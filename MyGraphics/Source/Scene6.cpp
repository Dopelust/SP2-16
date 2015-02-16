#include "Scene6.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

Scene6::Scene6()
{
}

Scene6::~Scene6()
{
}

void Scene6::Init()
{
	// Init VBO here
	
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	
	//variable to rotate geometry
	rotateAngle = 0;

	//Initialize camera settingss
	camera.Init(Vector3(0, 250, 800), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0,0.5,0), 1, 1);
	meshList[GEO_CUBE1] = MeshBuilder::GenerateCube("cube1", Color(1,0,0), 30,25,10);
	meshList[GEO_CUBE2] = MeshBuilder::GenerateCube("cube2", Color(0,0,1), 10,20,10);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1,1,1), Color(1,1,1), 36,36, 10);
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

//static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene6::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	static int rotateDir = 1;
	if(rotateAngle * rotateDir > 30)
	rotateDir = -rotateDir;
	rotateAngle += (float)(rotateDir * 100 * dt);
	camera.Update(dt);
}

void Scene6::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
	camera.position.z, camera.target.x, camera.target.y,
	camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_AXES] -> Render();

	modelStack.PushMatrix();
	modelStack.Translate(0,-95,0);
	modelStack.Scale(1000,1000,1000);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_QUAD] -> Render();
	modelStack.PopMatrix();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(125*i, 0, 125*j);
			RenderRobot();
			modelStack.PopMatrix();
		}
	}
}

void Scene6::RenderRobot()
{
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	modelStack.PushMatrix();
	RenderBody();

	modelStack.PushMatrix();
	modelStack.Translate(0, 42.5, 0);
	modelStack.Scale(2,2,2);
	RenderHead();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Right Arm
	modelStack.Translate(40, 0, 0);
	modelStack.Rotate(rotateAngle, 2, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Left Arm
	modelStack.Translate(-40, 0, 0);
	modelStack.Rotate(rotateAngle, -2, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Right Leg
	modelStack.Translate(20, -47.5, 0);
	modelStack.Rotate(rotateAngle, -1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(rotateAngle, -1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Left Leg
	modelStack.Translate(-20, -47.5, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Scene6::RenderBody()
{
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	modelStack.PushMatrix();
	modelStack.Translate(0,-7.5,0);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_CUBE1] -> Render();
	modelStack.PopMatrix();
}

void Scene6::RenderLimb()
{
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	modelStack.PushMatrix();
	modelStack.Translate(0,-5,0);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_CUBE2] -> Render();
	modelStack.PopMatrix();
}

void Scene6::RenderHead()
{
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	modelStack.PushMatrix();
	modelStack.Translate(0,-2.5,0);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_SPHERE] -> Render();
	modelStack.PopMatrix();
}

void Scene6::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
