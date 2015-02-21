#include "player.h"

static int bobXDir = 1;
static int bobYDir = 1;

static int knifeDir = 1;
Vector3 originalPos;

using namespace::std;

void Player::Update(double dt, vector<Object*>object)
{
	Control(dt, object);

	camera.position = position;
	camera.position.y += value[eyeLevel];
	camera.Update(dt, object);

	hOrientation = camera.orientation;
	vOrientation = camera.look;

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

	if(Application::IsKeyPressed(VK_SHIFT))
		velocity = Vector3(40,velocity.y,40);
	else
		velocity = Vector3(30,velocity.y,30);
	
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

	if(Application::IsKeyPressed(VK_SPACE) && !state[JUMP] && velocity.y == 0 && value[jumpCooldown] == 0)
	{
		originalPos = position; velocity.y = 30;
	}

	if (velocity.y > 0)
		state[JUMP] = true;
	else
		state[JUMP] = false;

	velocity.y -= 80 * dt;
	position.y += (float)(velocity.y * dt); 

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (!object[i]->ignoreCollision)
			if (checkCollision(object[i]))
			{
				Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
				Vector3 maxCube = Cube; maxCube += object[i]->position;
				Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

				Vector3 maxPlayer = collision.hitbox/2; maxPlayer.y = collision.hitbox.y; maxPlayer += initialPos;
				Vector3 minPlayer = -collision.hitbox/2; 

				if (state[JUMP])
					minPlayer.y = 0.1f;
				else
					minPlayer.y = 0.5f;

				minPlayer += initialPos;

				if (initialPos.y == maxCube.y || (Math::distBetween(initialPos.y, maxCube.y) <= 0.5f && velocity.y == 0)) //If standing on object or going up a block, check y first
				{
					if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && velocity.y <= 0)
					{
						position.y = maxCube.y; 
						velocity.y = 0;
					}

					else if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
						position.z = maxCube.z + collision.hitbox.z/2;

					else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
						position.z = minCube.z - collision.hitbox.z/2;

					else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
						position.x = maxCube.x + collision.hitbox.x/2;

					else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
						position.x = minCube.x - collision.hitbox.x/2;
					
					else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
					{	
						velocity.y = 0;
					}
				}
				else
				{
					if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
						position.z = maxCube.z + collision.hitbox.z/2;

					else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
						position.z = minCube.z - collision.hitbox.z/2;

					else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
						position.x = maxCube.x + collision.hitbox.x/2;

					else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
						position.x = minCube.x - collision.hitbox.x/2;

					else if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && velocity.y <= 0)
					{
						position.y = maxCube.y; 
						velocity.y = 0;
					}

					else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
					{	
						velocity.y = 0;
					}
				}

			}
	}
}

float inputDelay = 0;

void dynamicObject::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

	if (inputDelay > 0)
		inputDelay -= dt;
	else
		inputDelay = 0;

	yVelocity -= 40 * dt;
	position.y += (float)(yVelocity * dt); 

	Control(dt, object, player);
	RespondToCollision(initialPos, object, player);
}

void dynamicObject::Control(double dt, vector<Object*>object, Player* player)
{
	if (object[player->camera.lookAt] == this)
	{
		if (Application::IsKeyPressed('E') && player->holding < 0 && inputDelay == 0)
		{
			player->holding = player->camera.lookAt;
			inputDelay = 0.75f;
		}
	}

	if (player->holding >= 0)
	if (object[player->holding] == this)
	{
		position = player->camera.target;
		yVelocity = 0;

		if ((Application::IsKeyPressed('E') && inputDelay == 0))
		{
			player->holding = -1;
			inputDelay = 0.75f;
		}
	}
	
}

void dynamicObject::RespondToCollision(Vector3 initialPos, vector<Object*>object, Player* player)
{
	Vector3 Cube = collision.hitbox/2; Cube += collision.centre;
	Vector3 maxPlayer = Cube + initialPos;
	Vector3 minPlayer = Cube - collision.hitbox;
	minPlayer.y = 0.5f; 

	minPlayer += initialPos;

	if ( !(player->holding >= 0 && object[player->holding] == this) )
	if (player->checkCollision(this))
	{
		Vector3 maxCube = player->collision.hitbox/2; maxCube.y = player->collision.hitbox.y; maxCube += player->position;
		Vector3 minCube = -player->collision.hitbox/2; 
	
		if (player->state[player->JUMP])
			minCube.y = 0.1f;
		else
			minCube.y = 0.5f;

		minCube += player->position;

		if (initialPos.y == maxCube.y || (Math::distBetween(initialPos.y, maxCube.y) <= 0.5f && yVelocity == 0))  //If standing on object, check y first
		{
			if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
			{
				position.y = maxCube.y; 
				yVelocity = 0;
			}

			else if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
				position.z = maxCube.z + collision.hitbox.z/2;

			else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
				position.z = minCube.z - collision.hitbox.z/2;

			else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
				position.x = maxCube.x + collision.hitbox.x/2;

			else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
				position.x = minCube.x - collision.hitbox.x/2;

			else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
			{	
				yVelocity = 0;
			}
		}
		else
		{
			if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
				position.z = maxCube.z + collision.hitbox.z/2;

			else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
				position.z = minCube.z - collision.hitbox.z/2;

			else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
				position.x = maxCube.x + collision.hitbox.x/2;

			else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
				position.x = minCube.x - collision.hitbox.x/2;

			else if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
			{
				position.y = maxCube.y; 
				yVelocity = 0;
			}

			else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
			{	
				yVelocity = 0;
			}
		}
	}

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i] != this)
		if (!object[i]->ignoreCollision)
			if (Object::checkCollision(this, object[i]))
			{
				Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
				Vector3 maxCube = Cube; maxCube += object[i]->position;
				Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

				if (initialPos.y == maxCube.y || (Math::distBetween(initialPos.y, maxCube.y) <= 0.5f && yVelocity == 0)) //If standing on object, check y first
				{
					if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
					{
						position.y = maxCube.y; 
						yVelocity = 0;
					}

					else if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
						position.z = maxCube.z + collision.hitbox.z/2;

					else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
						position.z = minCube.z - collision.hitbox.z/2;

					else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
						position.x = maxCube.x + collision.hitbox.x/2;

					else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
						position.x = minCube.x - collision.hitbox.x/2;
					
					else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
					{	
						yVelocity = 0;
					}

					else
					{
						position.y = maxCube.y; 
						yVelocity = 0;
					}
				}
				else
				{
					if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
						position.z = maxCube.z + collision.hitbox.z/2;

					else if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
						position.z = minCube.z - collision.hitbox.z/2;

					else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
						position.x = maxCube.x + collision.hitbox.x/2;

					else if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
						position.x = minCube.x - collision.hitbox.x/2;

					else if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y && yVelocity <= 0)
					{
						position.y = maxCube.y; 
						yVelocity = 0;
					}

					else if(maxPlayer.y <= minCube.y && minPlayer.y <= minCube.y) //bump head
					{	
						yVelocity = 0;
					}

					else
					{
						position.y = maxCube.y; 
						yVelocity = 0;
					}
				}
			}
	}
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
