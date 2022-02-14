#include "PoliceMeter.h"
#include <cstdlib>
#include <ctime>

PoliceMeter::PoliceMeter()
{
	MeterPercentage = 0; // meter starts out 0%
	BribeSuccess = 100; // chance of bribe succeeding starts at 100%
}

PoliceMeter::~PoliceMeter()
{
}

void PoliceMeter::DailyIncreaseMP(int NoOfNoobWorkers, 
	int NoOfExperiencedWorkers, int NoOfExpertWorkers, bool minus3boost)
{
	// formula based on the google doc
	int dailyIncrease = (NoOfNoobWorkers * 3) + (NoOfExperiencedWorkers * 2) + (NoOfExpertWorkers * 1);

	// if boost has been bought and the bool value is true,
	// the daily increase will - 3%
	if ((minus3boost = true) && (dailyIncrease > 3))
	{
		dailyIncrease = dailyIncrease - 3;
	}
	else // in the scenario that daily inc. is alr < 3 and the boost is active
	{
		dailyIncrease = 0;
	}

	// add current meter % and the daily increase
	MeterPercentage = MeterPercentage + dailyIncrease;
}

void PoliceMeter::Bribe()
{
	int BribeChance;
	BribeChance = rand() % 100 + 1;

	// if the random number falls within the bribesuccess range,
	// bribe succeeds. If not, police meter + 5%.
	if (BribeChance <= BribeSuccess) 
	{
		MeterPercentage = MeterPercentage - (MeterPercentage / 2);
	}
	else
	{
		MeterPercentage = MeterPercentage + 5;
	}

	// after every bribe, successful or not, chance of getting next
	// bribe to work decreases(lowest the success rate goes is 30%)
	BribeSuccess = BribeSuccess - 10;
	if (BribeSuccess <= 30)
	{
		BribeSuccess = 30;
	}
}

void PoliceMeter::BoostBribeSuccess()
{
	BribeSuccess = BribeSuccess + 10;
}

void PoliceMeter::CustomIncreaseMP(float AmountToAdd)
{
	MeterPercentage = MeterPercentage + AmountToAdd;
}

void PoliceMeter::CustomDecreaseMP(float AmountToMinus)
{
	MeterPercentage = MeterPercentage - AmountToMinus;
	if (MeterPercentage > 0)
	{
		MeterPercentage = 0;
	}
}

int PoliceMeter::GetMP()
{
	return MeterPercentage;
}

bool PoliceMeter::IsMeterFull()
{
	if (MeterPercentage >= 100)
	{
		return true;
	}
	else
	{
		return false;
	}
}
