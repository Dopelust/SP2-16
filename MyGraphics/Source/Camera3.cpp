/******************************************************************************/
/*!
\file	Camera3.cpp
\author Ricsson
\par	
\brief
This is the camera3 cpp
*/
/******************************************************************************/
#include "Camera3.h"
#include "PROJECTScene.h"
#include "Application.h"
#include "Mtx44.h"

/******************************************************************************/
/*!
		Camera3:
\brief	This is the constructor
*/
/******************************************************************************/
Camera3::Camera3()
{
	orientation = 0.f;
	look = 0.f;

	fov = 70.f;
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the decstructor
*/
/******************************************************************************/
Camera3::~Camera3()
{
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the initializer
*/
/******************************************************************************/
void Camera3::Init(const Vector3& pos, const float& hO, const float& vO)
{
	orientation = defaultOrientation = hO;
	look = defaultLook = vO;

	this->position = defaultPosition = pos;
	Vector3 view; view.SphericalToCartesian(orientation, look);
	Vector3 right; right.SphericalToCartesian(orientation - 90.f, 0.0f);
	Vector3 direction; direction.SphericalToCartesian(orientation, 0.f);
	this->target = defaultTarget = target = position + direction;
	this->up = defaultUp = right.Cross(view);
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the zoom function when sprinting and CCTV camera zoom
*/
/******************************************************************************/
void Camera3::Zoom(double dt)
{
	fov -=float( dt * 250 * Application::mouseScroll );

	if (fov < 10.f)
		fov = 10.f;

	if (fov > 70.f)
		fov = 70.f;
}

extern int width;
extern int height;

/******************************************************************************/
/*!
		Camera3: Move
\brief	This is for the moving of the camera.
*/
/******************************************************************************/
void Camera3::Move(double dt)
{
	float yaw = (float)(10.f * dt * (float)(double(width)/2 - Application::getMousePos().x) * (fov/70.f));
	orientation += yaw;

	if (look <= 90 && look >= -90)
	{
		float pitch = (float)(10.f * dt * (float)(double(height)/2 - Application::getMousePos().y) * (fov/70.f));
		look += pitch;
	}


	if (look >= 90.f)
		look = 90.f;
	if (look <= -90.f)
		look = -90.f;
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the update function
*/
/******************************************************************************/
void Camera3::Update(double dt)
{
	Vector3 view; Vector3 right;
	view.SphericalToCartesian(orientation, look);
	right.SphericalToCartesian(orientation - 90.f, 0.0f);
	up = right.Cross(view);

	target = view*7 + position;
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the lookingAt
*/
/******************************************************************************/
int Camera3::lookingAt(vector<Object*>object, int Reach)
{
	Vector3 view = (target - position).Normalized(); view/=10;
	Object r(position + view* ( (float(Reach/2)) - 0.5f ), Vector3(0,0,0), Vector3(float(Reach/10),float(Reach/10),float(Reach/10)));
	Object o(position, Vector3(0,0,0), Vector3(0.1f,0.1f,0.1f));

	vector<Object*> closeObjects;
	vector<int> index;

	for (unsigned int i = 1; i < object.size(); i++)
	{
		if (Object::checkCollision(object[i], &r))
		{
			closeObjects.push_back(object[i]);
			index.push_back(i);
		}
	}

	if (!closeObjects.empty())
	{
		int reach = 0;
		for (Vector3 p = position; reach < Reach; p += view)
		{
			for (unsigned int i = 0; i < closeObjects.size(); i++)
			{
				o.position = p;
				if ( Object::checkCollision(closeObjects[i], &o) )
				{
					return index[i];
				}
			}
			reach++;
		}
	}

	return 0;
}

/******************************************************************************/
/*!
		Camera3:
\brief	This is the reset
*/
/******************************************************************************/
void Camera3::Reset()
{
	orientation = defaultOrientation;
	look = defaultLook;

	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	fov = 70;
}