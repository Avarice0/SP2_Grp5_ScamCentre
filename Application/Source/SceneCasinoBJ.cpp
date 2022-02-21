#include "SceneCasinoBJ.h"
#include "SceneGame.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

SceneCasinoBJ::SceneCasinoBJ()
{
}

SceneCasinoBJ::~SceneCasinoBJ()
{
}

void SceneCasinoBJ::Init()
{
	{

		glClearColor(1.f, 1.0f, 1.0f, 0.0f);

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
		glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	}
	srand(time(NULL));
	// Init VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{

		meshList[GEO_TABLE] = MeshBuilder::GenerateQuad("greentext", Color(0.54, 0.27, 0.07), 16);

		meshList[GEO_GREENTEXT] = MeshBuilder::GenerateQuad("greentext", 16, 16);
		meshList[GEO_GREENTEXT]->textureID = LoadTGA("Image//greenTextBubble.tga");
		meshList[GEO_BLUETEXT] = MeshBuilder::GenerateQuad("bluetext", 16, 16);
		meshList[GEO_BLUETEXT]->textureID = LoadTGA("Image//bluetext.tga");

		meshList[GEO_LINE] = MeshBuilder::GenerateQuad("line", 16, 16);
		meshList[GEO_LINE]->textureID = LoadTGA("Image//line.tga");
		
		meshList[GEO_EXIT] = MeshBuilder::GenerateQuad("x", Color(0, 0, 0), 1.f);
		meshList[GEO_EXIT]->textureID = LoadTGA("Image//Redx.tga");
	}
	{
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSEPOS] = MeshBuilder::GenerateText("mousepos", 16, 16);
		meshList[GEO_MOUSEPOS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSESTATE] = MeshBuilder::GenerateText("mousestate", 16, 16);
		meshList[GEO_MOUSESTATE]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_SCORE] = MeshBuilder::GenerateText("score", 16, 16);
		meshList[GEO_SCORE]->textureID = LoadTGA("Image//calibri.tga");
		
	}

	

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
}

