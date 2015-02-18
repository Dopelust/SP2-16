#ifndef NPC_H
#define NPC_H

#include "player.h"

class NPC : public Player
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
		position = Vector3(0,0,0);
		Init();
	};
	~NPC(void) {};

	virtual void Init();
	Bodypart bodyParts[NUM_BODYPARTS];

};

class Hobo : public NPC
{
public:
	Hobo()
	{
		position = Vector3(0,0,-100);
		Init();	
	};
	~Hobo() {};

	virtual void Init();
	virtual void Control(double dt, vector<Object*>object) {};
	virtual void Update(double dt, vector<Object*>object) {};
};

#endif