#pragma once

using namespace std;

class Card
{
public:
	Card();
	Card(int V, char S);
	~Card();

	char getsuit(void);
	void setsuit(char newsuit);
	int getvalue(void);
	void setvalue(int newvalue);

	char Suit;		//H, D, C, S
	int Value;
	
private:
	//char into int works as both unicode char and int comparison

};

