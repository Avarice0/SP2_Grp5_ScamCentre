#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	void changescene(int newscene);

	//input
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static double GetTime();
	static void SetTime(double time);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static unsigned m_width;
	static unsigned m_height;
  
private:

	//Declare a window object
	StopWatch m_timer;
	int scenenum = 1;
	/*	int time=0;*/
};

#endif