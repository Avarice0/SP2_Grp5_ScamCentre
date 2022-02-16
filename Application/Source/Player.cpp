#include "Player.h"

Player::Player()
{
	X = 0;
	Z = 0;
}

Player::~Player()
{
}

void Player::UpdatePlayerPosition(int x, int z)
{
	X = x;
	Z = z;
}

int Player::GetPlayerX()
{
	return X;
}

int Player::GetPlayerZ()
{
	return Z;
}
