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

struct Collision
{
	Vector3 centre;
	Vector3 hitbox;

	Mesh* boundingBox;
};

class Object
{
public:
	Object() { ignoreCollision = false; };
	Object(Vector3 p, Vector3 c, Vector3 h); 
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* b); //Bounding Box Only
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b); //Normal Objects
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i); //Full Constructor
	Object(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i, bool paid) {}; //Item Constructor
	~Object() {};

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

class Money : public Object
{
public:
	Money() {ignoreCollision = true;};
	Money(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, float v) : Object(p,c,h,m,b,s,o,true) {type = "Money"; value = v;};
	~Money();

	float value;
	float getValue() {return value;};
};

class Vending
{
public:
	Vending() {};
	Vending(Object D) { Drink = D; Drink.type = "Vending Machine"; drinks = 3;};
	~Vending() {};

	float drinks;
	float getDrinks() {return drinks;};
	
	bool drink() 
	{ 
		if (drinks != 0)
		{
			drinks -= 1; 
			return true;
		}
		return false;
	};

	Object Drink;
};

class Item : public Object
{
public:
	Item() {ignoreCollision = true;};
	Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i) :	Object(p,c,h,m,b,s,o,i) {paid = false; type = "Item";}
	Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i, float P, float R) :	Object(p,c,h,m,b,s,o,i) {restore = R;price = P; paid = false; type = "Item";};
	//Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i, bool P) :	Object(p,c,h,m,b,s,o,i) {paid = P; type = "Item";};
	~Item();

	void setPaid(bool p) {paid = p;};
	bool getPaid() {return paid;};
	bool paid;

	float price;
	float restore;
	float getValue() {return price;};
};

class Storage : public Object
{
public:
	Storage() {};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* b) : Object(p,c,h,b) {type = "Storage";};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b) : Object(p,c,h,m,b) {type = "Storage";};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float o) : Object(p,c,h,m,b) {type = "Storage"; orientation = o;};
	~Storage() {};

	virtual Vector3 getStorePos(Player* player);
};

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