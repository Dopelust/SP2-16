#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"
#include "Camera.h"
#include "Camera3.h"
#include "Utility.h"
#include "MatrixStack.h"
#include "LoadTGA.h"
#include "collision.h"
#include "player.h"
#include "NPC.h"
#include "dialogue.h"
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

class OnScreenText
{
public:
	OnScreenText(std::string n, Vector3 p) 
	{
		name = n;
		textPos = p;
		elapsedTime = 0;
		fall = false;
	};
	OnScreenText(std::string n, Vector3 p, bool f) 
	{
		name = n;
		textPos = p;
		elapsedTime = 0;
		fall = f;
	};
	~OnScreenText() {};

	std::string name;
	Vector3 textPos;
	float elapsedTime;
	bool fall;

	void Update(double dt);
};

class PROJECTScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_KNIFE,
		GEO_HEART,
		GEO_SPRINT,
		GEO_JUMP,
		GEO_TEXT,
		GEO_LINE,
		GEO_CUBE,
		GEO_BIGCUBE,
		GEO_HOLD,
		GEO_CCTV,
		GEO_CCTV_CORNER,
		GEO_TEXTBOX,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_LIGHT0_POSITION, U_LIGHT0_COLOR, U_LIGHT0_POWER, U_LIGHT0_KC, U_LIGHT0_KL, U_LIGHT0_KQ,
		U_LIGHT0_TYPE, U_LIGHT0_SPOTDIRECTION, U_LIGHT0_COSCUTOFF, U_LIGHT0_COSINNER, U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION, U_LIGHT1_COLOR, U_LIGHT1_POWER, U_LIGHT1_KC, U_LIGHT1_KL, U_LIGHT1_KQ,
		U_LIGHT1_TYPE, U_LIGHT1_SPOTDIRECTION, U_LIGHT1_COSCUTOFF, U_LIGHT1_COSINNER, U_LIGHT1_EXPONENT,
		U_LIGHT2_POSITION, U_LIGHT2_COLOR, U_LIGHT2_POWER, U_LIGHT2_KC, U_LIGHT2_KL, U_LIGHT2_KQ,
		U_LIGHT2_TYPE, U_LIGHT2_SPOTDIRECTION, U_LIGHT2_COSCUTOFF, U_LIGHT2_COSINNER, U_LIGHT2_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	void RenderSkybox();
	void RenderCrosshair();
	void InitSuperMarket();

public:
	PROJECTScene();
	~PROJECTScene();

	virtual void Init();
	void InitJunk();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RicssonInit();
	void JeremiahInit();
	void JessicaInit();
	void DarrenInit();

	int soundInit();
	int soundUpdate(Player player);

	static float inputDelay;
private:
	unsigned m_vertexArrayID;
	Mesh * meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	TextBox * textbox;
	vector<OnScreenText*> text;
	vector<OnScreenText*> text2D;
	vector<NPC*> character;
	vector<Object*> object;
	vector<Particles*> blood;

	Doorway doorway;
	Doorway doorway2;
	Doorway AutoDoor;
	ATM Bank;
	Vending Machine;
	Security controlPanel;

	Light light[3];
	void RenderScene();
	void RenderCCTVUI(int number);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	float getTextWidth(string t);

	Camera3* camera;
	Player player;

	MS modelStack, viewStack, projectionStack;
};

#endif
