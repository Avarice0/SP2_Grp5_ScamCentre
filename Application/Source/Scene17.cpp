#include "Scene17.h"

Scene17::Scene17()
{
}

Scene17::~Scene17()
{
}

void Scene17::Init()
{
	{
		// Set background color to dark blue
		glClearColor(0.1f, 0.0f, 0.4f, 0.0f);

		//Enable depth buffer and depth testing
		glEnable(GL_DEPTH_TEST);

		//Enable back face culling
		glEnable(GL_CULL_FACE);

		//Default to fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	{
		//Load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

		m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
		m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
		m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
		m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
		m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
		m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
		m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
		m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
		m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
		m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
		m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		// Get a handle for our "textColor" uniform
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

		// Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		// Use our shader
		glUseProgram(m_programID);
		glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	}

	//Initialize camera settings
	camera.Init(Vector3(10, 10, 10), Vector3(0, 10, 10), Vector3(0, 1, 0));

	// Init VBO
	{
		light[0].type = Light::LIGHT_SPOT;
		light[0].position.Set(camera.position.x, camera.position.y, camera.position.z);
		light[0].color.Set(1, 1, 1);
		light[0].power = 1.0f;
		light[0].kC = 1.f;
		light[0].kL = 0.01;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(12.5));
		light[0].cosInner = cos(Math::DegreeToRadian(10));
		light[0].exponent = 1.f;
		light[0].spotDirection.Set(camera.target.x, camera.target.y, camera.target.z);

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	}
	{
		light[1].type = Light::LIGHT_POINT;
		light[1].position.Set(0, 0, 20);
		light[1].color.Set(1, 1, 0.1);
		light[1].power = .0f;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = cos(Math::DegreeToRadian(45));
		light[1].cosInner = cos(Math::DegreeToRadian(30));
		light[1].exponent = 3.f;
		light[1].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	}
	{
		light[2].type = Light::LIGHT_DIRECTIONAL;
		light[2].position.Set(0, 1, 0);
		light[2].color.Set(1, 1, 1);
		light[2].power = 0.25f;
		light[2].kC = 1.f;
		light[2].kL = 0.01f;
		light[2].kQ = 0.001f;
		light[2].cosCutoff = cos(Math::DegreeToRadian(45));
		light[2].cosInner = cos(Math::DegreeToRadian(30));
		light[2].exponent = 3.f;
		light[2].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
	}
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{
		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
		meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.25, 0.75, 0.25), 1.f);
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	}
	{
		meshList[GEO_DAY_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_FRONT]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_ft.tga");
		meshList[GEO_DAY_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_BACK]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_bk.tga");
		meshList[GEO_DAY_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_LEFT]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_lf.tga");
		meshList[GEO_DAY_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_RIGHT]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_rt.tga");
		meshList[GEO_DAY_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_TOP]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_up.tga");
		meshList[GEO_DAY_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[GEO_DAY_BOTTOM]->textureID = LoadTGA("OBJ//skyboxes//rainbow//rainbow_dn.tga");

		meshList[GEO_NIGHT_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_FRONT]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_ft.tga");
		meshList[GEO_NIGHT_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_BACK]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_bk.tga");
		meshList[GEO_NIGHT_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_LEFT]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_lf.tga");
		meshList[GEO_NIGHT_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_RIGHT]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_rt.tga");
		meshList[GEO_NIGHT_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_TOP]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_up.tga");
		meshList[GEO_NIGHT_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[GEO_NIGHT_BOTTOM]->textureID = LoadTGA("OBJ//skyboxes//sunsetflat//sunsetflat_dn.tga");
	}
	{
		meshList[GEO_BODY] = MeshBuilder::GenerateCube("cube", Color(1, 0.7, 0.8), 1);
		meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", Color(1, 0.7, 0.8), 20, 2.5, 1);
		meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(1, 0.7, 0.8), 20, 20, 1);
	}
	{
		meshList[GEO_CAMPFIRE] = MeshBuilder::GenerateOBJMTL("campfire", "OBJ//survivalkit//campfire.obj", "OBJ//survivalkit//campfire.mtl");
		meshList[GEO_BEDROLL] = MeshBuilder::GenerateOBJMTL("bedroll", "OBJ//survivalkit//bedroll.obj", "OBJ//survivalkit//bedroll.mtl");
		meshList[GEO_TENT] = MeshBuilder::GenerateOBJMTL("tent", "OBJ//survivalkit//tentClosed.obj", "OBJ//survivalkit//tentClosed.mtl");
		meshList[GEO_CART] = MeshBuilder::GenerateOBJMTL("cart", "OBJ//survivalkit//cart.obj", "OBJ//survivalkit//cart.mtl");
		meshList[GEO_CARTHIGH] = MeshBuilder::GenerateOBJMTL("fullcart", "OBJ//survivalkit//cartHigh.obj", "OBJ//survivalkit//cartHigh.mtl");
		meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("tree", "OBJ//survivalkit//tree.obj", "OBJ//survivalkit//tree.mtl");
		meshList[GEO_AXE] = MeshBuilder::GenerateOBJMTL("axe", "OBJ//survivalkit//toolAxe.obj", "OBJ//survivalkit//toolAxe.mtl");
		meshList[GEO_CHEST] = MeshBuilder::GenerateOBJMTL("chest", "OBJ//survivalkit//chest.obj", "OBJ//survivalkit//chest.mtl");
		meshList[GEO_WORKBENCH] = MeshBuilder::GenerateOBJMTL("workbench", "OBJ//survivalkit//workbench.obj", "OBJ//survivalkit//workbench.mtl");
		meshList[GEO_ROCK] = MeshBuilder::GenerateOBJMTL("rockboundary", "OBJ//survivalkit//resourceStone.obj", "OBJ//survivalkit//resourceStone.mtl");
	}
	{
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSEPOS] = MeshBuilder::GenerateText("mousepos", 16, 16);
		meshList[GEO_MOUSEPOS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSESTATE] = MeshBuilder::GenerateText("mousestate", 16, 16);
		meshList[GEO_MOUSESTATE]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_COORDS] = MeshBuilder::GenerateText("coordinates", 16, 16);
		meshList[GEO_COORDS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_INTERACT] = MeshBuilder::GenerateText("interact", 16, 16);
		meshList[GEO_INTERACT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TUTORIAL] = MeshBuilder::GenerateText("tutorial", 16, 16);
		meshList[GEO_TUTORIAL]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TASKLIST] = MeshBuilder::GenerateText("taskliat", 16, 16);
		meshList[GEO_TASKLIST]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_DIALOGUE] = MeshBuilder::GenerateText("dialogue", 16, 16);
		meshList[GEO_DIALOGUE]->textureID = LoadTGA("Image//calibri.tga");
	}
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	for (int i = 0; i < 100; i++) {
		int randx = rand() % 500 - 250;
		int randz = rand() % 500 - 250;
		while ((randx<75 && randx>-75) && (randz<75 && randz>-75)) {
			randx = rand() % 500 - 250;
			randz = rand() % 500 - 250;
		}
		randomtrees[i][0] = randx;
		randomtrees[i][1] = randz;
	}
}