void SceneCasinoBJ::Update(double dt)
{
	//mouse inputs
	Application::GetCursorPos(&x, &y);
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	posX = x / w * 80; //convert (0,800) to (0,80)
	posY = 60 - y / h * 60; //convert (600,0) to (0,60)
	static bool bLButtonState = false;

	if (Application::IsKeyPressed('E'))
	{
		dollarsClone = SceneGame::dollars;
	}

	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		mousestate = "LBUTTON DOWN";
		if ((posX > 8 && posX < 15) && (posY > 44.5 && posY < 52))
		{
			gamenum = 1; //coins
			coin1x = rand() % 25 + 45;
			coin1y = rand() % 35 + 15;

		}
		else if ((posX > 17 && posX < 25) && (posY > 44.5 && posY < 52) && boom == false)
		{
			gamenum = 2; //texting
		}
		else if ((posX > 25 && posX < 35) && (posY > 44.5 && posY < 52) && boom == false)
		{
			gamenum = 3; //clicker mining
		}
		else {}

		if (gamenum == 1) {

			if ((posX > coinbombx - 2.5) && (posX < coinbombx + 2.5) && (posY > coinbomby - 2.5) && (posY < coinbomby + 2.5) && boom == false)
			{
				explosionx = 55;
				explosiony = 35;
				SceneGame::dollars -= 500;
				dollarsClone -= 500;
				boom = true;
			}

			if ( ( (posX > coin1x - 2.5) && (posX < coin1x + 2.5) && (posY > coin1y - 2.5) && (posY < coin1y + 2.5) && boom == false)   || 
		       ( (posX > coin2x - 2.5) && (posX < coin2x + 2.5) && (posY > coin2y - 2.5) && (posY < coin2y + 2.5) &&boom==false) )
			{
				coin1x = rand() % 25 + 45;
				coin1y = rand() % 35 + 15;

				coin2x = rand() % 25 + 45;
				coin2y = rand() % 35 + 15;
			//	cout << "coin1 ok" << endl;
				while ( (coin2x > coin1x - 5) && (coin2x < coin1x + 5) && (coin2y > coin1y - 5) && (coin2y < coin1y + 5) )
				{
					coin2x = rand() % 25 + 45;
					coin2y = rand() % 35 + 15;
					
				}
				//cout << "coin2 ok" << endl;
				coinbombx = rand() % 25 + 45;
				coinbomby = rand() % 35 + 15;
				
				while ( (coinbombx > coin1x - 5) && (coinbombx < coin1x + 5) && (coinbomby > coin1y - 5) && (coinbomby < coin1y + 5) && (coinbombx > coin2x - 5) && (coinbombx < coin2x + 5) && (coinbomby > coin2y - 5) && (coinbomby < coin2y + 5) )
				{					
						coinbombx = rand() % 25 + 45;
						coinbomby = rand() % 35 + 15;
				}
		//		cout << "coin3 ok" << endl;
				score++;

				coinStarted = true;
				//gameended = false;
			}
		
			else if ((posX > 32 && posX < 75) && (posY > 8 && posY < 53 && gameended == false))
			{
				score--;
				
			}
			gameended = true;
		}
		else if (gamenum == 2)
		{
			gameended = true;
			coin1x = 100; coin1y = 100;
			coin2x = 100; coin2y = 100;
			coinbombx = 100; coinbomby = 100;
			if ((posX > 38 && posX < 56) && (posY > 8 && posY < 21))
			{
				if (correctPos == true) {
					textscore++;
				}
				else
				{
					textscore--;
				}
				RNGmsg = rand() % 8;
				correctPos = rand() % 2;
			}
			if ((posX > 56 && posX < 74) && (posY > 8 && posY < 22))
			{
				if (correctPos == true) {
					textscore--;
				}
				else
				{
					textscore++;
				}
				RNGmsg = rand() % 8;
				correctPos = rand() % 2;
			}
			
		}
		else if (gamenum == 3)
		{
			if ((posX > 38 && posX < 74) && (posY > 8.5 && posY < 53)) {

				if (rand() % 2 == true) 
				{
					cout << "heads" << endl;
					heady = 30;
					taily = 100;
				}
				else
				{
					cout << "tails" << endl;
					taily = 30;
					heady = 100;
				}
			}
			gameended = true;
		}
		if (gameended == true) {

			SceneGame::dollars += score * 2;
			SceneGame::dollars += textscore * 4;
			SceneGame::dollars += minescore * 2;
			
			dollarsClone += score * 2;
			dollarsClone += textscore * 4;
			dollarsClone += minescore * 2;

			score = 0;
			textscore = 0;
			minescore = 0;
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

	
	int times = Application::GetTime(); // in seconds 
	hours = times % 5;
	day = times / 5;
	if (day == SceneGame::daydivide && times != 0)
	{

		bool dayUp = true;
		SceneGame::daydivide++;
		dollarsClone += SceneGame::profit;
		std::cout << (SceneGame::profit);
		std::cout << "if is OK ";

		explosionx = 100;
		explosiony = 100;
		
		boom = false;
	}




	timeprint = "Day:" + to_string(day) + ",Hour:" + to_string(hours);
	

	if (coinStarted == true) {
		totalframe++;
		if (totalframe >= 60)
		{
			totalframe = 0;
			seconds--;
		}
		if (seconds < 0) {
			seconds = 0;
			coinStarted = false;
			coin1x = rand() % 25 + 45;
			coin1y = rand() % 35 + 15;


			coin2x = 100;
			coin2y = 100;
			coinbombx = 100;
			coinbomby = 100;
			seconds = 5;
		}


	}
	
}

void SceneCasinoBJ::Render()
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
	}
	RenderMeshOnScreen(meshList[GEO_TABLE], 40 ,30, 10, 10);
	//UI buttons test
	string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
	RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 3.5);
	
	RenderMeshOnScreen(meshList[GEO_EXIT], 78, 58, 4, 4);

	RenderTextOnScreen(meshList[GEO_SCORE], timeprint, Color(0,0,0), 2, 10,37);
	
	//---------------------------------------------------------
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
}


void SceneCasinoBJ::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneCasinoBJ::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
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

void SceneCasinoBJ::Exit()
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

void SceneCasinoBJ::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneCasinoBJ::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

