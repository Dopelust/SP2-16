#include "collision.h"

Object::Object(Vector3 p, Vector3 c, Vector3 h)
{
	position = p;
	collision.hitbox = h;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = true; 
	mesh = NULL;
}

Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* b)
{
	position = p;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = false;
	mesh = NULL;
}
Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = 0; size = 1;
	ignoreCollision = false;
}

Object::Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i)
{
	position = p;
	mesh = m;
	collision.hitbox = h;
	collision.boundingBox = b;
	collision.centre = c;
	orientation = o; size = s;
	ignoreCollision = i;
}

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