void Scene17::Update(double dt)
{
	camera.Update(dt);

	light[0].position.x = camera.position.x;
	light[0].position.y = camera.position.y;
	light[0].position.z = camera.position.z;
	light[0].spotDirection.Set(camera.position.x-camera.target.x, camera.position.y - camera.target.y, camera.position.z - camera.target.z);
	
	if (flashlighttoggle == false && Application::IsKeyPressed('Q')) {
		flashlighttoggle = true;							//flashlight toggle
		if (light[0].power == 1.f) {
			light[0].power = 0.f;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
		else if (light[0].power == 0.f) {
			light[0].power = 1.f;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
	}
	else if (flashlighttoggle == true && !Application::IsKeyPressed('Q')) {
		flashlighttoggle = false;
	}
	else {}

	//talk to npc, receive task
	//pick up axe 
	//cut tree
	//fill cart
	//talk to npc for firestarter
	//light fire
	//return to npc
	//rest
	
	if (((camera.position.x < 5) && (camera.position.x > -5)) && ((camera.position.z < 5) && (camera.position.z > -5))) {		//npc location 0,-20
		interact = true;
		if (dialoguetoggle == false && Application::IsKeyPressed('E')) {
			dialoguetoggle = true;
			if (activetask == 0) {
				dialogue = true;
				activedialogue++;
				if (activedialogue == 5) {		//go to pickup axe
					activetask++;
				}
			}
			if (activetask == 4) {								//filled cart, get fire starter
				dialogue = true;
				activedialogue++;
				if (activedialogue == 6) {
					activetask++;
				}
			}
			if (activetask == 6) {								//lit fire, go bed
				dialogue = true;
				activedialogue++;
				if (activedialogue == 8) {
					activetask++;
				}
			}
		}
		else if (dialoguetoggle == true && !Application::IsKeyPressed('E')) {
			dialoguetoggle = false;
		}
	}
	else if (((camera.position.x < 55) && (camera.position.x > 45)) && ((camera.position.z < -5) && (camera.position.z > -15)) && activetask == 1) {		//axe
		interact = true;
		if (Application::IsKeyPressed('E')) {
			activetask++;
		}
	}
	else if (((camera.position.x < 165) && (camera.position.x > 135)) && ((camera.position.z < 15) && (camera.position.z > -15)) && activetask == 2) {		//tree
		interact = true;
		if (Application::IsKeyPressed('E')) {
			activetask++;
		}
	}
	else if (((camera.position.x < -25) && (camera.position.x > -35)) && ((camera.position.z < -20) && (camera.position.z > -30)) && activetask == 3) {		//cart
		interact = true;
		if (Application::IsKeyPressed('E')) {
			activetask++;
		}
	}
	else if (((camera.position.x < 5) && (camera.position.x > -5)) && ((camera.position.z < 25) && (camera.position.z > 15)) && activetask == 5) {		//campfire
		interact = true;
		if (Application::IsKeyPressed('E')) {
			light[1].power = 1.0f;
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
			activetask++;
		}
	}
	else if (((camera.position.x < 5) && (camera.position.x > -5)) && ((camera.position.z < 40) && (camera.position.z > 30)) && activetask == 7) {		//bed location
		interact = true;
		if (Application::IsKeyPressed('E')) {
			time = "day";
			light[2].power = 1.0f;
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
			activetask++;
			activedialogue++;
		}
	}
	else {
		interact = false;
	}

	{
		if (rrightarmx <= 0) {
			rrightarmx += 70 * dt;
			rrighthandx += 30 * dt;
		}
		if (thip <= 1.3) direction *= -1;
		if (thip >= 1.7) direction *= -1;

		thip += 0.5 * direction * dt;
		rleftarmx += 20 * direction * dt;
		rrightarmx -= 20 * direction * dt;
	}


	//mouse inputs
	Application::GetCursorPos(&x, &y);
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	posX = x / w * 80; //convert (0,800) to (0,80)
	posY = 60 - y / h * 60; //convert (600,0) to (0,60)
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		mousestate = "LBUTTON DOWN";
		//converting viewport space to UI space
		if ((posX > 30 && posX < 50) && (posY > 25 && posY < 35))
		{
			//trigger user action or function
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		mousestate = "";
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		mousestate = "RBUTTON DOWN";
		if ((posX > 30 && posX < 50) && (posY > 25 && posY < 35))
		{
			//trigger user action or function
		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		mousestate = "";
	}
}

void Scene17::Render()
{
	{
		// Render VBO here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Temp variables
		Mtx44 translate, rotate, scale;
		Mtx44 MVP;

		//These will be replaced by matrix stack soon
		Mtx44 model, view, projection;
		//Set view matrix using camera settings
		view.SetToLookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z);
		modelStack.LoadIdentity();
	}
	{			//spot light
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	{			//point light
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	//----------------------------------------
	{
		RenderSkybox();
		//RenderMesh(meshList[GEO_AXES], false);
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_FLOOR], true);
		modelStack.PopMatrix();

		RenderBody(0, 6, -20);
	}
	
	{
		modelStack.PushMatrix();			//interact
		modelStack.Translate(0, 0, 20); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_CAMPFIRE], true);

			modelStack.PushMatrix();		//interact
			modelStack.Translate(0, 0, 5); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_BEDROLL], true);
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 0); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_TENT], true);
				modelStack.PopMatrix();
			modelStack.PopMatrix();
			{
				modelStack.PushMatrix();
				modelStack.Translate(4, 0, 4); modelStack.Rotate(-45, 0, 1, 0); modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_TENT], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(5, 0, 0); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_TENT], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-4, 0, 4); modelStack.Rotate(45, 0, 1, 0); modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_TENT], true);
				modelStack.PopMatrix();
			}
		modelStack.PopMatrix();
	}

	{
		//big tree
		if (activetask < 3) {
			modelStack.PushMatrix();				//interact
			modelStack.Translate(150, 0, 0); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(25, 25, 25);
			RenderMesh(meshList[GEO_TREE], true);
			modelStack.PopMatrix();
		}
		
		//small trees 
		for (int i = 0; i < 100; i++) {
			modelStack.PushMatrix();				//interact
			modelStack.Translate(randomtrees[i][0],0,randomtrees[i][1]); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_TREE], true);
			modelStack.PopMatrix();
		}
	}

	if (activetask == 4) {
		//render the cart as cartHigh
		modelStack.PushMatrix();				//interact
		modelStack.Translate(-50, 0, -25); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(12.5, 12.5, 12.5);
		RenderMesh(meshList[GEO_CARTHIGH], true);
		modelStack.PopMatrix();
	}
	else {
		//render cart
		modelStack.PushMatrix();				//interact
		modelStack.Translate(-50, 0, -25); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(12.5, 12.5, 12.5);
		RenderMesh(meshList[GEO_CART], true);
		modelStack.PopMatrix();
	}
	
	{
		//workbench+chest
		modelStack.PushMatrix();				//interact
		modelStack.Translate(40, 0, -20); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_WORKBENCH], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();				//interact
		modelStack.Translate(50, 0, -20); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_CHEST], true);
		modelStack.PopMatrix();
	}

	//rock boundary
	for (int i = 0; i < 22; i++) {
		modelStack.PushMatrix();
		modelStack.Translate(-275, 0, -275 + (i * 25)); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_ROCK], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(275, 0, -275 + (i * 25)); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_ROCK], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-275 + (i * 25), 0, -275); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_ROCK], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-275 + (i * 25), 0, 275); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_ROCK], true);
		modelStack.PopMatrix();
	}

	//text render
	{
		if (dialogue == true) {
			modelStack.PushMatrix();
			modelStack.Translate(-5, 15, -20); modelStack.Rotate(0, 1, 0, 0); modelStack.Scale(0.5, 1, 0.5);
			RenderText(meshList[GEO_DIALOGUE], dialoguetext[activedialogue], Color(1, 1, 1));
			modelStack.PopMatrix();
		}

		string coord = to_string(camera.position.x) + ", " + to_string(camera.position.y) + ", " + to_string(camera.position.z);
		RenderTextOnScreen(meshList[GEO_COORDS], coord, Color(0.5, 0.5, 1), 2, 0, 0.5);
		if (interact == true) {
			RenderTextOnScreen(meshList[GEO_INTERACT], "Press 'E' to Interact", Color(0.5, 0.5, 1), 3, 20, 25);
		}

		RenderTextOnScreen(meshList[GEO_TUTORIAL], "ACTIVE TASKS: ", Color(0.5, 0.5, 1), 2, 1, 50);
		RenderTextOnScreen(meshList[GEO_TASKLIST], tasklisttext[activetask], Color(0.5, 0.5, 1), 2, 2, 48);
	}

	//render mesh on screen
	RenderMeshOnScreen(meshList[GEO_QUAD], 40, 30, 20, 10);

	//UI buttons test
	string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
	RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 3.5);
	//---------------------------------------------------------
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
}

