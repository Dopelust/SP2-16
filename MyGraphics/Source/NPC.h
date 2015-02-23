#ifndef NPC_H
#define NPC_H

#include "player.h"

class NPC : public dynamicObject
{
private:

public:
	enum BodyParts
	{
		HEAD,
		BODY,
		L_ARM,
		R_ARM,
		L_LEG,
		R_LEG,
		NUM_BODYPARTS,
	};
	NPC(void)
	{
		type = "NPC";
		mesh = NULL;

		collision.hitbox = Vector3(2.5f, 6.f, 2.5f);
		collision.boundingBox = MeshBuilder::GenerateCube("Hitbox", Color(1,1,1), collision.hitbox.x, collision.hitbox.y, collision.hitbox.z, 0);
		collision.centre = Vector3(0,collision.hitbox.y/2,0);

		position = Vector3(0,0,0);
		orientation = 0;
		Init();
	};
	~NPC(void) {};

	Bodypart bodyParts[NUM_BODYPARTS];
	string identity;
	string getIdentity() {return identity;};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player) {};
	virtual void Update(double dt, vector<Object*>object, Player* player) {};
};

class Hobo : public NPC
{
public:
	Hobo()
	{
		position = Vector3(0,2,-100);
		Init();	
	};
	~Hobo() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
	virtual void Update(double dt, vector<Object*>object, Player* player);
};

class Thug : public NPC
{
public:
	Thug()
	{
		position = Vector3(0,2,25);
		Init();	
	};
	~Thug() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
	virtual void Update(double dt, vector<Object*>object, Player* player);
};

class Cashier : public NPC
{
public:
	Cashier()
	{
		position = Vector3(0,2,0);
		Init();	
	};
	Cashier(Vector3 p)
	{
		position = p;
		Init();
	}
	~Cashier() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
	virtual void Update(double dt, vector<Object*>object, Player* player);
};

#endif