#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "collision.h"
#include "player.h"

#include <vector>

using namespace::std;

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float throttle;
	float orientation;
	float look;
	int lookAt;

	int lookingAt(vector<Object*>object);

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, Player* player, vector<Object*>object);
	virtual void Reset();
};

#endif