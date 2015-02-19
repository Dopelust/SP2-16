#include "Camera3.h"
#include "PROJECTScene.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
	orientation = 0.f;
	look = 0.f;
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	orientation = -90.f;
	look = 0.f;
	throttle = 0.f;

	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right; right.SphericalToCartesian(orientation - 90.f, 0.0f);
	this->up = defaultUp = right.Cross(view);
}

static int throttleDir = 1;
static float throttleSpeed = 50.f;
void Camera3::Update(double dt, Player* player, vector<Object*>object)
{
	/*
	if (Application::IsKeyPressed(VK_SHIFT)) //resetting to original location
	{
		if(throttle * throttleDir > 2.f)
			throttleDir = -throttleDir;
		throttle += (float)(throttleDir * throttleSpeed * dt);
	}

	else
	{	
		if(throttle * throttleDir >= 0) //need to minus to reset
			throttleDir = -throttleDir;

		else if(throttle * throttleDir <= 0) //need to plus to reset
			throttleDir = throttleDir;

		if(throttleDir > 0)
		{
			throttle += (float)(throttleDir * throttleSpeed * dt);
			if (throttle > 0)
				throttle = 0;
		}

		if(throttleDir < 0)
		{
			throttle += (float)(throttleDir * throttleSpeed * dt);
			if (throttle < 0)
				throttle = 0;
		}
	}
	*/
	orientation = player->hOrientation;
	look = player->vOrientation;
	position = player->position;
	position.y += player->value[player->eyeLevel];
	Vector3 view; Vector3 right;
	view.SphericalToCartesian(orientation, look);
	right.SphericalToCartesian(orientation - 90.f, 0.0f);
	up = right.Cross(view);

	target = view*6 + position;
}

int Camera3::lookingAt(vector<Object*>object)
{
	Vector3 view = (target - position).Normalized(); view/=10;
	Object r(position + view*59.5, Vector3(0,0,0), Vector3(12,12,12));
	vector<int> store[2];

	for (unsigned int i = 0; i < object.size(); i++)
	{
		if (Object::checkCollision(object[i], &r))
		{
			int reach = 0;
			int maxReach = 0;

			if (object[i]->type == "NPC")
				maxReach = 120;
			else
				maxReach = 100;

			for (Vector3 p = position; reach < maxReach; p += view)
			{
				Object o(p, Vector3(0,0,0), Vector3(0.1f,0.1f,0.1f));
				if ( Object::checkCollision(object[i], &o) )
				{
					store[0].push_back(reach);
					store[1].push_back(i);
				}
				reach++;
			}
		}
	}

	int nearest = 120;
	int index = 0;

	for (unsigned int i = 0; i < store[0].size(); i++)
	{
		if (nearest > store[0][i])
		{
			nearest = store[0][i];
			index = store[1][i];
		}
	}

	return index;
}

void Camera3::Reset()
{
	orientation = -90.f;
	look = 0.f;

	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}