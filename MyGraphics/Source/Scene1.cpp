#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	glEnable(GL_DEPTH_TEST);

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
	"Shader//SimpleVertexShader.vertexshader",
	"Shader//SimpleFragmentShader.fragmentshader" );

	// Use our shader
	glUseProgram(m_programID);

	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	//8 Sided Polygon Code

	static const GLfloat vertex_buffer_data3[] = { 
	-0.5f,-0.25f,0.0f,
	-0.75f,-0.0f, 0.0f, 
	0.0f,-0.25f,0.0f,
	0.25f,0.0f, 0.0f, 
	-0.5f,0.85f,0.0f,
	-0.75f,0.6f, 0.0f,
	0.0f,0.6f,0.0f, 
	0.0f,0.85f, 0.0f,
	0.25f,0.6f,0.0f, 
	0.25f,0.0f,0.0f,
	0.0f,0.0f,0.0f, 
	0.25f,0.6f,0.0f,//1 
	-0.75f,0.0f,0.0f,
	-0.5f,0.6f,0.0f, 
	-0.75f,0.6f,0.0f, //2
	0.0f,0.85f,0.0f,
	0.0f,0.6f,0.0f, 
	-0.5f,0.85f,0.0f, //3
	0.0f,-0.25f,0.0f, 
	-0.5f,0.0f,0.0f,
	-0.5f,-0.25f,0.0f, 
	
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data3),
	vertex_buffer_data3, GL_STATIC_DRAW);
	
	static const GLfloat color_buffer_data3[] = {
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data3),
	color_buffer_data3, GL_STATIC_DRAW);

	static const GLfloat vertex_buffer_data4[] = { 
	0.0f,1.0f,0.1f,
	0.76f,-0.81f,0.1f,
	-0.4f,0.05f,0.1f, 
	0.0f,1.0f,0.1f,
	-0.75f,-0.81f,0.1f,
	0.4f,0.05f,0.1f,
	-1.0f,0.5f,0.1f,
	1.0f,0.5f, 0.1f, 
	0.0f,-0.25f,0.1f, 
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data4),
	vertex_buffer_data4, GL_STATIC_DRAW);
	
	static const GLfloat color_buffer_data4[] = {
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f, 
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f, 
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f, 
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data4),
	color_buffer_data4, GL_STATIC_DRAW);
	
}
void Scene1::Update(double dt)
{
}

void Scene1::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//8 Sided Polygon Code
	glEnableVertexAttribArray(0); // 1st attribute buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 20); // Starting from vertex 0; 3 vertices = 1 triangle
	glDisableVertexAttribArray(1);

	glEnableVertexAttribArray(0); // 1st attribute buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 9); // Starting from vertex 0; 3 vertices = 1 triangle
	glDisableVertexAttribArray(1);
}

void Scene1::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}
