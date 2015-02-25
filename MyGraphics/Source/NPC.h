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

		hitDelay = 0;

		Init();
	};
	~NPC(void) {};

	Vector3 initialPos;
	Vector3 previousPos;
	Vector3 target;
	Bodypart bodyParts[NUM_BODYPARTS];
	string identity;
	string getIdentity() {return identity;};
	float hitDelay;
	float rotation[NUM_BODYPARTS];
	int rotationDir[NUM_BODYPARTS];
	float elapsedTime;

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player) {};
	
	void Update(double dt, vector<Object*>object, Player* player);
	void UpdateVelocity(double dt);
	void Orientate(float o, double dt, float speed);
	void Orientate(Vector3 t, double dt, float speed);
	void Goto(Vector3 destination);
	void Animate(double dt, float speed);
};

class Hobo : public NPC
{
public:
	Hobo()
	{
		position = Vector3(-25,0,-89.5);
		orientation = 180;
		Init();	
	};
	~Hobo() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Thug : public NPC
{
public:
	Thug()
	{
		position = Vector3(0,2,25);
		Init();	
	};
	Thug(Vector3 p)
	{
		position = p;
		Init();
	}
	~Thug() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Cashier : public NPC
{
public:
	Cashier()
	{
		position = Vector3(0,2,0);
		target = position; target.y = 0;
		Init();	
	};
	Cashier(Vector3 p)
	{
		position = p;
		target = position; target.y = 0;
		orientation = -90;
		Init();
	}
	~Cashier() {};

	Vector3 getPos()
	{
		return position;
	}

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Blindman : public NPC
{
public:
	Blindman()
	{
		position = Vector3(0,2,-100);
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);
		target = position + direction;
		target.y = 0;
		Init();	
	};
	Blindman(Vector3 p)
	{
		position = p;
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);
		target = position + direction;
		target.y = 0;
		Init();
	}
	~Blindman() {};

	Vector3 getPos()
	{
		return position;
	}

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class CustomerPath 
{
public:
	CustomerPath() {};
	~CustomerPath() {};

	vector<Object*> targetObj;
	vector<Vector3> targetPos;
};

class Customer : public NPC
{
public:
	Customer()
	{
		position = Vector3(-70,2,-70);
		target = position; target.y = 0;
		Init();	
	};
	Customer(Vector3 p)
	{
		position = p;
		target = position; target.y = 0;
		Init();
	}
	~Customer() {};

	Vector3 getPos()
	{
		return position;
	}

	CustomerPath pathing;
	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

#endif