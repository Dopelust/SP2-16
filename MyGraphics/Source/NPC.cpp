#include "NPC.h"

bool dynamicObject::Knockback(Vector3 dir, Vector3 vel)
{
	if (hitDelay == 0)
	{

		health -= (vel.Length() / (10 + rand () % 8));
		
		velocity.x = dir.x * vel.x;
		velocity.z = dir.z * vel.z;
		velocity.y = vel.y;
		
		hitDelay = 0.5f;

		return true;
	}

	return false;
}

void NPC::Init()
{
	Mesh* tempMesh;

	tempMesh = MeshBuilder::GenerateOBJ("Head", "OBJ//CharOBJ//Head.obj");
	bodyParts[HEAD] = Bodypart(position, Vector3(0,5.25f,0), Vector3(1.5f, 1.5f, 1.5f), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Body", "OBJ//CharOBJ//Body.obj");
	bodyParts[BODY] = Bodypart(position, Vector3(0,3.375f,0), Vector3(1.5f,2.25f,0.75f), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Left Arm", "OBJ//CharOBJ//LeftArm.obj");
	bodyParts[L_ARM] = Bodypart(position, Vector3(-1.125f,3.375f,0), Vector3(0.75f, 2.25f, 0.75f), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Right Arm", "OBJ//CharOBJ//RightArm.obj");
	bodyParts[R_ARM] = Bodypart(position, Vector3(1.125f,3.375f,0), Vector3(0.75f, 2.25f, 0.75f), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Left Leg", "OBJ//CharOBJ//LeftLeg.obj");
	bodyParts[L_LEG] = Bodypart(position, Vector3(-0.375f,1.125f,0), Vector3(0.75f, 2.25f, 0.75f), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Right Leg", "OBJ//CharOBJ//RightLeg.obj");
	bodyParts[R_LEG] = Bodypart(position, Vector3(0.375f,1.125f,0), Vector3(0.75f, 2.25f, 0.75f), tempMesh);

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].ignoreCollision = true;
		rotation[i] = 0.f;

		if (i == L_ARM || i == R_LEG)
			rotationDir[i] = 1.f;
		else
			rotationDir[i] = -1.f;
	}
}

void NPC::Orientate(float o, double dt, float speed)
{
	Vector3 direction;
	direction.SphericalToCartesian(orientation, 0.f);

	Vector3 destination;
	destination.SphericalToCartesian(o, 0.f);

	orientation += dt * direction.Cross(destination).y * speed;
}

void NPC::Orientate(Vector3 t, double dt, float speed)
{
	Vector3 direction;
	direction.SphericalToCartesian(orientation, 0.f);

	Vector3 destination = Vector3(t - position).Normalized();

	orientation += dt * direction.Cross(destination).y * speed;
}

void NPC::Animate(double dt, float speed)
{
	if (velocity.x == 0 && velocity.z == 0) //resetting to original location
	{	
		for (int i = L_ARM; i < NUM_BODYPARTS; i++)
		{
			if(rotation[i] * rotationDir[i] > 0)
				rotationDir[i] = -rotationDir[i];
		
			else if(rotation[i] * rotationDir[i] < 0)
				rotationDir[i] = rotationDir[i];

			if (rotation[i] != 0)
				rotation[i] += (float)(rotationDir[i] * speed * dt);
			if (rotationDir[i] == -1 && rotation[i] <= 0)
				rotation[i] = 0;
			else if (rotationDir[i] == 1 && rotation[i] >= 0)
				rotation[i] = 0;
		}
	}

	else
	{
		for (int i = L_ARM; i < NUM_BODYPARTS; i++)
		{
			if(rotation[i] * rotationDir[i] > 30.f)
			{
				rotationDir[i] = -rotationDir[i];
			}
			rotation[i] += (float)(rotationDir[i] * speed * dt);
		}
	}
}

void NPC::Animate(double dt, float speed, float limit)
{
	if (velocity.x == 0 && velocity.z == 0) //resetting to original location
	{	
		for (int i = L_ARM; i < NUM_BODYPARTS; i++)
		{
			if(rotation[i] * rotationDir[i] > 0)
				rotationDir[i] = -rotationDir[i];
		
			else if(rotation[i] * rotationDir[i] < 0)
				rotationDir[i] = rotationDir[i];

			if (rotation[i] != 0)
				rotation[i] += (float)(rotationDir[i] * speed * dt);
			if (rotationDir[i] == -1 && rotation[i] <= 0)
				rotation[i] = 0;
			else if (rotationDir[i] == 1 && rotation[i] >= 0)
				rotation[i] = 0;
		}
	}

	else
	{
		for (int i = L_ARM; i < NUM_BODYPARTS; i++)
		{
			if(rotation[i] * rotationDir[i] > limit)
			{
				rotationDir[i] = -rotationDir[i];
			}
			rotation[i] += (float)(rotationDir[i] * speed * dt);
		}
	}
}

void dynamicObject::UpdateVelocity(double dt)
{
	if (velocity.z > 0)
	{
		velocity.z -= 80 * dt;
		if (velocity.z <= 0)
			velocity.z = 0;
	}
	else if (velocity.z < 0)
	{
		velocity.z += 80 * dt;
		if (velocity.z >= 0)
			velocity.z = 0;
	}

	if (velocity.x > 0)
	{
		velocity.x -= 80 * dt;
		if (velocity.x <= 0)
			velocity.x = 0;
	}
	else if (velocity.x < 0)
	{
		velocity.x += 80 * dt;
		if (velocity.x >= 0)
			velocity.x = 0;
	}

	velocity.y -= 80 * dt;

}

void NPC::Goto(Vector3 destination, double dt, float turn, float speed)
{
	Vector3 p = position; p.y = 0;

	Vector3 direction = Vector3(target - p).Normalized();
	Orientate(destination, dt, turn);

	if (hitDelay == 0)
	{
		velocity.x = direction.x * speed;
		velocity.z = direction.z * speed;

		if (p.Dist(target) <= speed/100.f)
		{
			position = target;
			velocity.x = 0;
			velocity.z = 0;
		}
	}

	if (previousPos != position)
	{
		previousPos = position;
		elapsedTime = 0.f;
	}
	else
	{
		elapsedTime += dt;

		if (elapsedTime > 0.3f)
			velocity.y = 25;
	}
}

void NPC::Update(double dt, vector<Object*>object, Player* player)
{
	initialPos = position;

	if (health > 0)
	{
		if (object[player->camera.lookAt] == this && Application::mouseButton(0) && !inConversation)
		{
			Vector3 dir;
			dir.SphericalToCartesian(player->hOrientation, 0.f);
			Vector3 vel(25,15,25);

			Knockback(dir, vel);
		}
		if (inConversation)
		{
			Orientate(player->position, dt, 500.f);
			Animate(dt, 150.f);
		}
		else
			Control(dt, object, player);
	}
	else
	{
		health = 0;
		Animate(dt, 150.f);
		ignoreCollision = true;
	}

	if (position != target)
		UpdateVelocity(dt);

	position += velocity * (float)dt; 

	if (position != target)
		RespondToCollision(initialPos, object, player);
	
	Material color;
	if (hitDelay > 0)
	{
		hitDelay -= dt;

		if (hitDelay > 0.2f)
		{
			color.kAmbient.Set(0.92f,0.34f,0.29f);
			color.kDiffuse.Set(0.92f,0.34f,0.29f);
		}
	}
	else 
		hitDelay = 0;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
		bodyParts[i].mesh->material = color;
	}
}

void Hobo::Init()
{
	identity = "Homeless Man";
	health = INT_MAX;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//hobo.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//hobo.txt");
	InitQuest("Filestream//Quests//hobo.txt");
}

void Hobo::Control(double dt, vector<Object*>object, Player* player)
{
	Orientate(180, dt, 150.f);
	target = position;
	velocity = 0;
}

void Thug::Init()
{
	identity = "Thug";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//Thug.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//thug.txt");
}

void Thug::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 120.f);

	if (position.Dist(player->position) < 100.f)
	{
		target = player->position; target.y = position.y;

		if (position.Dist(target) > 2.5f)
			Goto(target, dt, 1000.f, 6.f);
	}
}

