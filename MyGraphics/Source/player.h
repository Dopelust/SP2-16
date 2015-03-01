#ifndef PLAYER_H
#define PLAYER_H

#include "Application.h"
#include "collision.h"
#include "inventory.h"
#include "Camera3.h"

#include <vector>
#include <string>

using namespace::std;

class dynamicObject : public Object
{
public:
	dynamicObject()  {type = "Dynamic";};
	dynamicObject(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b) :	Object(p,c,h,m,b) {type = "Dynamic"; health = 0;};
	dynamicObject(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float o) :	Object(p,c,h,m,b) {orientation = o; type = "Dynamic"; health = 0;};
	~dynamicObject() {};

	virtual void Init() {};
	virtual void Control(double dt, vector<Object*>object, Player* player);
	virtual void Update(double dt, vector<Object*>object, Player* player);
	Vector3 velocity;

	float hitDelay;
	void UpdateVelocity(double dt);
	bool Knockback(Vector3 dir, Vector3 vel);
	void RespondToCollision(Vector3 initialPos, vector<Object*>object);
	void RespondToCollision(Vector3 initialPos, vector<Object*>object, Player* player);

	float health;
	float getHealth() {return health;}
};

class Player : public dynamicObject
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
		eyeLevel,
		jumpCooldown,
		eatElapsed,
		TOTAL_VALUES,
	};
	Player() 
	{
		type = "Player";
		health = 100;
		apparentHealth = 100;
		position = Vector3(0, 2, -100);
		for (int i = 0; i < TOTAL_VALUES; i++)
		{
			value[i] = 0.f;
		}

		value[eyeLevel] = 5.5f;
		collision.hitbox = Vector3(2.f, 6.5f, 2.f);
		collision.centre = Vector3(0, collision.hitbox.y/2, 0);

		hOrientation = 0;
		vOrientation = 0;

		holding = -1;

		state[JUMP] = false;
		value[jumpCooldown] = 0.f;

		camera.Init(position, hOrientation, vOrientation); 
		inventory.Init(Vector3(0,-10.5,0));
		inConversation = false;
		mesh =  NULL;
	};
	~Player() {};

	float hOrientation;
	float vOrientation;
	float apparentHealth;
	Inventory inventory;
	Camera3 camera;

	int holding;
	bool state[TOTAL_STATES];
	bool inConversation;
	float value[TOTAL_VALUES];

	virtual void Control(double dt, vector<Object*>object);
	virtual void Update(double dt, vector<Object*>object);

	float getRenderWallet() {return inventory.wallet.apparentValue;};
	float getWallet() {return inventory.wallet.trueValue;};
	float getHealth() {return apparentHealth;};
};

class Doorway
{
public:
	Doorway() { open = false; close = true; };

	void Init(Vector3 p, Object D, Object B1, Object B2, Object R)
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
		Range = R; Range.position += p; Range.ignoreCollision = true;
		elapsedTime = 0;
	};
	void Init(Vector3 p, Object D, Object R)
	{
		doorPosition[0] = p; doorPosition[0].x -= D.collision.hitbox.x/2;
		doorPosition[1] = p; doorPosition[1].x += D.collision.hitbox.x/2;
		Door[0] = D; Door[0].position = doorPosition[0];
		Door[1] = D; Door[1].position = doorPosition[1];
		Range = R; Range.position += p; Range.ignoreCollision = true;
		elapsedTime = 0;
	};

	~Doorway() {};

	Object Door[2]; //Double Doors
	Vector3 doorPosition[2]; //Closed Position
	Object Button[2];
	Object Range;
	Mesh* buttonStatus[2];
	bool open; bool close;
	float elapsedTime;

	void ButtonUpdate(double dt, vector<Object*>object, Player* players);
	void RangeUpdate(double dt, vector<Object*>object, Player* player);
	void Update(double dt, vector<Object*>object, Player* player);
};

#endif