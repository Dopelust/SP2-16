#include "NPC.h"

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

		if (i % 2 == 0)
			rotationDir[i] = 1.f;
		else
			rotationDir[i] = -1.f;
	}
}

void NPC::Animate(double dt, float speed)
{
	if (velocity.x == 0 && velocity.z == 0) //resetting to original location
	{	
		/*
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
			*/
	}

	else
	{
		for (int i = L_ARM; i < NUM_BODYPARTS; i++)
		{
			if(rotation[i] * rotationDir[i] > 25.f)
			{
				rotationDir[i] = -rotationDir[i];
			}
			rotation[i] += (float)(rotationDir[i] * speed * dt);
		}
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

	//Animate(dt);
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

void Hobo::Control(double dt, vector<Object*>object, Player* player)
{
	if (object[player->camera.lookAt] == this && Application::mouseButton(0) && hitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(player->hOrientation, 0.f);

		velocity.x += direction.x * 25;
		velocity.z += direction.z * 25;
		velocity.y += 15;
		hitDelay = 0.5f;
	}
	else if (hitDelay == 0)
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

	Animate(dt, 80.f);

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

	if (object[player->camera.lookAt] == this && Application::mouseButton(0) && hitDelay == 0)
	{
		Vector3 direction;
		direction.SphericalToCartesian(player->hOrientation, 0.f);

		velocity.x += direction.x * 25;
		velocity.z += direction.z * 25;
		velocity.y += 15;
		hitDelay = 0.5f;
	}
	else if (hitDelay == 0)
	{
		direction.SphericalToCartesian(orientation, 0.f);
		velocity.x = direction.x * 4.f;
		velocity.z = direction.z * 4.f; //state[WALK] = true;
	}
}