void Cashier::Init()
{
	identity = "Cashier";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//Cashier.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//cashier.txt");
	InitQuest("Filestream//Quests//cashier.txt");
}

void Cashier::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 100.f);
	Vector3 p = position; p.y = 0;

	if (p == target)
	{
		Orientate(-90, dt, 200.f);
	}
	else if (p != target)
	{
		Goto(target, dt, 50.f, 5.f);
	}
}

void Blindman::Init()
{
	identity = "Ossan";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//blindman.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//blindman.txt");
}

void Blindman::Control(double dt, vector<Object*>object, Player* player)
{
	elapsedTime = 0;
	Animate(dt, 80.f);
	
	Vector3 direction;
	direction.SphericalToCartesian(orientation, 0.f);

	position += velocity * float(dt);

	Vector3 Cube = collision.hitbox/2; Cube += collision.centre;
	Vector3 maxPlayer = Cube + initialPos;
	Vector3 minPlayer = Cube - collision.hitbox;
	minPlayer.y = 0.4f; 

	minPlayer += initialPos;
	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (object[i] != this)
			if(!object[i]->ignoreCollision)
				if (Object::checkCollision(this, object[i]))
				{
					if (object[i]->type == "NPC" || object[i]->type == "Player")
					{
						if (object[i]->getHealth() > 0)
							object[i]->Knockback(direction, Vector3(80,30,80));
					}
					else if (hitDelay == 0)
					{
						Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
						Vector3 maxCube = Cube; maxCube += object[i]->position;
						Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

						if ((maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z) || (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z))
							target.z = position.z - direction.z;

						else if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x || maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
							target.x = position.x - direction.x;
					}
				}
	}

	position -= velocity * float(dt);


	Goto(target, dt, 500.f, 5.f);
	target += velocity * float(dt);
}

