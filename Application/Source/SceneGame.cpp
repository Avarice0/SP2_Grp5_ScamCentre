#include "SceneGame.h"
#include <math.h> 

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
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
	camera.Init(Vector3(30, 10, -10), Vector3(0, 10, 0), Vector3(0, 1, 0));
	camera2.Init(Vector3(0, 120, 160), Vector3(0, 0, 25), Vector3(0, 1, 0));

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
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{
		meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.25, 0.75, 0.25), 1.f);
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	}
	{
		meshList[GEO_UPGRADESHOPBG] = MeshBuilder::GenerateQuad("shopbg", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADESHOPFG] = MeshBuilder::GenerateQuad("shopfg", Color(1, 0, 0), 1.f);
		meshList[GEO_LOCKEDFG] = MeshBuilder::GenerateQuad("lockfg", Color(0.5, 0.5, 0.5), 1.f);
		meshList[GEO_UPGRADEITEM1] = MeshBuilder::GenerateQuad("item1", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADEITEM1]->textureID = LoadTGA("Image//Coffee.tga");
		meshList[GEO_UPGRADEITEM2] = MeshBuilder::GenerateQuad("item2", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADEITEM2]->textureID = LoadTGA("Image//PoliceCap.tga");
		meshList[GEO_LOCK] = MeshBuilder::GenerateQuad("lock", Color(1, 1, 1), 1.f);
		meshList[GEO_LOCK]->textureID = LoadTGA("Image//Lock.tga");
		meshList[GEO_WORKERUPGRADE] = MeshBuilder::GenerateQuad("WorkerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_WORKERUPGRADE]->textureID = LoadTGA("Image//WorkerUp.tga");
		meshList[GEO_COMPUTERUPGRADE] = MeshBuilder::GenerateQuad("ComputerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_COMPUTERUPGRADE]->textureID = LoadTGA("Image//ComputerUp.tga");
		meshList[GEO_PUPGRADE] = MeshBuilder::GenerateQuad("PermUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_PUPGRADE]->textureID = LoadTGA("Image//PermUpgrade.tga");
		meshList[GEO_WUPGRADE] = MeshBuilder::GenerateQuad("WorkerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_WUPGRADE]->textureID = LoadTGA("Image//ComputerUp.tga");
		meshList[GEO_METREBARBG] = MeshBuilder::GenerateQuad("metrebarbg", Color(1, 1, 1), 1.f);
		meshList[GEO_METREBARBG]->textureID = LoadTGA("Image//Metrebar.tga");
		meshList[GEO_METREBARFG] = MeshBuilder::GenerateQuad("quad", Color(1, 0.1, 0.1), 1.f);
		meshList[GEO_METREBARBGBG] = MeshBuilder::GenerateQuad("quad", Color(0.4, 0.4, 0.4), 1.f);
		meshList[GEO_METREBARBULB] = MeshBuilder::GenerateCircle("circle", Color(1, 0.4, 0.4), 20, 1.f);
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
		meshList[GEO_TIME] = MeshBuilder::GenerateText("time", 16, 16);
		meshList[GEO_TIME]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_DOLLARS] = MeshBuilder::GenerateText("dollars", 16, 16);
		meshList[GEO_DOLLARS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_PROFIT] = MeshBuilder::GenerateText("profit", 16, 16);
		meshList[GEO_PROFIT]->textureID = LoadTGA("Image//calibri.tga");
	}

	{
		meshList[GEO_SKINNED] = MeshBuilder::GenerateSphere("sphere", Color(1, 0.8, 0.6), 5, 5, 1.f);
		meshList[GEO_SKINNED]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SKINNED]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SKINNED]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SKINNED]->material.kShininess = 1.f;

		meshList[GEO_PANTS] = MeshBuilder::GenerateSphere("sphere", Color(0, 0, 0.4), 5, 5, 1.f);
		meshList[GEO_PANTS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PANTS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PANTS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PANTS]->material.kShininess = 1.f;

		meshList[GEO_SHIRT1] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0.5, 0.5), 5, 5, 1.f);
		meshList[GEO_SHIRT1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT1]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT1]->material.kShininess = 1.f;

		meshList[GEO_SHIRT2] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 1, 0.5), 5, 5, 1.f);
		meshList[GEO_SHIRT2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT2]->material.kShininess = 1.f;

		meshList[GEO_SHIRT3] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0.5, 1), 5, 5, 1.f);
		meshList[GEO_SHIRT3]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT3]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT3]->material.kShininess = 1.f;

		shirtrarity[0] = meshList[GEO_SHIRT1]; shirtrarity[1] = meshList[GEO_SHIRT2];
		shirtrarity[2] = meshList[GEO_SHIRT3]; 
	}
	
	{
		meshList[GEO_TABLE] = MeshBuilder::GenerateCube("table", Color(0.54, 0.27, 0.07), 1);
		//meshList[GEO_TABLE]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_TABLE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_TABLE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TABLE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_TABLE]->material.kShininess = 1.f;

		meshList[GEO_ROOM] = MeshBuilder::GenerateQuad("room", Color(0.1, 0.1, 0.5), 1);
		//meshList[GEO_ROOM]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_ROOM]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_ROOM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_ROOM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_ROOM]->material.kShininess = 1.f;

		meshList[GEO_FLOORTILES] = MeshBuilder::GenerateQuad("roomtiles", Color(1, 1, 1), 1);
		meshList[GEO_FLOORTILES]->textureID = LoadTGA("Image//floor tiles.tga");

		meshList[GEO_OFFICE] = MeshBuilder::GenerateQuad("playeroffice", Color(0.3, 0.3, 0.3), 1);
		//meshList[GEO_OFFICE]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_OFFICE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_OFFICE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_OFFICE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_OFFICE]->material.kShininess = 1.f;

		meshList[GEO_UPGRADEAREA] = MeshBuilder::GenerateQuad("upgradearea", Color(0.1, 0.1, 0.1), 1);
		//meshList[GEO_UPGRADEAREA]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_UPGRADEAREA]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_UPGRADEAREA]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_UPGRADEAREA]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_UPGRADEAREA]->material.kShininess = 1.f;
	}
	{
		meshList[GEO_PHONE] = MeshBuilder::GenerateCube("phone", Color(0.4, 0.4, 0.4), 1);
		meshList[GEO_PHONE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE]->material.kShininess = 1.f;

		meshList[GEO_PHONE1] = MeshBuilder::GenerateCube("phone1", Color(0.7, 0.7, 0.7), 1);
		meshList[GEO_PHONE1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE1]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE1]->material.kShininess = 1.f;

		meshList[GEO_PHONE2] = MeshBuilder::GenerateCube("phone2", Color(0.6, 0.2, 0.2), 1);
		meshList[GEO_PHONE2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE2]->material.kShininess = 1.f;

		meshList[GEO_PAPERSTACK] = MeshBuilder::GenerateCube("paperstack", Color(0.9, 0.9, 0.9), 1);
		//meshList[GEO_PAPERSTACK]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_PAPERSTACK]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PAPERSTACK]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PAPERSTACK]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PAPERSTACK]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP] = MeshBuilder::GenerateCube("laptop", Color(0.6, 0.6, 0.6), 1);
		//meshList[GEO_LAPTOP]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP2] = MeshBuilder::GenerateCube("laptop2", Color(0.05, 0.05, 0.05), 1);
		//meshList[GEO_LAPTOP2]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP2]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP3] = MeshBuilder::GenerateCube("laptop3", Color(0.3, 0.3, 0.3), 1);
		//meshList[GEO_LAPTOP3]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP3]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP3]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP3]->material.kShininess = 1.f;
		
		meshList[GEO_INKJAR] = MeshBuilder::GenerateCylinder("inkjar", Color(0.1,0.1, 0.1), 8, 1, 1.f);
		meshList[GEO_INKJAR]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_INKJAR]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_INKJAR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_INKJAR]->material.kShininess = 1.f;

		meshList[GEO_PENBASE] = MeshBuilder::GenerateCylinder("penbase", Color(0.9, 0.9, 0.9), 8, 1, 1.f);
		meshList[GEO_PENBASE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PENBASE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PENBASE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PENBASE]->material.kShininess = 1.f;

		meshList[GEO_FEATHER] = MeshBuilder::GenerateCube("feather", Color(0.7, 0.7, 0.7), 1);
		meshList[GEO_FEATHER]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FEATHER]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_FEATHER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_FEATHER]->material.kShininess = 1.f;
	}
	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	debugRot = 20;

	{
		entities[0] = new entity(000, 6, 00); entities[5] = new entity(000, 6, 20);	entities[10] = new entity(000, 6, 40); entities[15] = new entity(000, 6, 60);
		entities[1] = new entity(-15, 6, 00); entities[6] = new entity(-15, 6, 20); entities[11] = new entity(-15, 6, 40); entities[16] = new entity(-15, 6, 60);
		entities[2] = new entity(-30, 6, 00); entities[7] = new entity(-30, 6, 20); entities[12] = new entity(-30, 6, 40); entities[17] = new entity(-30, 6, 60);
		entities[3] = new entity(-45, 6, 00); entities[8] = new entity(-45, 6, 20); entities[13] = new entity(-45, 6, 40); entities[18] = new entity(-45, 6, 60);
		entities[4] = new entity(-60, 6, 00); entities[9] = new entity(-60, 6, 20); entities[14] = new entity(-60, 6, 40); entities[19] = new entity(-60, 6, 60);
	}
}

