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

	int lookingAt(vector<Object*>object);

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const float& orientation, const float& look);
	virtual void Update(double dt);
	void Move(double dt);
	void Zoom(double dt);
	virtual void Reset();
};

#endif