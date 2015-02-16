#ifndef PLAYER_H
#define PLAYER_H

#include "Application.h"
#include "collision.h"
#include "inventory.h"

#include <vector>
#include <string>

using namespace::std;

class Loot
{
public:
	Loot(std::string n, Vector3 p) 
	{
		name = n;
		textPos = p;
		elapsedTime = 0;
	};
	~Loot() {};

	std::string name;
	Vector3 textPos;
	float elapsedTime;

	void Update(double dt);
};

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
		bobbing,
		knifeRotation,
		knifeStrike,
		knifeCooldown,
		mouseSens,
		groundLevel,
		eyeLevel,
		jumpCooldown,
		TOTAL_VALUES,
	};
	Player() 
	{ 
		for (int i = 0; i < TOTAL_VALUES; i++)
		{
			value[i] = 0.f;
		}

		hOrientation = 0;
		vOrientation = 0;

		velocity = Vector3(50,40,50);
		value[mouseSens] = 10.f;
		value[eyeLevel] = 12.f;

		state[JUMP] = false;
		value[jumpCooldown] = 0.f;

		collision.hitbox = Vector3(3, 14, 3);
		collision.centre = Vector3(0, 7.f, 0);

		inventory.Init(Vector3(0,-10.5,0));
	};
	~Player() {};

	Vector3 position;
	Vector3 velocity;
	Collision collision;
	float hOrientation;
	float vOrientation;
	Inventory inventory;

	bool state[TOTAL_STATES];
	float value[TOTAL_VALUES];
	vector<Loot*> loot;

	bool checkCollision(Object* b);
	void Control(double dt, vector<Object*>object);
	void Update(double dt, vector<Object*>object);
};

#endif