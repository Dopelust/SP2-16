/******************************************************************************/
/*!
\file	collision.h
\author Ricsson
\par	
\brief
This is the collsion class using AABB.
*/
/******************************************************************************/
#ifndef COLLISION_H
#define COLLISION_H

#include "Vector3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

#include <string>
#include <vector>

using std::vector;
using std::string;

class Player;

/******************************************************************************/
/*!
		Struct Collision:
\brief	This is a struct that has the positions of the hitBox size and its centre.
*/
/******************************************************************************/
struct Collision
{
	Vector3 centre;
	Vector3 hitbox;
};

/******************************************************************************/
/*!
		Class Object:
\brief	This is the object class that takes in a lot of member variables depending on what is needed
like if there is no mesh or whether or not the object is an item that can be paid for.
*/
/******************************************************************************/
class Object
{
public:
	Object() { ignoreCollision = false; };
	Object(Vector3 p, Vector3 c, Vector3 h); 
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m); //Normal Objects
	Object(Vector3 p, Vector3 c, Vector3 h, float s, float o, bool i); 
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i); //Full Constructor
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i, bool paid) {}; //Item Constructor
	virtual ~Object() {};

	std::string type;
	Vector3 position;
	Collision collision;
	Mesh* mesh;
	float orientation;
	float size;
	bool ignoreCollision;
	static bool checkCollision(Object* a, Object* b);

	virtual void Update(double dt) {};
	virtual void Update(double dt, vector<Object*>object, Player* player) {};

	//Derived
	virtual bool Knockback(Vector3 dir, Vector3 vel) {return 0;};
	virtual void setPaid(bool p) {return;};
	virtual bool getPaid() {return 0;};
	virtual float getHealth() {return 0;};
	virtual float getValue() {return 0;};
	virtual string getIdentity() {return "";};
	virtual Vector3 getStorePos(Player* player) {return Vector3();};
};

/******************************************************************************/
/*!
		Class Body: inherits object
\brief	This is the Body class which inherits somethings from the Object class. This class is for the
NPCs bodyparts animation and hitbox as well.
*/
/******************************************************************************/
class Bodypart : public Object
{
public:
	Bodypart() { ignoreCollision = false; };
	Bodypart(Vector3 p, Vector3 c, Vector3 h, Mesh* m) : Object(p,c,h)
	{
		position = p;
		collision.centre = c;
		collision.hitbox = h;
		mesh = m;
		type = "Bodypart";
	};
	~Bodypart() {};

	string getIdentity() {return identity;};
	string identity;
};

/******************************************************************************/
/*!
		Class Money: inherits object
\brief	Money class which has object properties for initializing them.
*/
/******************************************************************************/
class Money : public Object
{
public:
	Money() {ignoreCollision = true;};
	Money(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, float v) : Object(p,c,h,m,s,o,true) {type = "Money"; value = v;};
	~Money() {};

	float value;
	float getValue() {return value;};
};

/******************************************************************************/
/*!
		Class Item: inherits object
\brief	Item class which is used for initializing item objects for the player to pick up.
*/
/******************************************************************************/
class Item : public Object
{
public:
	Item() {ignoreCollision = true;};
	Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i) :	Object(p,c,h,m,s,o,i) {paid = false; type = "Item";}
	Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i, float P, float R) : Object(p,c,h,m,s,o,i) {restore = R;price = P; paid = false; type = "Item";};
	Item(Mesh * m, string t, bool p) {mesh = m; type = t; paid = p;}
	~Item() {};

	void setPaid(bool p) {paid = p;};
	bool getPaid() {return paid;};
	bool paid;

	float price;
	float restore;
	float getValue() {return price;};
	float getHealth() {return restore;};
};

/******************************************************************************/
/*!
		Class Vending:
\brief	Vending class which is used for initializing the vending machine and its functions.
*/
/******************************************************************************/
class Vending
{
public:
	Vending() {};
	Vending(Object D) 
	{
		Drink = D; Drink.type = "Vending Machine"; 
		price = 2.f;
	};
	~Vending() {};

	float price;
	Item * generateDrink() 
	{ 
		price *= 2;

		Mesh * canMesh;
		canMesh =  MeshBuilder::GenerateOBJ("Statboost Drink", "OBJ//can.obj");
		canMesh->textureID = LoadTGA("Image//Food//mountaindew.tga");

		return new Item(canMesh, "Statboost", true);
	};

	Object Drink;
};

/******************************************************************************/
/*!
		Class Storage: object
\brief	Storage calss which is used for the shelves that can store Items.
*/
/******************************************************************************/
class Storage : public Object
{
public:
	Storage() {};
	Storage(Vector3 p, Vector3 c, Vector3 h) : Object(p,c,h) {type = "Storage"; mesh = NULL;};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* m) : Object(p,c,h,m) {type = "Storage";};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float o) : Object(p,c,h,m) {type = "Storage"; orientation = o;};
	~Storage() {};

	virtual Vector3 getStorePos(Player* player);
};

class Break : public Object
{
public:
	Break() {};
	Break(Vector3 p, Vector3 c, Vector3 h, Mesh* m, float s, float o, bool i) : Object(p,c,h,m,s,o,i) {type = "Break";}; 
	~Break() {};
};

/******************************************************************************/
/*!
		Class Storage: object
\brief	Particles class which is used for particle effects when the player hits the NPC.
*/
/******************************************************************************/
class Particles
{
public:
	Particles(Vector3 pos) 
	{
		float size = float(rand() % 5 + 1);
		float red = float(rand() % 10 + 1);
		mesh = MeshBuilder::GenerateCircle("Blood", Color(red/10, 0, 0), size/20);

		direction.x = float ((rand() % 11) - 5);
		direction.y = float ((rand() % 11) - 5);
		direction.z = float ((rand() % 11) - 5);

		position = pos; elapsedTime = 0;
	};
	~Particles() {};

	Mesh * mesh;
	Vector3 position;
	Vector3 direction;
	float elapsedTime;

	void Update(double dt);
};

#endif