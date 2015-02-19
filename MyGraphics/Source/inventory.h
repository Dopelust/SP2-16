#ifndef INVENTORY_H
#define INVENTORY_H

#include "Application.h"
#include "collision.h"

#include <vector>
#include <string>

using namespace::std;

class Slot
{
public:
	Slot() {};
	~Slot() {};

	Vector3 position;
	vector<Object*> item;
	Mesh * mesh;
};

class Selector
{
public:
	Selector() {};
	~Selector() {};

	Mesh * mesh;
	Slot * selectedSlot;
};

class Inventory
{
public:
	Inventory() {};
	~Inventory() {};

	Vector3 position;
	Slot slots[9];
	Selector selector;
	int index;
	int emptySlot();

	bool Insert(Object* o);
	void Init(Vector3 pos);
	void Update(double dt);
};

#endif