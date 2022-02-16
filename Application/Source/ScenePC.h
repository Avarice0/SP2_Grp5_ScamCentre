#ifndef SCENE_PC_H
#define SCENE_PC_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Utility.h"

#include "LoadTGA.h"
#include "Utility.h"
#include <iostream>

#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


using namespace std;

class ScenePC : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_FLOOR,
		GEO_QUAD,

		GEO_COIN,
		GEO_WALLPAPER,
		GEO_PICKAXE,
	
		//text
		GEO_TEXT,			//debug for text
		GEO_MOUSEPOS,		//debug for mouse
		GEO_MOUSESTATE,		//debug for mouse
		GEO_SCORE,			//number of coins clicked
		GEO_MINING,			//Instructions
		
		GEO_LINE,
		GEO_EXIT,
		GEO_GREENTEXT,


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

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	ScenePC();
	~ScenePC();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	//mouse input
	double x, y;
	float posX, posY;
	string mousestate;

	string times;
	int day;


	int gamenum = 2;

	int coinx = rand() % 25 + 45;
	int coiny = rand() % 50 + 10;

	int score = 0;
	int minescore = 0;
	int textscore = 0;

	int seconds = 5; //coin game timer
	bool coinStarted = false;

	int miningScale = 25;

	int totalframe = 0;

	std::string victimMsg[8][2] = 
	{
		{"What company are", "you calling from?"},
		{"Are you a scammer?", "  "},
		{"How do I stop the" ,"police from coming???"},
		{"How do I receive ","the product refund?"},
		{"How 2 receive this ","lottery prize? lol"},
		{"how to get FREE"," discerds nitre"},
		{"How do I stop my acc","from being suspended"},
		{"Give me back my son!", "  "}
	};

	std::string correctAns[8]
	{
		"Amazon",
		"No",
		"Give credit card no.",
		"Give credit card no.",
		"give a deposit",
		"Give account details",
		"pay up",
		"hand over the mooney",
	};

	std::string wrongAns[8]
	{
		"amazoone",
		"Yes",
		"you can't",
		"go to our office",
		"It was a prank",
		"buy it yourself",
		"Nothing",
		"no",
	};

	int RNGmsg = 5;
	bool correctPos = rand() % 2;
};



#endif