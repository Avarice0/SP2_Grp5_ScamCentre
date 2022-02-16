#ifndef SCENE_17_H
#define SCENE_17_H

#include "Scene.h"
#include "Camera3.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "PoliceMeter.h"

#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Utility.h"

#include "LoadTGA.h"
#include "Utility.h"

#include <string>

#include "entity.h"

using namespace std;

class SceneGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_FLOOR,
		GEO_QUAD,

		//text
		GEO_TEXT,			//debug for text
		GEO_MOUSEPOS,		//debug for mouse
		GEO_MOUSESTATE,		//debug for mouse
		GEO_COORDS,			//debug for position
		GEO_TIME,
		GEO_DOLLARS,
		GEO_PROFIT,

		//worker body
		GEO_SKINNED,
		GEO_SHIRT1,
		GEO_SHIRT2,
		GEO_SHIRT3,
		GEO_PANTS,

		//workstation objs
		GEO_MAIL,
		GEO_PHONE,
		GEO_LAPTOP,
		GEO_TOWER,

		// for the room
		GEO_TABLE,
		GEO_ROOM,
		GEO_FLOORTILES,
		GEO_OFFICE,
		GEO_UPGRADEAREA,

		GEO_UPGRADESHOPBG,
		GEO_UPGRADESHOPFG,
		GEO_UPGRADEITEM1,
		GEO_UPGRADEITEM2,
		GEO_LOCK,
		GEO_LOCKEDFG,
		GEO_WORKERUPGRADE,
		GEO_COMPUTERUPGRADE,
		GEO_PUPGRADE,
		GEO_WUPGRADE,

		//HUD
		GEO_METREBARBG,
		GEO_METREBARFG,
		GEO_METREBARBGBG,
		GEO_METREBARBULB,

		//objs

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

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack, viewStack, projectionStack;

	Light light[1];	

	PoliceMeter metre;
	
	static float GetMouseX();
	static float GetMouseY();

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	//automated render functions
	void renderworker(int x, int y, int z, int rarity);		//call entities list
	void RenderRoom(void);
	void RenderTable(int x, int y, int z);
	void RenderPermUpgrade();
	void RenderUpgrade();
	void RenderPoliceMetre();

	//mouse input
	double x, y;
	float posX, posY;
	string mousestate;
	int NoobCount = 5, ExperiencedCount = 0, ExpertCount = 0;

	//time system
	// gay
	int day = 0;
	int totalframe = 0;
	string time;

	//entities
	entity* entities[20];
	float dollars = 0;
	float policemeter = 0;		//0-100% 
	int NoobCount = 0, ExperiencedCount = 0, ExpertCount = 0;

	//Upgrade
	bool coffee = false, policedeter = false, RenderPermItem1 = false, RenderPermItem2 = false;
	bool workerupgrade = false, stationupgrade = false;

	bool playerMoving = false;
	bool legBack = false;
	float debugRot;

	Mesh* shirtrarity[3];			//increases with number of shirts
};


#endif
