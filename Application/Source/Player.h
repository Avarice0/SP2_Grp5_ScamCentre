#pragma once
class Player {
private:
	int X, Z;

public:
	Player();
	~Player();

	void UpdatePlayerPosition(int x, int z);
	int GetPlayerX();
	int GetPlayerZ();
};