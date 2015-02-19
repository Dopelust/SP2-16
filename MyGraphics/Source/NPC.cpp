#include "NPC.h"

void NPC::Init()
{
	Mesh* tempMesh;

	tempMesh = MeshBuilder::GenerateOBJ("Head", "OBJ//CharOBJ//Head.obj");
	bodyParts[HEAD] = Bodypart(position, Vector3(0,5.25f,0), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Body", "OBJ//CharOBJ//Body.obj");
	bodyParts[BODY] = Bodypart(position, Vector3(0,3.375f,0), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Left Arm", "OBJ//CharOBJ//LeftArm.obj");
	bodyParts[L_ARM] = Bodypart(position, Vector3(-1.125f,3.375f,0), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Right Arm", "OBJ//CharOBJ//RightArm.obj");
	bodyParts[R_ARM] = Bodypart(position, Vector3(1.125f,3.375f,0), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Left Leg", "OBJ//CharOBJ//LeftLeg.obj");
	bodyParts[L_LEG] = Bodypart(position, Vector3(-0.375f,1.125f,0), tempMesh);

	tempMesh = MeshBuilder::GenerateOBJ("Right Leg", "OBJ//CharOBJ//RightLeg.obj");
	bodyParts[R_LEG] = Bodypart(position, Vector3(0.375f,1.125f,0), tempMesh);

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].ignoreCollision = true;
	}
}

void Hobo::Init()
{
	identity = "Homeless Man";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//hobo.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}
}

void Hobo::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

	position.z += dt * 0.5f;
	Control(dt, object, player);
	RespondToCollision(initialPos, object, player);

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
	}
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
}

void Thug::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

	Control(dt, object, player);
	RespondToCollision(initialPos, object, player);

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
	}
}

void Thug::Control(double dt, vector<Object*>object, Player* player)
{
	Vector3 direction;
	direction.SphericalToCartesian(orientation, 0.f);

	Vector3 target = player->position; target.y = position.y;
	Vector3 destination = Vector3(target - position).Normalized();

	float Dot = direction.Dot(destination);
	float Mag = direction.Length() * destination.Length();

	if( direction.Cross(destination).y > 0 )
	{
		if (orientation + Math::RadianToDegree(acos(Dot/Mag)) > orientation)
		{
			orientation += dt * abs(direction.Cross(destination).y) * 1000;
		}
	}
	else if( direction.Cross(destination).y < 0 )
	{
		if (orientation + Math::RadianToDegree(-acos(Dot/Mag)) < orientation)
		{
			orientation -= dt * abs(direction.Cross(destination).y) * 1000;
		}
	}
	position += direction * 5 * float(dt); //state[WALK] = true;
}

void NPC::RespondToCollision(Vector3 initialPos, vector<Object*>object, Player* player)
{
	
	Vector3 Cube = collision.hitbox/2; Cube += collision.centre;
	Vector3 maxPlayer = Cube + initialPos;
	Vector3 minPlayer = Cube - collision.hitbox + initialPos;
	minPlayer.y += 0.5f;
	if (player->checkCollision(this))
	{
		Vector3 maxCube = player->collision.hitbox/2; maxCube.y = player->collision.hitbox.y; maxCube += player->position;
		Vector3 minCube = -player->collision.hitbox/2; 

		if (player->state[player->JUMP])
			minCube.y = 0.1f;
		else
			minCube.y = 0.5f;

		minCube += player->position;

		if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
			position.z = maxCube.z + collision.hitbox.z/2;

		if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
			position.z = minCube.z - collision.hitbox.z/2;

		if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
			position.x = maxCube.x + collision.hitbox.x/2;

		if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
			position.x = minCube.x - collision.hitbox.x/2;
	}

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (!object[i]->ignoreCollision)
			if (Object::checkCollision(this, object[i]))
			{
				Vector3 Cube = object[i]->collision.hitbox/2; Cube += object[i]->collision.centre;
				Vector3 maxCube = Cube; maxCube += object[i]->position;
				Vector3 minCube = Cube - object[i]->collision.hitbox; minCube += object[i]->position;

				if (maxPlayer.y >= maxCube.y && minPlayer.y >= maxCube.y)
					position.y = maxCube.y; 

				if (maxPlayer.z >= maxCube.z && minPlayer.z >= maxCube.z)
					position.z = maxCube.z + collision.hitbox.z/2;

				if (maxPlayer.z <= minCube.z && minPlayer.z <= minCube.z)
					position.z = minCube.z - collision.hitbox.z/2;

				if (maxPlayer.x >= maxCube.x && minPlayer.x >= maxCube.x)
					position.x = maxCube.x + collision.hitbox.x/2;

				if (maxPlayer.x <= minCube.x && minPlayer.x <= minCube.x)
					position.x = minCube.x - collision.hitbox.x/2;
			}
	}
}