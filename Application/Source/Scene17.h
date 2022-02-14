#ifndef SCENE_17_H
#define SCENE_17_H

#include "Scene.h"
#include "Camera3.h"
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

#include <string>

using namespace std;

class Scene17 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_FLOOR,
		GEO_QUAD,

		//day skybox		rainbow
		GEO_DAY_FRONT,
		GEO_DAY_BACK,
		GEO_DAY_LEFT,
		GEO_DAY_RIGHT,
		GEO_DAY_TOP,
		GEO_DAY_BOTTOM,

		//night skybox		sunsetflat
		GEO_NIGHT_FRONT,
		GEO_NIGHT_BACK,
		GEO_NIGHT_LEFT,
		GEO_NIGHT_RIGHT,
		GEO_NIGHT_TOP,
		GEO_NIGHT_BOTTOM,

		//OBJ with testure
		//survival
		GEO_CAMPFIRE,		//add logs to light on fire					campfire
		GEO_BEDROLL,		//rest to change skybox						bedroll
		GEO_TENT,			//bedroll in tent							tentclosed
		GEO_CART,
		GEO_CARTHIGH,
		GEO_TREE,		//chop tree to destroy
		GEO_AXE,
		GEO_CHEST,														//chest
		GEO_WORKBENCH,													//workbench
		GEO_ROCK,		//prolly boundaries

		//text
		GEO_TEXT,			//debug for text
		GEO_MOUSEPOS,		//debug for mouse
		GEO_MOUSESTATE,		//debug for mouse
		GEO_COORDS,			//debug for position
		GEO_INTERACT,		//prompts interactions
		GEO_TUTORIAL,		//static text
		GEO_TASKLIST,		//use array for actions
		GEO_DIALOGUE,		//render NPC dialogue in worldspace
		
		//old character		npc
		GEO_BODY,		//hip and chest, rectangle
		GEO_SPHERE,		//sphere
		GEO_CYLINDER,	//limbs

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

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	Scene17();
	~Scene17();

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

	Light light[3];	
	
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void RenderSkybox();
	void RenderBody(int x, int y, int z);


	bool flashlighttoggle = false;
	bool frametoggle = false;
	//update interactables
	bool interact;				//check if player is in range of interactable

	string time = "night";		//night/day		//change skybox

	//get axe from floor
	//cut tree
	//talk to npc to get flint and steel

	string tasklisttext[9] = { "Find out where you are. Talk to the NPC", "Take the axe from the chest", "Cut down the tallest tree", 
								"Fill the carts", "Return to NPC", "Light campfire", 
								"Return firestarter to NPC", "Rest in bed", ""};
	int activetask = 0;
	bool dialogue = false;
	string dialoguetext[10] = { "You're finally awake.", "I'm a woodcutter.", "I found you unconcious in the woods.", "Manpower is short here.", "You need to earn your keep here.", "Go and cut the tallest tree here to prove your worth.",
								"Its getting dark. Take this firestarter and start the campfire.", 
								"What a brilliant flame", "Go rest now, we have a long day tomorrow.", ""};
	int activedialogue = 0;
	bool dialoguetoggle = false;
	int randomtrees[100][2];
	//old character
	float rleftarmx = 0; float rleftarmy = 0; float rleftarmz = 20;
	float rrightarmx = 0; float rrightarmy = 0; float rrightarmz = -20;
	float rlefthandx = -45; float rlefthandy = 0; float rlefthandz = 0;
	float rrighthandx = -45; float rrighthandy = 0; float rrighthandz = 0;
	float rleftthighx = 0; float rleftthighy = 0; float rleftthighz = 5;
	float rrightthighx = 0; float rrightthighy = 0; float rrightthighz = -5;
	float rleftcalfx = 0; float rleftcalfy = 0; float rleftcalfz = 0;
	float rrightcalfx = 0; float rrightcalfy = 0; float rrightcalfz = 0;
	float rhipx = 0;	//0/5, bend forward
	float rhipy = 0;	//swing axe
	float thip = 1.5;
	int direction = 1;



	//mouse input
	double x, y;
	float posX, posY;
	string mousestate;
	
};


#endif