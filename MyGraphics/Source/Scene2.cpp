#include "Scene2.h"
#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	m_programID = LoadShaders(
	"Shader//TransformVertexShader.vertexshader",
	"Shader//SimpleFragmentShader.fragmentshader" );

	m_parameters[U_MVP] = glGetUniformLocation(m_programID,
	"MVP");

	glUseProgram(m_programID);
	glClearColor(0.5f, 0.0f, 0.0f, 0.0f);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	rotateAngle = 90;
	translateX = 0.1;
	scaleAll = 1.5;

	static const GLfloat vertex_buffer_data[] = { 
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	-1.0f,0.0f,0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
	vertex_buffer_data, GL_STATIC_DRAW);

}

bool left, right;

void Scene2::Update(double dt)
{
	if(Application::IsKeyPressed(VK_SPACE))
	{
		if (rotateAngle <= 90)
		{
			right = true;
		}
		else if (rotateAngle >=180)
		{
			left = true;
		}
		if (right == true)
		{
			rotateAngle += (float)(100 * dt);

			if (rotateAngle >=180)
			{
				right = false;
			}
		}
		else if (left == true)
		{
			rotateAngle -= (float)(100 * dt);

			if (rotateAngle <=90)
			{
				left = false;
			}
		}
		if (translateX >= 2)
		{
			translateX = -2;
		}
		else
		{
			translateX += (float)(10 * dt);
		}

		if (scaleAll >= 4)
		{
			scaleAll = 1;
		}
		else
		{
			scaleAll += (float)(2 * dt);
		}
	}
}

void Scene2::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();
	projection.SetToOrtho(-2, +2, -2, +2, -2, +2);

	//Object 1
	scale.SetToScale(0.5, 0.5, 0.5);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);

	//Object 2
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(-90, 0, 0, 1);
	translate.SetToTranslation(translateX, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
		
	//Object 3
	scale.SetToScale(scaleAll,scaleAll,scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-0.5, 0.75, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
