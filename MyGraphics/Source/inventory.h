/******************************************************************************/
/*!
\file	inventory.h
\author Ricsson
\par	
\brief
This is for player inventory
*/
/******************************************************************************/
#ifndef INVENTORY_H
#define INVENTORY_H

#include "Application.h"
#include "collision.h"

#include <vector>
#include <string>

using namespace::std;

/******************************************************************************/
/*!
		Class Wallet:
\brief	This is the players wallet when he picks up money obj.
*/
/******************************************************************************/
class Wallet
{
public:
	Wallet() {apparentValue = 0.f; trueValue = 50.f;};
	~Wallet() {};

	float trueValue;
	float apparentValue;

	void Update();
};

/******************************************************************************/
/*!
		Class ATM:
\brief	This is for the automated teller machines outside the supermarket.
One iof for withdrawing and the other for depositing.
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
		Class Slot:
\brief	This is the players inventory UI.
*/
/******************************************************************************/
class Slot
{
public:
	Slot() {};
	~Slot() {};

	Vector3 position;
	vector<Object*> item;
	Mesh * mesh;
};

/******************************************************************************/
/*!
		Class Selector:
\brief	This is the selector indicator in the players inventory.
*/
/******************************************************************************/
class Selector
{
public:
	Selector() {};
	~Selector() {};

	Mesh * mesh;
	Slot * selectedSlot;
};

/******************************************************************************/
/*!
		Class Inventory:
\brief	Basic functionality of keepnig track of looted items as well as checking out items.
*/
/******************************************************************************/
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
	bool Delete();

	void Init(Vector3 pos);
	void Update(double dt);

	Object * getHolding()
	{
		if (!selector.selectedSlot->item.empty())
			return selector.selectedSlot->item[ selector.selectedSlot->item.size() - 1 ];

		return NULL;
	}
};

#endif