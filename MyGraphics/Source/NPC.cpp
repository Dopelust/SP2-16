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

	float Dot = direction.Dot(destination);
	float Mag = direction.Length() * destination.Length();

	orientation += dt * direction.Cross(destination).y * speed;
}

void NPC::Orientate(Vector3 t, double dt, float speed)
{
	Vector3 direction;
	direction.SphericalToCartesian(orientation, 0.f);

	Vector3 destination = Vector3(t - position).Normalized();

	float Dot = direction.Dot(destination);
	float Mag = direction.Length() * destination.Length();

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
			if(rotation[i] * rotationDir[i] > 25.f)
			{
				rotationDir[i] = -rotationDir[i];
			}
			rotation[i] += (float)(rotationDir[i] * speed * dt);
		}
	}
}

void NPC::UpdateVelocity(double dt)
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

void NPC::Update(double dt, vector<Object*>object, Player* player)
{
	Vector3 initialPos = position;

	Control(dt, object, player);
	UpdateVelocity(dt);

	position += velocity * (float)dt; 
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

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//hobo.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}
}

void Hobo::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 25.f);

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

void Thug::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 80.f);

	Vector3 target = player->position; target.y = position.y;
	Orientate(target, dt, 1000.f);

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

void Cashier::Control(double dt, vector<Object*>object, Player* player)
{
	Vector3 p = position; p.y = 0;

	if (p == target)
	{
		Animate(dt, 100.f);
		Orientate(-90, dt, 200.f);
	}
	else if (p != target)
	{
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

		Vector3 destination = Vector3(target - p).Normalized();
	
		Animate(dt, 50.f);
		Orientate(target, dt, 100.f);

		if (hitDelay == 0)
		{
			velocity.x = destination.x * 5;
			velocity.z = destination.z * 5;

			if (p.Dist(target) <= 0.1f)
			{
				position = target;
				velocity.x = 0;
				velocity.z = 0;
			}
		}
	}

	if (object[player->camera.lookAt] == this)
	{
		if (Application::mouseButton(0) && hitDelay == 0)
		{
			Vector3 direction;
			direction.SphericalToCartesian(player->hOrientation, 0.f);

			velocity.x += direction.x * 25;
			velocity.z += direction.z * 25;
			velocity.y += 15;

			hitDelay = 0.5f;
		}

		else if (Application::IsKeyPressed('E') && position == target && hitDelay == 0)
		{
			player->inventory.Checkout();
		}
	}
}

void Blindman::Init()
{
	identity = "Homeless Man";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//Blindman.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}
}

void Blindman::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 25.f);

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

void Customer::Init()
{
	identity = "Homeless Man";

	for (int i = 0; i < NUM_BODYPARTS; i++)
	{	
		bodyParts[i].mesh->textureID = LoadTGA("Image//CharTGA//Customer.tga");
		bodyParts[i].position = position;
		bodyParts[i].identity = identity;
	}
}

void Customer::Control(double dt, vector<Object*>object, Player* player)
{
	Animate(dt, 25.f);

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
