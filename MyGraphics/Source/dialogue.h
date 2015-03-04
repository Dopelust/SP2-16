#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "inventory.h"

#include <string>

using namespace::std;

class TextBox;

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
		price = to_string(long double( inventory.checkPrice() ));
		return text + price;
	}
};

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

		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] != 'X')
				t += text[i];

			else
				t += inventory.getHolding()->mesh->name;
		}

		return t;
	}
};

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
		for(int i = 0; i < object.size(); i++)
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