void SceneGame::Update(double dt)
{
	camera.Update(dt);

	player.UpdatePlayerPosition(camera.position.x, camera.position.z);
	for (int i = 0; i < size(entities); i++) {
		if ((player.GetPlayerX() > entities[i]->ECoords[0] - 10) && (player.GetPlayerX() < entities[i]->ECoords[0] - 5)) {
			if ((player.GetPlayerZ() > entities[i]->ECoords[2] - 2.5) && (player.GetPlayerZ() < entities[i]->ECoords[2] + 2.5)) {
				if (camera.GetDirection() == 0) {
					player.UpdatePlayerPosition(entities[i]->ECoords[0], entities[i]->ECoords[2] - 1);
					std::cout << " collided up" << std::endl;
				}
				else if (camera.GetDirection() == 1) {
					player.UpdatePlayerPosition(entities[i]->ECoords[0], entities[i]->ECoords[2] + 1);
					std::cout << " collided down" << std::endl;
				}
				else if (camera.GetDirection() == 2) {
					player.UpdatePlayerPosition(entities[i]->ECoords[0] - 1, entities[i]->ECoords[2]);
					std::cout << " collided left" << std::endl;
				}
				else if (camera.GetDirection() == 3) {
					player.UpdatePlayerPosition(entities[i]->ECoords[0] + 1, entities[i]->ECoords[2]);
					std::cout << " collided right" << std::endl;
				}
				else {}
			}
		}
	}
	if (metre.GetMP() == 1000) {
		SceneEnd::EndingScene(1);
		Application::changescene(4);
	}
	if (dollars <=-1) {
		SceneEnd::EndingScene(2);
		Application::changescene(4);
	}
	if (dollars >= 600)
		RenderPermItem1 = true;
	if (dollars >= 600)
		RenderPermItem2 = true;

	for (int i = 0; i < size(entities); i++) {
		if (entities[i]->getworkertier() == 1) {
			NoobCount++;
		}
		else if (entities[i]->getworkertier() == 2) {
			ExperiencedCount++;
		}
		else if (entities[i]->getworkertier() == 3) {
			ExpertCount++;
		}
		else {}
	}

	//mouse inputs
	{
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		posX = x / w * 80; //convert (0,800) to (0,80)
		posY = 60 - y / h * 60; //convert (600,0) to (0,60)
	}
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		mousestate = "LBUTTON DOWN";

		//converting viewport space to UI space
		/*if ((posX > 30 && posX < 50) && (posY > 25 && posY < 35))
		{
			mousestate = "shop click";
		}*/

		if (RenderPermItem1 == true && coffee == false) {
			if ((posX > 2.4 && posX < 17.4) && (posY > 1.6 && posY < 8.5))
			{
				coffee = true;
				RenderPermItem1 = false;
				mousestate = "Coffee Bought";
				dollars -= 600;
			}
		}
		if (RenderPermItem2 == true && policedeter == false) {
			if ((posX > 22.4 && posX < 37.4) && (posY > 1.6 && posY < 8.5))
			{
				//Add to police meter later
				policedeter = true;
				RenderPermItem2 = false;
				mousestate = "Police Deterrence Bought";
				dollars -= 600;
			}
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
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		mousestate = "";
	}

	//worker counter
	NoobCount = 0; ExperiencedCount = 0; ExpertCount = 0;
	for (int i = 0; i < size(entities); i++) {
		if (entities[i]->getworkertier() == 1) {
			NoobCount++;
		}
		if (entities[i]->getworkertier() == 2) {
			ExperiencedCount++;
		}
		if (entities[i]->getworkertier() == 3) {
			ExpertCount++;
		}
	}

	int times = Application::GetTime(); // in seconds 
	
	if (times / 5 == daydivide && times != 0)
	{
		dayUp = true;
		daydivide++;
		//Application::getday();
	}
	if (dayUp == true) {

	//	Application::SetTime(0);
		day++;
		metre.DailyIncreaseMP(NoobCount, ExperiencedCount, ExpertCount, policedeter);
		for (int i = 0; i < size(entities); i++) {
			if (entities[i] != NULL) {
				if (coffee == false) {
					dollars += entities[i]->getprofit();
				}
				else {
					dollars += entities[i]->getprofit() * 1.1;
				}

			}
		}
		dayUp = false;
	}
	//time = "Day:" + to_string(day) + ",Hour:" + to_string(totalframe / 60);
	time = "Day:" + to_string(day) + ",Hour:" + to_string(times);

	if (playerMoving == true)
	{
		if (debugRot <= -40)
		{
			legBack = false;
		}
		if (debugRot >= 40)
		{
			legBack = true;
		}
	}
	else
	{
		debugRot = 0;
	}

	if (legBack == false) {
		debugRot += (float)(40 * dt);
	}
	else
	{
		debugRot += (float)(-40 * dt);
	}

	if (Application::IsKeyPressed('9')) {
		cameranumber = 2;
	}
	else if(Application::IsKeyPressed('8')) {
		cameranumber = 1;
	}

	if (Application::IsKeyPressed('E')) {		
		if ((camera.position.x > 27 && camera.position.x < 97) && (camera.position.z > -74 && camera.position.z < -36)) {
			Application::changescene(3);

		}
	}
}

