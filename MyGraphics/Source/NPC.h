#ifndef NPC_H
#define NPC_H

#include "player.h"
#include "dialogue.h"

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
		quest = NULL;
		health = 100;
		elapsedTime = 0;
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
	float rotation[NUM_BODYPARTS];
	int rotationDir[NUM_BODYPARTS];
	float elapsedTime;
	vector<TextBox> greetings;
	vector<TextBox> clues;
	Quest * quest;
	bool inConversation;
	virtual bool getCriteria(vector<Object*>object) {return 0;};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player) {};
	
	virtual TextBox * getConversation(Player * player)
	{
		if (!inConversation)
		{
			inConversation = true;

			if (quest == NULL)
			{
				int r = rand() % greetings.size();
				return &greetings[r];
			}
			else if (quest->Accept.trigger)
				return quest->Accept.next;
			else
				return quest;
		}

		return NULL;
	};
	void InitClues(const char* filename);
	void InitDialogue(const char* filename);
	virtual void InitQuest(const char* filename);
	void Update(double dt, vector<Object*>object, Player* player);
	void Orientate(float o, double dt, float speed);
	void Orientate(Vector3 t, double dt, float speed);
	void Goto(Vector3 destination, double dt, float turn, float speed);
	void Animate(double dt, float speed);
	void Animate(double dt, float speed, float limit);
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
	void InitQuest(const char* filename);
	TextBox * getConversation(Player * player)
	{
		if (!inConversation)
		{
			inConversation = true;

			if (player->inventory.getHolding() == NULL)
			{
				int r = rand() % greetings.size();
				return &greetings[r];
			}
			else
				return quest;
		}

		return NULL;
	};
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

	void InitQuest(const char* filename);
	TextBox * getConversation(Player * player)
	{
		if (!inConversation)
		{
			inConversation = true;

			if (player->inventory.checkPaid())
			{
				int r = rand() % greetings.size();
				return &greetings[r];
			}
			else
				return quest;
		}

		return NULL;
	};
	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Blindman : public NPC
{
public:
	Blindman()
	{
		position = Vector3(0,2,-10);
		orientation = rand () % 360;
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);
		target = position + direction * 5;
		target.y = 0;
		Init();	
	};
	Blindman(Vector3 p)
	{
		position = p;
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);
		target = position + direction * 5;
		target.y = 0;
		Init();
	}
	~Blindman() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class NPCTarget
{
public:
	NPCTarget() {};
	NPCTarget(Vector3 p, float o) {position = p; orientation = o;};
	~NPCTarget() {};

	Vector3 position;
	float orientation;
};

class Customer : public NPC
{
public:
	Customer()
	{
		position = Vector3(0,2,0);
		target = position; target.y = 0;
		orientation = -90;
		Init();	
		InitDialogue("Filestream//customer.txt");
		//InitClues("Filestream//Quests//Hints//customer.txt");
	};
	Customer(vector<NPCTarget> t, string name, unsigned tID, float d)
	{
		identity = name;
		index = 0;
		decisionTime = d;
		position = t[0].position;
		orientation = t[0].orientation;;
		target = t[0].position; target.y = 0;
		tOrientation = t[0].orientation;
		targets = t;

		for (int i = 0; i < targets.size(); i++)
		{
			targets[i].position.y = 0;
		}

		for (int i = 0; i < NUM_BODYPARTS; i++)
		{	
			bodyParts[i].mesh->textureID = tID;
			bodyParts[i].position = position;
			bodyParts[i].identity = identity;
		}
		InitDialogue("Filestream//customer.txt");
		//InitClues("Filestream//Quests//Hints//customer.txt");
	}
	~Customer() {};

	int index;
	float decisionTime;
	float tOrientation;
	vector<NPCTarget> targets;
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Detective : public NPC
{
public:
	Detective()
	{
		position = Vector3(60,0,20);
		orientation = 30;
		rotation[HEAD] = 15;
		Init();
	};
	Detective(Vector3 p)
	{
		position = p;
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);
		target = position + direction;
		target.y = 0;
		Init();
	}
	~Detective() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class S_Guard : public NPC
{
public:
	S_Guard()
	{
		Init();	
	};
	S_Guard(Vector3 p)
	{
		position = p;
		orientation = 0;
		target = p; target.y = 0;
		original.position = target;
		original.orientation = 0;
		
		Init();	
	};
	~S_Guard() {};

	NPCTarget original;
	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

class Manager : public NPC
{
public:
	Manager()
	{
		position = Vector3(-20,27,10);
		target = position;
		orientation = 0;
		rotation[HEAD] = 15;
		Init();
	};
	Manager(Vector3 p)
	{
		position = p;
		target = position;
		Vector3 direction;
		Init();
	}
	~Manager() {};

	ManagerQuest collection;

	virtual bool getCriteria(vector<Object*>object) 
	{
		if(quest->Accept.trigger)
			return collection.Criteria(object);
		return 0;
	};
	virtual void Init();
	virtual void Control(double dt, vector<Object*>object, Player* player);
};

#endif