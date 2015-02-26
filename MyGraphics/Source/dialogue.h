#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

#include <string>

using namespace::std;

class Button2D
{
public:
	Button2D() {};
	Button2D(Vector3 pos) {position = pos;}
	~Button2D() {};

	Mesh * mesh;
	Mesh * hoverMesh;
	Vector3 position;
	string text;
};

class TextBox
{
public:
	TextBox() {};
	TextBox(string t, Mesh * m) 
	{
		mesh = m;
		text = t;
	};
	~TextBox() {};

	Mesh * mesh;
	static Vector3 position;
	vector<Button2D> button;
	string text;
};

#endif