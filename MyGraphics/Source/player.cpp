#include "player.h"

static int bobXDir = 1;
static int bobYDir = 1;

static int knifeDir = 1;

using namespace::std;

void CollisionResponse(Vector3 initialPos, Vector3& position, Vector3 hitbox, Vector3 maxCube, Vector3 minCube, Vector3 maxPlayer, Vector3 minPlayer, float& yVelocity, bool failSafe)
{
	if (initialPos.y == maxCube.y || (Math::distBetween(minPlayer.y, maxCube.y) <= 0.4f && yVelocity >= -1.5f))  //If standing on object, check y first
	{
		if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
		{
			position.y = maxCube.y; 
			yVelocity = 0;
		}

		else if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
			position.z = maxCube.z + hitbox.z/2;

		else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
			position.z = minCube.z - hitbox.z/2;

		else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
			position.x = maxCube.x + hitbox.x/2;

		else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
			position.x = minCube.x - hitbox.x/2;

		else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
		{	
			yVelocity = 0;
		}

		else if (failSafe)
		{
			position.y = maxCube.y; 
			yVelocity = 0;
		}
	}
	else
	{
		if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
			position.z = maxCube.z + hitbox.z/2;

		else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
			position.z = minCube.z - hitbox.z/2;

		else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
			position.x = maxCube.x + hitbox.x/2;

		else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
			position.x = minCube.x - hitbox.x/2;

		else if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
		{
			position.y = maxCube.y; 
			yVelocity = 0;
		}

		else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
		{	
			yVelocity = 0;
		}

		else if (failSafe)
		{
			position.y = maxCube.y; 
			yVelocity = 0;
		}
	}
}

void Player::Update(double dt, vector<Object*>object)
{
	Vector3 initialPos = position;

	Control(dt, object);
	UpdateVelocity(dt);
	position += velocity * dt;
	RespondToCollision(initialPos, object, this);

	camera.position = position;
	camera.position.y += value[eyeLevel];
	camera.Update(dt, object);

	hOrientation = camera.orientation;
	vOrientation = camera.look;

	inventory.Update(dt);

	if (hitDelay > 0)
		hitDelay -= dt;
	else 
		hitDelay = 0;

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

	state[SPRINT] = false;

	if (hitDelay == 0)
	{
		Vector3 v;
		if(Application::IsKeyPressed('A'))
		{
			v += -right;
		}
		if( Application::IsKeyPressed('D'))
		{
			v += right;
		}
		if(Application::IsKeyPressed('W'))
		{
			v += direction;
		}
		if(Application::IsKeyPressed('S'))
		{
			v += -direction;
		}

		velocity.x = v.x * 30;
		velocity.z = v.z * 30;

		if (Application::IsKeyPressed(VK_SHIFT))
		{
			velocity.x *= 2;
			velocity.z *= 2;
		}

		if (count == 2)
		{
			velocity.x /= 1.5f;
			velocity.z /= 1.5f;
		}
	}

	if(Application::IsKeyPressed(VK_SPACE) && !state[JUMP] && velocity.y == 0 && value[jumpCooldown] == 0)
	{
		if (Application::IsKeyPressed(VK_CONTROL))
			velocity.y = 90;
		else
			velocity.y = 30;
	}

	if (velocity.y > 0)
		state[JUMP] = true;
	else
		state[JUMP] = false;
}

float eDelay = 0;

void dynamicObject::Update(double dt, vector<Object*>object, Player* player)
{
	if (position.Dist(player->position) < 25.f)
	{
		Vector3 initialPos = position;

		velocity.y -= 40 * dt;
		position += velocity * (float)dt; 

		Control(dt, object, player);

		RespondToCollision(initialPos, object, player);
	}
}

