/******************************************************************************/
/*!
\file	dialogue.h
\author Ricsson
\par	
\brief
This is where the textbox is contained as well as the various quests.
*/
/******************************************************************************/
#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "inventory.h"

#include <string>

using namespace::std;

class TextBox;

/******************************************************************************/
/*!
		Class Button2D:
\brief	Button2D class for the Accept and Decline button when talking to NPCs
*/
/******************************************************************************/
class Button2D
{
public:
	Button2D() {};
	Button2D(string t, Vector3 pos) {position = pos; text = t; trigger = false;}
	~Button2D() {};

	Vector3 position;
	string text;
	TextBox * next;
	TextBox * altNext;
	bool trigger;
};

/******************************************************************************/
/*!
		Class TextBox:
\brief	This is for the Textbox whenever you talk to an NPC
*/
/******************************************************************************/
class TextBox
{
public:
	TextBox() {};
	TextBox(string t)
	{
		text = t;
		type = "Normal";
		apparentext = "";
	};
	TextBox(string t, string T)
	{
		text = t;
		type = T;
		apparentext = "";
	};
	~TextBox() {};

	string type;
	static Vector3 position;
	string apparentext;
	string text;

	void Update();
	virtual Button2D getAccept() {return Button2D();};
	virtual void triggerQuest() {return;}
	virtual Button2D getDecline() {return Button2D();};
	virtual string GenerateText(Inventory inventory) {return "";};
};


/******************************************************************************/
/*!
		Class Quest: TextBox
\brief	This is for creating quests that the player can take from various NPCs
*/
/******************************************************************************/
class Quest : public TextBox
{
public:
	Quest() {};
	Quest(string t)
	{
		criteria = false;
		text = t;
		type = "Quest";

		Accept = Button2D("Accept", Vector3(7,-9.5f,0));
		Decline = Button2D("Decline", Vector3(11,-9.5f,0));
	};
	Quest(string t, string Type, string a, string d)
	{
		criteria = false;
		text = t;
		type = Type;

		Accept = Button2D(a, Vector3(7,-8.3f,0));
		Decline = Button2D(d, Vector3(11,-8.3f,0));
	};
	~Quest() {};

	Button2D Accept;
	Button2D Decline;

	Button2D getAccept() {return Accept;};
	Button2D getDecline() {return Decline;};
	void triggerQuest() {Accept.trigger = true;};
	bool criteria;
};

/******************************************************************************/
/*!
		Class Checkout: Quest
\brief	This is for when the player wants to check out an item from the store.
*/
/******************************************************************************/
class Checkout : public Quest
{
public:
	Checkout() {};
	Checkout(string t)
	{
		text = t;
		type = "Checkout";

		Accept = Button2D("Yes", Vector3(7,-8.3f,0));
		Decline = Button2D("Not Yet", Vector3(11,-8.3f,0));
	}
	~Checkout() {};

	string GenerateText(Inventory inventory) 
	{
		string price;
		if( discount == true )
		{
		price = to_string(long double( inventory.checkPrice() ));
		return text + price;
		}
		else
		{
		price = to_string(long double( inventory.checkPrice()/2 ));
		return text + price;
		}
	}
};

/******************************************************************************/
/*!
		Class Donation: Quest
\brief	This quest is for specifically for the Hobo class.
*/
/******************************************************************************/
class Donation : public Quest
{
public:
	Donation() {};
	Donation(string t)
	{
		text = t;
		type = "Donation";

		Accept = Button2D("Yes", Vector3(7,-8.3f,0));
		Decline = Button2D("No", Vector3(11,-8.3f,0));
	}
	~Donation() {};

	string GenerateText(Inventory inventory) 
	{
		string t;

		for (unsigned int i = 0; i < text.size(); i++)
		{
			if (text[i] != 'X')
				t += text[i];

			else
				t += inventory.getHolding()->mesh->name;
		}

		return t;
	}
};

/******************************************************************************/
/*!
		Class ManagerQuest
\brief	This quest is for specifically for the Manager class.
*/
/******************************************************************************/
class ManagerQuest
{
public:
	ManagerQuest() {
		Vector3 hitBox = Vector3(8.5,8.5,8.5);
		box_range = Object(Vector3(-30.f,27.1f,15.f), Vector3(0,hitBox.y/2,0), hitBox, NULL);
	};
	~ManagerQuest() {};

	Object box_range;
	bool Criteria(vector<Object *>object) 
	{ 
		int count = 0;
		for(unsigned int i = 0; i < object.size(); i++)
		{
			if( object[i]->mesh != NULL && object[i]->mesh->name == "Crate")
			{
				if(Object::checkCollision(object[i], &box_range))
					count++;
			}
		}

		if(count >= 3)
		{
			return true;
		}
		return false;

	}
};
#endif