void SceneGame::Render()
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
			if(cameranumber == 1){
				view.SetToLookAt(
					camera2.position.x, camera2.position.y, camera2.position.z,
					camera2.target.x, camera2.target.y, camera2.target.z,
					camera2.up.x, camera2.up.y, camera2.up.z
				);
				viewStack.LoadIdentity();
				viewStack.LookAt(camera2.position.x, camera2.position.y, camera2.position.z,
					camera2.target.x, camera2.target.y, camera2.target.z,
					camera2.up.x, camera2.up.y, camera2.up.z);
				modelStack.LoadIdentity();
				renderworker(player.GetPlayerX(), 5, player.GetPlayerZ(), 1);
			}
			else if (cameranumber){
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
		}
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		//----------------------------------------
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_FLOOR], true);
		modelStack.PopMatrix();
		RenderRoom();

		for (int i = 0; i < size(entities); i++) {
			RenderTable(entities[i]->ECoords[0] - 5, 3, entities[i]->ECoords[2]);
			if (entities[i]->getstationtier() == 0) {
				//place obj above table
			}
			else if (entities[i]->getstationtier() == 1) {
				//place obj above table
			}
			else if (entities[i]->getstationtier() == 2) {
				//place obj above table
			}
			else {}		//statement break
			if (entities[i]->getworkertier() > 0) {
				renderworker(entities[i]->ECoords[0], entities[i]->ECoords[1], entities[i]->ECoords[2], entities[i]->getworkertier());
			}
		}

		//text render
		string coord = to_string(camera.position.x) + "," + to_string(camera.position.y) + "," + to_string(camera.position.z);
		RenderTextOnScreen(meshList[GEO_COORDS], coord, Color(0.5, 0.5, 1), 2, 0, 22.5);

		//render mesh on screen
		//RenderMeshOnScreen(meshList[GEO_QUAD], 40, 30, 20, 10);

		//UI buttons test
		string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
		RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 20);
		RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 30.5);
		RenderTextOnScreen(meshList[GEO_TIME], time, Color(0.5, 0.5, 1), 2, 60, 57.5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(dollars), Color(0.5, 0.5, 1), 2, 2, 57.5);

		if ((camera.position.x < 98 && camera.position.x > 27) && (camera.position.z < 74 && camera.position.z > 20)) {
			RenderPermUpgrade();
		}
		else {
			for (int i = 0; i < size(entities); i++) {
				//distance
				float distance = sqrt((camera.position.x - entities[i]->ECoords[0] + 5) * (camera.position.x - entities[i]->ECoords[0] + 5) +
					(camera.position.z - entities[i]->ECoords[2]) * (camera.position.z - entities[i]->ECoords[2]));
				if (distance <= 5) {
					RenderUpgrade();
					//get upgrade cost and tier
					//render the name and attach it to unique entity
				}
			}
		}
		RenderPoliceMetre();


		//---------------------------------------------------------
		Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	
}