void Customer::Control(double dt, vector<Object*>object, Player* player)
{	
	Animate(dt, 100.f);
	Vector3 p = position; p.y = 0;

	if (p == target)
	{
		Orientate(tOrientation, dt, 200.f);
		elapsedTime += dt;

		if (elapsedTime >= decisionTime )
		{
			int r = rand() % targets.size();
			if (target != targets[r].position)
			{
				target = targets[r].position;
				tOrientation = targets[r].orientation;
				index = r;
			}
			elapsedTime = 0;
		}
	}
	else if (p != target)
	{
		Goto(target, dt, 100.f, 5.f);
	}
}

void Detective::Init()
{
	identity = "Detective-san";
	health = INT_MAX;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//detective.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//detective.txt");
	InitQuest("Filestream//Quests//detective.txt");
	
	quest->Accept.altNext->type = "Reward";
}

void Detective::Control(double dt, vector<Object*>object, Player* player)
{
	if (quest != NULL)
	{
		for (unsigned int i = 0; i < object.size(); i++)
		{
			if(object[i]->getIdentity() == "Ossan" && object[i]->getHealth() > 0)
				break;
			
			if (i == object.size() - 1)
				quest->criteria = true;
		}
	}

	Orientate(30, dt, 150.f);
	target = position;
	velocity = 0;
}

void S_Guard::Init()
{
	identity = "Security Guard";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//S_Guard.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//security.txt");
}

extern bool entrance;

void S_Guard::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 200.f + position.Dist(target) * 20, 60.f);
	if (target != original.position)
	{
		if (position.Dist(player->position) < 5.f)
		{
			Vector3 direction;
			direction.SphericalToCartesian(0.f, 0.f);
			player->Knockback(direction, Vector3(80,30,80));
		}
	}

	Vector3 p = position; p.y = 0;

	if (!player->inventory.checkPaid() && entrance)
	{
		target = player->position;
	}
	else
	{
		target = original.position;
	}

	if (p == target)
	{
		if (target == original.position)
			Orientate(0, dt, 200.f);
	}
	else if (p != target)
	{
		if (target == original.position)
			Goto(target, dt, 500.f, 10.f);
		else
			Goto(target, dt, 2000.f, 50.f);
	}
}

void Manager::Init()
{
	identity = "Manager-taichou";
	health = INT_MAX;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//manager.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}

	InitDialogue("Filestream//manager.txt");
	InitQuest("Filestream//Quests//manager.txt");
}

void Manager::Control(double dt, vector<Object*>object, Player* player)
{
	if (quest != NULL)
	{
		if (collection.Criteria(object))
			quest->criteria = true;
		else
			quest->criteria = false;

		cout << quest->criteria << endl;
	}

	Orientate(-145, dt, 150.f);
	target = position;
	velocity = 0;
}