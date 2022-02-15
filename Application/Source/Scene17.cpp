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
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{
		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
		meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.25, 0.75, 0.25), 1.f);
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
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
	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);


	debugRot = 20;
}

void Scene17::Update(double dt)
{
	camera.Update(dt);

	light[0].position.x = camera.position.x;
	light[0].position.y = camera.position.y;
	light[0].position.z = camera.position.z;
	light[0].spotDirection.Set(camera.position.x-camera.target.x, camera.position.y - camera.target.y, camera.position.z - camera.target.z);
	
	//if (flashlighttoggle == false && Application::IsKeyPressed('Q')) {
	//	flashlighttoggle = true;							//flashlight toggle
	//	if (light[0].power == 1.f) {
	//		light[0].power = 0.f;
	//		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	//	}
	//	else if (light[0].power == 0.f) {
	//		light[0].power = 1.f;
	//		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	//	}
	//}
	//else if (flashlighttoggle == true && !Application::IsKeyPressed('Q')) {
	//	flashlighttoggle = false;
	//}
	//else {}

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
			mousestate = "shop click";
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
	entities[0] = new entity();
	entities[1] = new entity();
	entities[2] = new entity();
	totalframe++;
	if (totalframe >= 1440) {
		totalframe = 0;
		day++;
		for (int i = 0; i < size(entities); i++) {
			if (entities[i] != NULL) {
				dollars += entities[i]->getprofit();
			}
		}
	}
	time = "Day:" + to_string(day) + ",Hour:" + to_string(totalframe / 60);

	debugRot += (float)(20 * dt);

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
	//----------------------------------------
	//RenderMesh(meshList[GEO_AXES], false);
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();
	
	for (int i = 0; i < size(entities); i++) {
		if (entities[i] != NULL) {
			renderworker(i * 10, 12, 20, entities[i]->getworkertier());
		}
	}

	//text render
	string coord = to_string(camera.position.x) + "," + to_string(camera.position.y) + "," + to_string(camera.position.z);
	RenderTextOnScreen(meshList[GEO_COORDS], coord, Color(0.5, 0.5, 1), 2, 0, 0.5);

	//render mesh on screen
	//RenderMeshOnScreen(meshList[GEO_QUAD], 40, 30, 20, 10);

	//UI buttons test
	string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
	RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 3.5);
	RenderTextOnScreen(meshList[GEO_TIME], time, Color(0.5, 0.5, 1), 2, 60, 57.5);
	RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(dollars), Color(0.5, 0.5, 1), 2, 2, 57.5);

	//---------------------------------------------------------
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
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

	for (int i = 0; i < size(entities); i++) {
		delete entities[i];
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

void Scene17::renderworker(int x, int y, int z, int rarity) {
	modelStack.PushMatrix(); 
	modelStack.Translate(x, y, z); modelStack.Scale(1, 1.3, 1);
	RenderMesh(meshList[GEO_SKINNED], true);
	modelStack.Translate(0, -2, 0); modelStack.Scale(1.1, 1.5, 1.1);
	RenderMesh(shirtrarity[rarity], true);

		modelStack.PushMatrix(); 
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -0.3, -1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -0.3, 1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix(); 
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix(); 

		modelStack.PushMatrix(); 
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix(); 

	modelStack.PopMatrix();
}