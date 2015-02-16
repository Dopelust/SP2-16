#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
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
	camera.Init(Vector3(120, 100, 120), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("sun", Color(1,0,0), Color(1,0.15,0), 36,36, 10);
	meshList[GEO_PLANET01] = MeshBuilder::GenerateSphere("planet01", Color(1,1,0), Color(1,1,0), 36,36, 10);
	meshList[GEO_PLANET01_RING] = MeshBuilder::GenerateCircle("planet01_ring", Color(1,0.5,0),Color(1,0,0), 1);
	meshList[GEO_PLANET03] = MeshBuilder::GenerateSphere("planet03", Color(0.5,0.9,1), Color(0.5,0.9,1), 36,36, 10);
	meshList[GEO_PLANET03_MOON] = MeshBuilder::GenerateSphere("planet03_moon", Color(0.3,0.3,0.3), Color(0.3,0.3,0.3), 36,36, 10);
	meshList[GEO_PLANET04] = MeshBuilder::GenerateSphere("planet04", Color(0,0.5,0), Color(0,0,0), 36,36, 10);
	meshList[GEO_PLANET05] = MeshBuilder::GenerateSphere("planet05", Color(0.5,1,0), Color(0.5,1,0), 36,36, 10);
	meshList[GEO_PLANET06] = MeshBuilder::GenerateSphere("planet06", Color(0.75,0.75,0.75),  Color(0.75,0.75,0.75), 36,36, 10);
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

//static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene4::Update(double dt)
{
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	rotateAngle += (float)(10 * dt);

	camera.Update(dt);
}

void Scene4::Render()
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
	modelStack.Rotate(rotateAngle,0,1,0);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_SUN] -> Render();
	modelStack.PopMatrix();


	modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle,0,1,0);
		modelStack.Translate(-55,0,0);
		modelStack.Rotate(rotateAngle,0,1,0);
		modelStack.Scale(0.5f,0.5f,0.5f);
	mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
	meshList[GEO_PLANET01] -> Render();

		modelStack.PushMatrix();
			modelStack.Scale(16.f,16.f,16.f);
		mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
		meshList[GEO_PLANET01_RING] -> Render();
		
		modelStack.PopMatrix();

	modelStack.PopMatrix();

	for (int i = GEO_PLANET03; i < NUM_GEOMETRY; i++)
	{
		if ( i != GEO_PLANET03_MOON) {
			modelStack.PushMatrix();

			if ( i == GEO_PLANET03 ) {
				modelStack.Rotate(rotateAngle/2,0,1,0.2);
				modelStack.Translate(90,0,0);
				modelStack.Rotate(rotateAngle/2,0,1,0);
				modelStack.Scale(0.6f,0.6f,0.6f);
			}
			if ( i == GEO_PLANET04 ) {
				modelStack.Rotate(rotateAngle/5,0,1,0);
				modelStack.Translate(-120,0,0);
				modelStack.Rotate(rotateAngle/5,0,1,0);
			}
			if ( i == GEO_PLANET05 ) {
				modelStack.Rotate(rotateAngle/4,0,1,0.3);
				modelStack.Translate(25,0,0);
				modelStack.Rotate(rotateAngle,0,1,0);
				modelStack.Scale(0.3f,0.3f,0.3f);
			}
			if ( i == GEO_PLANET06 ) {
				modelStack.Rotate(rotateAngle/6,0,1,0);
				modelStack.Translate(110,0,0);
				modelStack.Rotate(rotateAngle,0,1,0);
				modelStack.Scale(0.2f,0.2f,0.2f);
			}
			mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
			meshList[i] -> Render();
			if ( i == GEO_PLANET03 )  {
				modelStack.PushMatrix();
				modelStack.Rotate(rotateAngle/2,0,1,0);
				modelStack.Translate(20,0,0);
				modelStack.Rotate(rotateAngle,0,1,0);
				modelStack.Scale(0.5f,0.5f,0.5f);
				mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &mvp.a[0]);
				meshList[GEO_PLANET03_MOON] -> Render();
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}
	}
}

void Scene4::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