void Scene17::RenderSkybox(void) {
	if (time == "day") {
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 495, camera.position.y, camera.position.z + 0); modelStack.Rotate(-90, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_FRONT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x - 495, camera.position.y, camera.position.z + 0); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_BACK], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y, camera.position.z + 495); modelStack.Rotate(180, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_LEFT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y, camera.position.z - 495); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_RIGHT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y + 500, camera.position.z + 0); modelStack.Rotate(90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_TOP], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y - 500, camera.position.z + 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_DAY_BOTTOM], false);
		modelStack.PopMatrix();
	}
	else if (time == "night") {
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 495, camera.position.y, camera.position.z + 0); modelStack.Rotate(-90, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_FRONT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x - 495, camera.position.y, camera.position.z + 0); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_BACK], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y, camera.position.z + 495); modelStack.Rotate(180, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_LEFT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y, camera.position.z - 495); modelStack.Rotate(0, 0, 1, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_RIGHT], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y + 500, camera.position.z + 0); modelStack.Rotate(90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_TOP], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + 0, camera.position.y - 500, camera.position.z + 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_NIGHT_BOTTOM], false);
		modelStack.PopMatrix();
	}
}

void Scene17::RenderBody(int x, int y, int z) {
	modelStack.PushMatrix();		//hip
	modelStack.Translate(x, y, z); modelStack.Scale(3.1, 1, 2.1);
	modelStack.Rotate(0, 1, 0, 0); modelStack.Rotate(0, 0, 1, 0); modelStack.Rotate(0, 0, 0, 1);
	RenderMesh(meshList[GEO_BODY], true);

		modelStack.PushMatrix();
		modelStack.Translate(0.25, 0, 0); modelStack.Scale(0.33, 1.3, 0.5);
		modelStack.Rotate(rleftthighx, 1, 0, 0); modelStack.Rotate(rleftthighy, 0, 1, 0); modelStack.Rotate(rleftthighz, 0, 0, 1);
		RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PushMatrix();		//left thigh
			modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CYLINDER], true);
				modelStack.PushMatrix();
				modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
				modelStack.Rotate(rleftcalfx, 1, 0, 0); modelStack.Rotate(rleftcalfy, 0, 1, 0); modelStack.Rotate(rleftcalfz, 0, 0, 1);
				RenderMesh(meshList[GEO_SPHERE], true);
					modelStack.PushMatrix();		//left calf
					modelStack.Translate(0, -1.2, 0); modelStack.Scale(0.9, 1.1, 0.9);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-0.25, 0, 0); modelStack.Scale(0.33, 1.3, 0.5);
		modelStack.Rotate(rrightthighx, 1, 0, 0); modelStack.Rotate(rrightthighy, 0, 1, 0); modelStack.Rotate(rrightthighz, 0, 0, 1);
		RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PushMatrix();		//right thigh
			modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CYLINDER], true);
				modelStack.PushMatrix();
				modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
				modelStack.Rotate(rrightcalfx, 1, 0, 0); modelStack.Rotate(rrightcalfy, 0, 1, 0); modelStack.Rotate(rrightcalfz, 0, 0, 1);
				RenderMesh(meshList[GEO_SPHERE], true);
					modelStack.PushMatrix();		//right calf
					modelStack.Translate(0, -1.2, 0); modelStack.Scale(0.9, 1.1, 0.9);
					RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();		//hip joint
		modelStack.Translate(0, thip, 0); modelStack.Scale(0.1, 0.1, 0.1);
		modelStack.Rotate(rhipx, 1, 0, 0); modelStack.Rotate(rhipy, 0, 1, 0); modelStack.Rotate(0, 0, 0, 1);
		RenderMesh(meshList[GEO_SPHERE], true);

			modelStack.PushMatrix();		//chest
			modelStack.Translate(0, 2.5, 0); modelStack.Scale(10, 40, 10);
			RenderMesh(meshList[GEO_BODY], true);

				modelStack.PushMatrix();		//backpack
				modelStack.Translate(0, 0, -0.9); modelStack.Scale(0.9, 0.9, 0.9);
				RenderMesh(meshList[GEO_BODY], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();		//neck
				modelStack.Translate(0, 0.65, 0); modelStack.Scale(0.15, 0.15, 0.25);
				modelStack.Rotate(0, 1, 0, 0); modelStack.Rotate(0, 0, 1, 0); modelStack.Rotate(0, 0, 0, 1);
				RenderMesh(meshList[GEO_CYLINDER], true);
					modelStack.PushMatrix();		//head
					modelStack.Translate(0, 1.5, 0); modelStack.Scale(2.2, 1.7, 2);
					modelStack.Rotate(0, 1, 0, 0); modelStack.Rotate(0, 0, 1, 0); modelStack.Rotate(0, 0, 0, 1);
					RenderMesh(meshList[GEO_SPHERE], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.5, 0.2, 0); modelStack.Scale(0.2, 0.3, 0.3);
				modelStack.Rotate(rleftarmx, 1, 0, 0); modelStack.Rotate(rleftarmy, 0, 1, 0); modelStack.Rotate(rleftarmz, 0, 0, 1);
				RenderMesh(meshList[GEO_SPHERE], true);
					modelStack.PushMatrix();		//left arm
					modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 0.9, 1);
					RenderMesh(meshList[GEO_CYLINDER], true);
						modelStack.PushMatrix();
						modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
						modelStack.Rotate(rlefthandx, 1, 0, 0); modelStack.Rotate(rlefthandy, 0, 1, 0); modelStack.Rotate(rlefthandz, 0, 0, 1);
						RenderMesh(meshList[GEO_SPHERE], true);
							modelStack.PushMatrix();		//left hand
							modelStack.Translate(0, -1.2, 0); modelStack.Scale(0.9, 1.2, 0.9);
							RenderMesh(meshList[GEO_CYLINDER], true);
							modelStack.PopMatrix();
						modelStack.PopMatrix();
					modelStack.PopMatrix();
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-0.5, 0.2, 0); modelStack.Scale(0.2, 0.3, 0.3);
				modelStack.Rotate(rrightarmx, 1, 0, 0); modelStack.Rotate(rrightarmy, 0, 1, 0); modelStack.Rotate(rrightarmz, 0, 0, 1);
				RenderMesh(meshList[GEO_SPHERE], true);
					modelStack.PushMatrix();		//right arm
					modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 0.9, 1);
					RenderMesh(meshList[GEO_CYLINDER], true);
						modelStack.PushMatrix();
						modelStack.Translate(0, -1.2, 0); modelStack.Scale(1, 1, 1);
						modelStack.Rotate(rrighthandx, 1, 0, 0); modelStack.Rotate(rrighthandy, 0, 1, 0); modelStack.Rotate(rrighthandz, 0, 0, 1);
						RenderMesh(meshList[GEO_SPHERE], true);
							modelStack.PushMatrix();		//right hand
							modelStack.Translate(0, -1.2, 0); modelStack.Scale(0.9, 1.2, 0.9);
							RenderMesh(meshList[GEO_CYLINDER], true);
							modelStack.PopMatrix();
						modelStack.PopMatrix();
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();

		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Scene17::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void Scene17::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix(); viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix(); modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0); modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * .75f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix(); viewStack.PopMatrix(); modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}

void Scene17::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Scene17::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Scene17::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);		//size of screen ui
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();		//no need for camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0); modelStack.Scale(sizex, sizey, 0);
	RenderMesh(mesh, false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}