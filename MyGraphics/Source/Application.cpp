
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "PROJECTScene.h"
#include "MENUScene.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

int Application::mouseScroll = 0;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}
void scroll_callback(GLFWwindow* window, double x, double y)
{
	Application::mouseScroll = int(y);
}

int width;
int height;

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glfwGetWindowSize(m_window, &width, &height);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::mouseButton(unsigned short button)
{
    return glfwGetMouseButton(m_window, button);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);
	
	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(1184, 666, "Computer Graphics", NULL, NULL);
	glfwSetWindowPos(m_window, 50, 50);
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	glfwSetWindowSizeCallback(m_window, resize_callback);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

Position Application::getMousePos()
{
	double xpos = 0; double ypos = 0;
	glfwGetCursorPos(m_window, &xpos, &ypos);

	return Position((float)xpos, (float)ypos, 0);
}

extern bool menu;
extern bool main;
extern bool play;
extern bool quit;

void Application::Run()
{
	glfwGetWindowSize(m_window, &width, &height);

	Scene * menuScene = new MENUScene(); menuScene->Init();
	Scene * playScene = new PROJECTScene(); playScene->Init();
	Scene *scene = menuScene;

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (true)
	{
		glfwSetScrollCallback(m_window,scroll_callback);

		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		
		glfwSetCursorPos(m_window, double(width)/2, double(height)/2);

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  

		if (scene->type == "MENUScene")
		{
			if (play)
			{
				menu = false;

				scene = playScene;
				scene->Reset();
			}
			if (quit)
				break;
		}
		else if (scene->type == "PROJECTScene")
		{
			if (menu)
			{
				play = false;

				scene = menuScene;
				scene->Reset();
			}
		}
	}

	menuScene->Exit();
	delete menuScene;

	playScene->Exit();
	delete playScene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
