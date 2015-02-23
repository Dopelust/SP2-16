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

float hoboHitDelay = 0.f;

void Hobo::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

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

	Control(dt, object, player);
	position += velocity * (float)dt; 
	RespondToCollision(initialPos, object, player);

	Material color;
	if (hoboHitDelay > 0)
	{
		hoboHitDelay -= dt;

		if (hoboHitDelay > 0.2f)
		{
			color.kAmbient.Set(0.92f,0.34f,0.29f);
			color.kDiffuse.Set(0.92f,0.34f,0.29f);
		}
	}
	else 
		hoboHitDelay = 0;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
		bodyParts[i].mesh->material = color;
	}
}

void Hobo::Control(double dt, vector<Object*>object, Player* player)
{
	if (object[player->camera.lookAt] == this && Application::mouseButton(0) && hoboHitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(player->hOrientation, 0.f);

		velocity.x += direction.x * 25;
		velocity.z += direction.z * 25;
		velocity.y += 15;
		hoboHitDelay = 0.5f;
	}
	else if (hoboHitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);

		velocity.x = direction.x * 5;
		velocity.z = direction.z * 5;
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

float thugHitDelay = 0;
void Thug::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

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

	Control(dt, object, player);
	position += velocity * (float)dt; 
	RespondToCollision(initialPos, object, player);

	Material color;
	if (thugHitDelay > 0)
	{
		thugHitDelay -= dt;

		if (thugHitDelay > 0.2f)
		{
			color.kAmbient.Set(0.92f,0.34f,0.29f);
			color.kDiffuse.Set(0.92f,0.34f,0.29f);
		}
	}
	else 
		thugHitDelay = 0;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
		bodyParts[i].mesh->material = color;
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

	if (object[player->camera.lookAt] == this && Application::mouseButton(0) && thugHitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(player->hOrientation, 0.f);

		velocity.x += direction.x * 25;
		velocity.z += direction.z * 25;
		velocity.y += 15;
		thugHitDelay = 0.5f;
	}
	else if (thugHitDelay == 0)
	{
		direction.SphericalToCartesian(orientation, 0.f);
		velocity.x = direction.x * 4.f;
		velocity.z = direction.z * 4.f; //state[WALK] = true;
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
}

float CashierHitDelay = 0.f;

void Cashier::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

	/*if (velocity.z > 0)
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
	}*/

	velocity.y -= 80 * dt;

	Control(dt, object, player);
	position += velocity * (float)dt; 
	RespondToCollision(initialPos, object, player);

	Material color;
	if (CashierHitDelay > 0)
	{
		CashierHitDelay -= dt;

		if (CashierHitDelay > 0.2f)
		{
			color.kAmbient.Set(0.92f,0.34f,0.29f);
			color.kDiffuse.Set(0.92f,0.34f,0.29f);
		}
	}
	else 
		CashierHitDelay = 0;

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{
		bodyParts[i].position = position;
		bodyParts[i].orientation = orientation;
		bodyParts[i].mesh->material = color;
	}
}

void Cashier::Control(double dt, vector<Object*>object, Player* player)
{
	if (object[player->camera.lookAt] == this && Application::mouseButton(0) && CashierHitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(player->hOrientation, 0.f);

		velocity.x += direction.x * 25;
		velocity.z += direction.z * 25;
		velocity.y += 15;
		CashierHitDelay = 0.5f;
	}
	else if (CashierHitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(orientation, 0.f);

		velocity.x = direction.x * 5;
		velocity.z = direction.z * 5;
	}
}

