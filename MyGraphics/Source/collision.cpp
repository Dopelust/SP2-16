/******************************************************************************/
/*!
\file	collision.cpp
\author Ricsson
\par	
\brief
This is the collision.cpp that contains all the functions and constructors.
*/
/******************************************************************************/
#include "collision.h"

/******************************************************************************/
/*!
		Object:
\brief	Basic constructor
*/
/******************************************************************************/
Object::Object(Vector3 p, Vector3 c, Vector3 h)
{
	position = p;
	collision.hitbox = h;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = false;
	mesh = NULL;
	type = "";
}

/******************************************************************************/
/*!
		Object:
\brief	Overloaded constructor
*/
/******************************************************************************/
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = false;
	type = "";
}

/******************************************************************************/
/*!
		Object:
\brief	Overloaded constructor
*/
/******************************************************************************/
Object::Object(Vector3 p, Vector3 c, Vector3 h, float s, float o, bool i)
{
	position = p;
	mesh = NULL;
	collision.hitbox = h;
	collision.centre = c;
	orientation = o; size = s;
	ignoreCollision = i;
	type = "";
}

/******************************************************************************/
/*!
		Object:
\brief	Overloaded constructor
*/
/******************************************************************************/
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.centre = c;
	orientation = o; size = s;
	ignoreCollision = i;
	type = "";
}


/******************************************************************************/
/*!
		Class Object:
\brief	Collsion checker function
*/
/******************************************************************************/
bool Object::checkCollision(Object* a, Object*  b)
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

/******************************************************************************/
/*!
		Class Object:
\brief	Partical update function
*/
/******************************************************************************/
void Particles::Update(double dt)
{
	position += direction * float(dt);
	elapsedTime += float(dt);
}