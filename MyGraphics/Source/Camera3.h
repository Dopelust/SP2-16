#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "collision.h"

#include <vector>

using namespace::std;

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float defaultOrientation;
	float defaultLook;

	float fov;
	float orientation;
	float look;
	int lookAt;

	int lookingAt(vector<Object*>object, int Reach);

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const float& orientation, const float& look);
	virtual void Update(double dt);
	void Move(double dt);
	void Zoom(double dt);
	virtual void Reset();
};

class Security
{
public:
	Security() {};
	Security(Object o1, Object o2) 
	{ 
		TV[0] = o1; TV[1] = o2;
		TV[0].type = "Control Panel";
		TV[1].type = "Control Panel";

		CCTVs[0].Init(Vector3(79,22,-85), -45, 0);
		CCTVs[1].Init(Vector3(79,22,40.5f), -135, 0);
		CCTVs[2].Init(Vector3(-69,22,30.5f), 135, 0);
		CCTVs[3].Init(Vector3(-69,22,-75.5f), 45, 0);
	}
	~Security() {};

	Object TV[2];
	Camera3 CCTVs[4];
};
#endif