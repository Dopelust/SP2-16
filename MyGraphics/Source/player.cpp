#include "player.h"

static int bobXDir = 1;
static int bobYDir = 1;

static int knifeDir = 1;
Vector3 originalPos;

using namespace::std;

void Player::InitPos()
{
	value[eyeLevel] = 5.5f;
	collision.hitbox = Vector3(2.f, 6.f, 2.f);
	collision.centre = Vector3(0, collision.hitbox.y/2, 0);
}
void Player::Update(double dt, vector<Object*>object)
{
	Control(dt, object);
	inventory.Update(dt);

	if (value[jumpCooldown] > 0)
		value[jumpCooldown] -= dt;
	if (value[jumpCooldown] <= 0)
		value[jumpCooldown] = 0;

	if (state[WALK] ==  false) //resetting to original location
	{	
		if(value[bobbingX] * bobXDir >= 0) //need to minus to reset
			bobXDir = -bobXDir;

		else if(value[bobbingX] * bobXDir <= 0) //need to plus to reset
			bobXDir = bobXDir;

		if(!(value[bobbingX] > -0.05f && value[bobbingX] < 0.05f)) //if not 0, reset
			value[bobbingX] += (float)(bobXDir * 1.0f * dt); 

		if(value[bobbingY] * bobYDir >= 0) //need to minus to reset
			bobYDir = -bobYDir;

		else if(value[bobbingY] * bobYDir <= 0) //need to plus to reset
			bobYDir = bobYDir;

		if(!(value[bobbingY] > -0.05f && value[bobbingY] < 0.05f)) //if not 0, reset
			value[bobbingY] += (float)(bobYDir * 1.0f * dt); 
	}

	else if (state[WALK] == true)
	{
		if(value[bobbingX] * bobXDir > 0.3f)
			bobXDir = -bobXDir;
		value[bobbingX] += (float)(bobXDir * 1.0f * dt);
		if (state[SPRINT] == true)
			value[bobbingX] += (float)(bobXDir * 1.0f * dt);

		if(value[bobbingY] * bobYDir > 0.15f)
			bobYDir = -bobYDir;
		value[bobbingY] += (float)(bobYDir * 1.0f * dt);
		if (state[SPRINT] == true)
			value[bobbingY] += (float)(bobYDir * 1.0f * dt);
	}

	if(Application::mouseButton(0) && value[knifeCooldown] == 0)
	{
		state[KNIFE] = true;
		value[knifeCooldown] = 1.f;
	}

	if (value[knifeCooldown] > 0.f)
	{
		value[knifeCooldown] -= float(dt);

		if (value[knifeCooldown] < 0.f)
			value[knifeCooldown] = 0.f;
	}

	if (state[KNIFE] == true)
	{
		if(value[knifeRotation] > 30.f)
		{
			value[knifeStrike]+= 1.f;
		}

		if (!value[knifeStrike])
			value[knifeRotation] += (float)(knifeDir * 300.f * dt);
		else if (value[knifeStrike] >= 20)
			value[knifeRotation] -= (float)(knifeDir * 80.f * dt);

		if(value[knifeRotation] < 0.f)
		{
			value[knifeRotation] = 0.f;
			state[KNIFE] = false;
			value[knifeStrike] = 0.f;
		}
	}

	for (unsigned int i = 0; i < loot.size(); i++)
	{
		loot[i]->Update(dt);

		if (loot[i]->elapsedTime > 0.8f)
		{
			delete loot[i];
			loot.erase(loot.begin()+i);
		}
	}

	state[WALK] = false;
}
void Player::Control(double dt, vector<Object*>object)
{
	Vector3 direction;
	direction.SphericalToCartesian(hOrientation, 0.f);
	Vector3 right;
	right.SphericalToCartesian(hOrientation - 90.f, 0.f);

	int count = 0;
	
	if(Application::IsKeyPressed('W'))
		count++;
	if(Application::IsKeyPressed('A'))
		count++;
	if(Application::IsKeyPressed('S'))
		count++;
	if(Application::IsKeyPressed('D'))
		count++;

	if(Application::IsKeyPressed(VK_CONTROL))
	{
		collision.hitbox.y = 3.f;
		collision.centre.y = collision.hitbox.y/2;
		value[eyeLevel] = 2.5f;
		velocity = Vector3(15,30,15);
	}
	else
	{
		InitPos();
	}

	if(Application::IsKeyPressed(VK_SHIFT) && !Application::IsKeyPressed(VK_CONTROL))
		velocity = Vector3(40,30,40);
	else if (!Application::IsKeyPressed(VK_CONTROL))
		velocity = Vector3(30,30,30);
	
	if (count == 2)
	{
		velocity.x /= 1.5f;
		velocity.z /= 1.5f;
	}

	state[SPRINT] = false;

	Vector3 initialPos = position;

	if(Application::IsKeyPressed('A'))
	{
		position -= right * velocity * float(dt); state[WALK] = true;
		if(Application::IsKeyPressed(VK_SHIFT))
			state[SPRINT] = true;			
	}
	if( Application::IsKeyPressed('D'))
	{
		position += right * velocity * float(dt); state[WALK] = true;
		if(Application::IsKeyPressed(VK_SHIFT))
			state[SPRINT] = true;
	}
	if(Application::IsKeyPressed('W'))
	{
		position += direction * velocity * float(dt); state[WALK] = true;
		if(Application::IsKeyPressed(VK_SHIFT))
			state[SPRINT] = true;
	}
	if(Application::IsKeyPressed('S'))
	{
		position -= direction * velocity * float(dt); state[WALK] = true;
		if(Application::IsKeyPressed(VK_SHIFT))
			state[SPRINT] = true;
	}

	if(Application::IsKeyPressed(VK_SPACE) && !state[JUMP] && value[jumpCooldown] == 0 && Math::distBetween(position.y, value[groundLevel]) >= 0 && Math::distBetween(position.y, value[groundLevel]) < 0.75f)
	{
		state[JUMP] = true; state[GROUND] = false; originalPos = position; value[jumpCooldown] = 0.5f; 
	}

	if (state[JUMP]) //Jump
	{
		position.y += (float)(velocity.y * dt); 

		if(position.y > (originalPos.y + 5))
		{
			state[JUMP] = false;
		}
	}

	else //Fall to ground
	{
		if (position.y > value[groundLevel])
		{
			position.y -= (float)(velocity.y * dt); 

			if (position.y < value[groundLevel])
			{
				position.y = value[groundLevel];
				state[GROUND] = true; 
			}
		}
		else
		{
			velocity.y = 40;
			state[GROUND] = true;
		}
	}
	value[groundLevel] = 0.f; //Default
	
	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (!object[i]->ignoreCollision)
		if (checkCollision(object[i]))
		{
			Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
			Vector3 maxCube = Cube; maxCube += object[i]->position;
			Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

			Vector3 maxPlayer = collision.hitbox/2; maxPlayer.y = collision.hitbox.y; maxPlayer += initialPos;
			Vector3 minPlayer = -collision.hitbox/2; minPlayer.y = 0; minPlayer += initialPos;

			Vector3 pos = position; //y is ground
						
			if(Vector3(0,1,0).Dot(maxCube - pos) < 0.75f)
			{
				value[groundLevel] = maxCube.y;
			}
			
			else if (Vector3(0,1,0).Dot(maxCube - pos) < 2.5f && state[GROUND] == true)
			{
				value[groundLevel] = maxCube.y;
				position.y = maxCube.y;
			}

			else
			{
				if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
				{
					position.z = maxCube.z + collision.hitbox.z/2;
				}

				if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
				{
					position.z = minCube.z - collision.hitbox.z/2;
				}

				if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
				{
					position.x = maxCube.x + collision.hitbox.x/2;
				}

				if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
				{
					position.x = minCube.x - collision.hitbox.x/2;
				}
			}

			pos = position; pos.y += collision.hitbox.y; //head level
			if(  Vector3(0,-1,0).Dot(minCube - pos) < 0.75f && state[JUMP] == true) //bump head
			{	
				state[JUMP] = false;
			}
		}
	}
	
	float yaw = (float)(value[mouseSens] * dt * (float)(800/2 - Application::getMousePos().x));
	hOrientation += yaw;

	if (vOrientation <= 90 && vOrientation >= -90)
	{
		float pitch = (float)(value[mouseSens] * dt * (float)(600/2 - Application::getMousePos().y));
		vOrientation += pitch;
	}

	if (vOrientation >= 90.f)
		vOrientation = 90.f;
	if (vOrientation <= -90.f)
		vOrientation = -90.f;
}

bool Player::checkCollision(Object* b)
{
	Vector3 CubeA = collision.hitbox/2; CubeA += collision.centre;
	Vector3 CubeB = b->collision.hitbox/2; CubeB += b->collision.centre;

	Vector3 maxCubeA = CubeA; maxCubeA += position;
	Vector3 minCubeA = CubeA - collision.hitbox; minCubeA += position;

	Vector3 maxCubeB = CubeB;  maxCubeB += b->position;
	Vector3 minCubeB = CubeB - b->collision.hitbox; minCubeB += b->position;

	return(maxCubeA.x > minCubeB.x && 
	minCubeA.x < maxCubeB.x &&
    maxCubeA.y > minCubeB.y &&
    minCubeA.y < maxCubeB.y &&
    maxCubeA.z > minCubeB.z &&
    minCubeA.z < maxCubeB.z);
} 
void Loot::Update(double dt)
{
	elapsedTime += float(dt);

	if (elapsedTime < 0.8f)
	{
		textPos.y += 0.03f;
	}
}