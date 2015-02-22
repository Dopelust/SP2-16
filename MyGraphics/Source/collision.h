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
	virtual bool getPaid() {return 0;};
	virtual string getIdentity() {return "";};
	virtual Vector3 getStorePos(Player* player) {return Vector3();};
};

class Bodypart : public Object
{
public:
	Bodypart() { ignoreCollision = false; };
	Bodypart(Vector3 p, Vector3 c, Mesh* m)
	{
		position = p;
		collision.centre = c;
		mesh = m;
		type = "Bodypart";
	};
	~Bodypart() {};

	string getIdentity() {return identity;};
	string identity;
};

class Item : public Object
{
public:
	Item() {ignoreCollision = true;};
	Item(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b, float s, float o, bool i) :	Object(p,c,h,m,b,s,o,i) {paid = false; type = "Item";};
	~Item();

	bool getPaid() {return paid;};
	bool paid;
};

class Storage : public Object
{
public:
	Storage() {};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* b) : Object(p,c,h,b) {type = "Storage";};
	Storage(Vector3 p, Vector3 c, Vector3 h, Mesh* m, Mesh* b) : Object(p,c,h,m,b) {type = "Storage";};
	~Storage() {};

	virtual Vector3 getStorePos(Player* player);
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

	void Update(double dt);
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