void SceneGame::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
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

void SceneGame::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}

	for (int i = 0; i < size(entities); i++) {
		delete entities[i];
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


void SceneGame::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneGame::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

void SceneGame::renderworker(int x, int y, int z, int rarity) {
	rarity--;
	modelStack.PushMatrix(); 
	modelStack.Translate(x, y, z); modelStack.Scale(1, 1.3, 1);
	RenderMesh(meshList[GEO_SKINNED], true);
	modelStack.Translate(0, -2, 0); modelStack.Scale(1.1, 1.5, 1.1);
	RenderMesh(shirtrarity[rarity], true);

		modelStack.PushMatrix(); 
		modelStack.Rotate(debugRot, 0, 0, 1);
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -0.3, -1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-debugRot, 0, 0, 1);
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -0.3, 1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix(); 
		modelStack.Rotate(debugRot, 0, 0, 1);
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix(); 

		modelStack.PushMatrix(); 
		modelStack.Rotate(-debugRot, 0, 0, 1);
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix(); 

	modelStack.PopMatrix();
}

void SceneGame::RenderRoom(void)
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Translate(0, 0.01, 0);
		// room floor
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(20, 15, 1);
		RenderMesh(meshList[GEO_FLOORTILES], true);
		modelStack.PopMatrix();

		// room left side
		modelStack.PushMatrix();
		modelStack.Translate(-10, 2.5, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(15, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		// room right side
		modelStack.PushMatrix();
		modelStack.Translate(10, 2.5, 0);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(15, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		// room back side
		modelStack.PushMatrix();
		modelStack.Translate(0, 2.5, -7.5);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(20, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		// office area
		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.03, -5.5);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(7, 3.5, 1);
		RenderMesh(meshList[GEO_OFFICE], true);
		modelStack.PopMatrix();

		// upgrade area
		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.03, 4.75);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(7, 5, 1);
		RenderMesh(meshList[GEO_UPGRADEAREA], true);
		modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneGame::RenderTable(int x, int y, int z)
{
	// tabletop
	modelStack.PushMatrix();
	modelStack.Translate(x, y, z);
	modelStack.Scale(5, 0.2, 5);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.Scale(0.1, 15, 0.1);
	modelStack.Translate(4, -0.5, 4);

	RenderMesh(meshList[GEO_TABLE], true);
	
	modelStack.Translate(0, 0, -8);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.Translate(-8, 0, 0);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.Translate(0, 0, 8);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.PopMatrix();
}

void SceneGame::RenderQuillPaper()
{
	modelStack.PushMatrix();
	modelStack.Translate(-0.8, 0.25, 0);
	modelStack.Scale(0.25, 0.5, 0.25);
	RenderMesh(meshList[GEO_INKJAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, 1, 0);
	modelStack.Rotate(20, 0, 0, 1);
	modelStack.Scale(0.01, 1.75, 0.01);
	RenderMesh(meshList[GEO_PENBASE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.15, 1.4, 0);
	modelStack.Rotate(20, 0, 0, 1);
	modelStack.Scale(0.01, 1, 0.25);
	modelStack.Rotate(45, 1, 0, 0);
	RenderMesh(meshList[GEO_FEATHER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, 0);
	modelStack.Scale(2.9, 1, 2.1);
	RenderMesh(meshList[GEO_PAPERSTACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, 0);
	modelStack.Scale(2.9, 1, 2.1);
	RenderMesh(meshList[GEO_PAPERSTACK], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderPhone()
{
	// phone base
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.1, 0);
	modelStack.Scale(2, 0.2, 2);
	RenderMesh(meshList[GEO_PHONE], true);
	// phone screen
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0.4, -0.35);
	modelStack.Scale(0.4, 0.5, 0.2);
	RenderMesh(meshList[GEO_LAPTOP2], true);
	modelStack.PopMatrix();
	// numpad keys
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0.25);
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE2], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	//phone
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.4, 0);
	modelStack.Scale(0.75, 0.15, 1.75);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.25, -0.6);
	modelStack.Scale(0.75, 0.4, 0.55);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.25, 0.6);
	modelStack.Scale(0.75, 0.4, 0.55);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderLaptop()
{
	// laptop base
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.075, 0);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Scale(3.2, 0.15, 2.4);
	RenderMesh(meshList[GEO_LAPTOP], true);
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.4, -0.075);
		modelStack.Scale(0.85, 0.3, 0.4);
		RenderMesh(meshList[GEO_LAPTOP3], true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(-0.075, 0.4, 0.3);
		modelStack.Scale(0.35, 0.3, 0.3);
		RenderMesh(meshList[GEO_LAPTOP3], true);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// laptop screen
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.1, -1.2);
	modelStack.Rotate(75, 1, 0, 0);
	modelStack.Scale(3.2, 0.15, 2.2);
	RenderMesh(meshList[GEO_LAPTOP], true);
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.5, 0);
		modelStack.Scale(0.85, 0.3, 0.85);
		RenderMesh(meshList[GEO_LAPTOP2], true);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneGame::RenderPermUpgrade() {
	RenderMeshOnScreen(meshList[GEO_UPGRADESHOPBG], 40, 5, 80, 10);

	if (coffee == false && RenderPermItem1 == true) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_UPGRADEITEM1], 10, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 10, 5, 15, 11);
	}

	if (policedeter == false && RenderPermItem2 == true) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_UPGRADEITEM2], 30, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 30, 5, 15, 11);
	}
	RenderMeshOnScreen(meshList[GEO_PUPGRADE], 60, 5, 20, 7);
}

void SceneGame::RenderUpgrade() {
	RenderMeshOnScreen(meshList[GEO_UPGRADESHOPBG], 40, 5, 80, 10);
	if (dollars >= 0) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_WORKERUPGRADE], 10, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 10, 5, 15, 11);
	}

	if (dollars >= 0) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_COMPUTERUPGRADE], 30, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 30, 5, 15, 11);
	}


}

void SceneGame::RenderPoliceMetre()
{
	RenderMeshOnScreen(meshList[GEO_METREBARBGBG], 73, 33, 5, 20);
	RenderMeshOnScreen(meshList[GEO_METREBARFG], 73, 20, 7, 7);
	RenderMeshOnScreen(meshList[GEO_METREBARFG], 73, 22 + metre.GetMP() * 11 / 1000, 5, metre.GetMP()/ 50); 
	RenderMeshOnScreen(meshList[GEO_METREBARBG], 73, 30, 28, 30);
}