void dynamicObject::RespondToCollision(Vector3 initialPos, vector<Object*>object, Player* player)
{
	Vector3 Cube = collision.hitbox/2; Cube += collision.centre;
	Vector3 maxPlayer = Cube + initialPos;
	Vector3 minPlayer = Cube - collision.hitbox;

	if (this == player)
	{
		if (player->state[player->JUMP])
			minPlayer.y = 0.1f;
		else
			minPlayer.y = 0.4f; 
	}

	minPlayer.y = 0.4f; 

	minPlayer += initialPos;

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if ( !(player->holding >= 0 && object[player->holding] == this) )
			if (object[i] != this)
			{
				if ((type == "Player" || type == "NPC") && object[i]->ignoreCollision)
					continue;

				if (type == "Dynamic" && object[i]->ignoreCollision)
					if (object[i]->type != "Item")
						continue;

				if (Object::checkCollision(this, object[i]))
				{
					Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
					Vector3 maxCube = Cube; maxCube += object[i]->position;
					Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

					if (type == "Dynamic" && object[i]->type == "Dynamic")
						CollisionResponse(initialPos, position, collision.hitbox, maxCube, minCube, maxPlayer, minPlayer, velocity.y, true);
					else
						CollisionResponse(initialPos, position, collision.hitbox, maxCube, minCube, maxPlayer, minPlayer, velocity.y, false);
				}
			}
	}
}
/*
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
*/
void Doorway::Update(double dt, vector<Object*>object, Player* player) 
{
	Vector3 initialPos[2];
	initialPos[0] = Door[0].position;
	initialPos[1] = Door[1].position;

	if (open == true)
	{
		elapsedTime += float(dt);

		if (elapsedTime > 3.f)
		{
			open = false;
			Button[0].mesh = buttonStatus[0]; Button[1].mesh = buttonStatus[0];
			elapsedTime = 0;
		}
		
		Door[0].position.x -= float(10.f * dt);
		Door[1].position.x += float(10.f * dt);

		if(Door[0].position.x <= doorPosition[0].x - Door[0].collision.hitbox.x)
			Door[0].position.x = doorPosition[0].x - Door[0].collision.hitbox.x;
		if(Door[1].position.x >= doorPosition[1].x + Door[1].collision.hitbox.x)
			Door[1].position.x = doorPosition[1].x + Door[1].collision.hitbox.x;
	}

	else
	{
		Door[0].position.x += float(10.f * dt);
		Door[1].position.x -= float(10.f * dt);

		if (Door[0].position.x >= doorPosition[0].x)
		{
			Door[0].position.x = doorPosition[0].x; close = true;
		}
		if (Door[1].position.x <= doorPosition[1].x)
		{
			Door[1].position.x = doorPosition[1].x; close = true;
		}
	}

	if (close == false)
	{
		for (int d = 0; d < 2; d++)
		{
			for (unsigned int i = 0; i < object.size(); i++)
			{
				if (object[i] != &Door[d])
					if(object[i]->type == "Dynamic" || object[i]->type == "NPC" || object[i]->type == "Player")
						if (Object::checkCollision(&Door[d], object[i]))
						{
							open = true; close = false;
							Button[0].mesh = buttonStatus[1]; Button[1].mesh = buttonStatus[1];
							elapsedTime = 0;
						}
			}
		}
	}
}


void Doorway::RangeUpdate(double dt, vector<Object*>object, Player* player) 
{
	close = true; open = false;

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i] != &Range)
		{
			if ((object[i]->type == "Player" || object[i]->type == "NPC"))
				if (Object::checkCollision(&Range, object[i]))
				{
					open = true; close = false;
					break;
				}
		}
	}
}

void Doorway::ButtonUpdate(double dt, vector<Object*>object, Player* player) 
{
	if (player->holding < 0)
	if (Application::IsKeyPressed('E') && (object[player->camera.lookAt] == &Button[0] || object[player->camera.lookAt] == &Button[1]))
	{
		open = true; close = false;
		Button[0].mesh = buttonStatus[1]; Button[1].mesh = buttonStatus[1];
		elapsedTime = 0;
	}
}