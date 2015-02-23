#ifndef PLAYER_H
#define PLAYER_H

#include "Application.h"
#include "collision.h"
#include "inventory.h"
#include "Camera3.h"

#include <vector>
#include <string>

using namespace::std;

class Player
{
public:
	enum playerStates
	{
		WALK,
		SPRINT,
		JUMP,
		GROUND,
		KNIFE,
		TOTAL_STATES,
	};

	enum playerValues
	{
		bobbingX,
		bobbingY,
		knifeRotation,
		knifeStrike,
		knifeCooldown,
		mouseSens,
		eyeLevel,
		jumpCooldown,
		TOTAL_VALUES,
	};
	Player() 
	{ 
		position = Vector3(0, 2, 10);
		for (int i = 0; i < TOTAL_VALUES; i++)
		{
			value[i] = 0.f;
		}

		value[eyeLevel] = 5.5f;
		collision.hitbox = Vector3(2.f, 6.5f, 2.f);
		collision.centre = Vector3(0, collision.hitbox.y/2, 0);

		hOrientation = 0;
		vOrientation = 0;

		value[mouseSens] = 10.f;
		holding = -1;

		state[JUMP] = false;
		value[jumpCooldown] = 0.f;

		camera.Init(position, hOrientation, vOrientation); 
		inventory.Init(Vector3(0,-10.5,0));
	};
	~Player() {};

	Vector3 position;
	Vector3 velocity;
	Collision collision;
	float hOrientation;
	float vOrientation;
	Inventory inventory;
	Camera3 camera;

	int holding;
	bool state[TOTAL_STATES];
	float value[TOTAL_VALUES];

	bool checkCollision(Object* b);
	virtual void Control(double dt, vector<Object*>object);
	virtual void Update(double dt, vector<Object*>object);
};

class dynamicObject : public Object
{
public:
	dynamicObject()  {type = "Dynamic";};
	dynamicObject(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b) :	Object(p,c,h,m,b) {type = "Dynamic";};
	~dynamicObject() {};

	virtual void Init() {};
	virtual void Control(double dt, vector<Object*>object, Player* player);
	virtual void Update(double dt, vector<Object*>object, Player* player);
	Vector3 velocity;

	void RespondToCollision(Vector3 initialPos, vector<Object*>object, Player* player);
};

class Doorway
{
public:
	Doorway() { open = false; close = true; };
	void Init(Vector3 p, Object D, Object B1, Object B2)
	{
		doorPosition[0] = p; doorPosition[0].x -= D.collision.hitbox.x/2;
		doorPosition[1] = p; doorPosition[1].x += D.collision.hitbox.x/2;
		Door[0] = D; Door[0].position = doorPosition[0];
		Door[1] = D; Door[1].position = doorPosition[1];
		Button[0] = B1; Button[0].position += p;
		Button[1] = B2; Button[1].position += p;
		buttonStatus[0] = B1.mesh;
		buttonStatus[1] = MeshBuilder::GenerateOBJ("Button", "OBJ//button.obj"); 
		buttonStatus[1]->textureID = LoadTGA("Image//buttonOn.tga");
		elapsedTime = 0;
	};
	~Doorway() {};

	Object Door[2]; //Double Doors
	Vector3 doorPosition[2]; //Closed Position
	Object Button[2];
	Mesh* buttonStatus[2];
	bool open; bool close;
	float elapsedTime;

	virtual void Update(double dt, vector<Object*>object, Player* player);
};

#endif