//#include <iostream>
//#include <string>
//#include <conio.h>
//
//#include "Deck.h"
//using namespace std;
//
//int main(void) {
//	int balance = 1000;
//	int bettingvalue = 0;
//	int playerinput; bool pass;
//	Deck OpenDeck;
//	OpenDeck.resetopen();
//	while (balance > 0) {
//		do {
//			system("CLS");
//			std::cout << "Welcome to BlackJack!" << std::endl;
//			std::cout << "Press buttons to change bets." << std::endl;
//			std::cout << "You have $" << balance << " in your account." << std::endl;
//			std::cout << "You are betting $" << bettingvalue << std::endl;
//			std::cout << "Q for -100, W for -50, E for -10, R to start the game, T for +10, Y for +50, U for +100" << std::endl;
//			pass = false;
//			playerinput = _getch();
//			if (playerinput == 114) {		//A to continue next phase
//				pass = true;
//			}
//			else {
//				//reduce bet value
//				if (playerinput == 113 && bettingvalue >= 100) {
//					bettingvalue -= 100;
//				}
//				if (playerinput == 119 && bettingvalue >= 50) {
//					bettingvalue -= 50;
//				}
//				if (playerinput == 101 && bettingvalue >= 10) {
//					bettingvalue -= 10;
//				}
//				if (playerinput == 116 && bettingvalue + 10 <= balance) {
//					bettingvalue += 10;
//				}
//				if (playerinput == 121 && bettingvalue + 50 <= balance) {
//					bettingvalue += 50;
//				}
//				if (playerinput == 117 && bettingvalue + 100 <= balance) {
//					bettingvalue += 100;
//				}
//			}
//		} while (pass == false);
//
//		OpenDeck.resethand(OpenDeck.dealerhand); OpenDeck.resethand(OpenDeck.playerhand);
//		int result = 0;			//0 is no result, 1 is lose, 2 is tie, 3 is win
//		OpenDeck.addcard(OpenDeck.dealerhand); OpenDeck.addcard(OpenDeck.dealerhand);
//		OpenDeck.addcard(OpenDeck.playerhand); OpenDeck.addcard(OpenDeck.playerhand);
//		bool stand = false;
//		do {
//			system("CLS");
//			std::cout << "You have $" << balance << " in your account." << std::endl;
//			std::cout << "You are betting $" << bettingvalue << std::endl;
//			std::cout << "dealer hand:" << OpenDeck.valuecount(OpenDeck.dealerhand) << std::endl; OpenDeck.printdeck(OpenDeck.dealerhand);
//			std::cout << "player hand:" << OpenDeck.valuecount(OpenDeck.playerhand) << std::endl; OpenDeck.printdeck(OpenDeck.playerhand);
//			if (OpenDeck.valuecount(OpenDeck.playerhand) == 21 && OpenDeck.valuecount(OpenDeck.dealerhand) == 21) {
//				result = 2;
//			}
//			else if (OpenDeck.valuecount(OpenDeck.dealerhand) > 21) {
//				result = 3;
//			}
//			else if (OpenDeck.valuecount(OpenDeck.playerhand) > 21) {
//				result = 1;
//			}
//			else if (stand == true) {
//				if (OpenDeck.valuecount(OpenDeck.dealerhand) > OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 1;
//				}
//				else if (OpenDeck.valuecount(OpenDeck.dealerhand) == OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 2;
//				}
//				else if (OpenDeck.valuecount(OpenDeck.dealerhand) < OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 3;
//				}
//			}
//			else if (stand == false) {
//				if (OpenDeck.valuecount(OpenDeck.dealerhand) <= 21 && OpenDeck.valuecount(OpenDeck.playerhand) <= 21) {
//					std::cout << "Q to Hit, W to stand" << std::endl;
//					playerinput = _getch();
//					if (playerinput == 113) {
//						OpenDeck.addcard(OpenDeck.playerhand);
//					}
//					else if (playerinput == 119) {
//						while (OpenDeck.valuecount(OpenDeck.dealerhand) <= 17) {
//							OpenDeck.addcard(OpenDeck.dealerhand);
//						}
//						stand = true;
//					}
//				}
//			}
//
//		} while (result == 0);
//
//		if (result == 1) {
//			std::cout << "Player lost" << std::endl;
//			balance -= bettingvalue;
//		}
//		else if (result == 2) {
//			std::cout << "Tie/Push" << std::endl;
//		}
//		else if (result == 3) {
//			std::cout << "Player won" << std::endl;
//			balance += bettingvalue;
//		}
//
//		std::cout << "Do you want to play again?: Q for yes, W for no";
//		int playagain = 0;
//		do {
//			playerinput = _getch();
//			if (playerinput == 113) {
//				playagain = 1;
//			}
//			else if (playerinput == 119) {
//				playagain = 2;
//			}
//		} while (playagain == 0);
//		if (playagain == 2) {
//			std::cout << "You left with $" << balance << std::endl;
//			break;
//		}
//	}
//	if (balance <= 0) {
//		std::cout << "You ran out of money!" << std::endl;
//	}
//
//
//	return 0;
//}