#ifndef INVENTORY_H
#define INVENTORY_H

#include "Application.h"
#include "collision.h"

#include <vector>
#include <string>

using namespace::std;

class Wallet
{
public:
	Wallet() {apparentValue = 0.f; trueValue = 50.f;};
	~Wallet() {};

	float trueValue;
	float apparentValue;

	void Update();
};


class ATM
{
public:
	ATM() {};
	ATM(Object D, Object W) { Deposit = D; Withdraw = W; Deposit.type = "ATM"; Withdraw.type = "ATM"; savings = 0;};
	~ATM() {};

	float savings;
	float getSavings() {return savings;};
	bool withdraw(Wallet& wallet) 
	{ 
		if (savings != 0)
		{
			savings--;
			wallet.trueValue++;

			return true;
		}
		return false;
	};
	bool deposit(Wallet& wallet) 
	{ 
		if (wallet.trueValue != 0)
		{
			wallet.trueValue--;
			savings++;

			return true;
		}
		return false;
	};

	Object Deposit;
	Object Withdraw;
};

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

	Wallet wallet;
	Vector3 position;
	Slot slots[9];
	Selector selector;
	int index;
	int emptySlot();
	Mesh * slotMeshes[3];

	bool checkPaid();
	bool canUse();
	bool Insert(Object* o);

	float checkPrice();
	bool Checkout();

	bool Remove();
	void Init(Vector3 pos);
	void Update(double dt);
};

#endif