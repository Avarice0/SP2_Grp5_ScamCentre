#pragma once
class PoliceMeter
{
private:
	int MeterPercentage; // the police-meter "score"
	int BribeSuccess; // global variable

public:
	PoliceMeter(); // con
	~PoliceMeter(); // decon

	// the daily increase in meter %
	void DailyIncreaseMP(int NoOfNoobWorkers, 
		int NoOfExperiencedWorkers, int NoOfExpertWorkers, bool minus3boost);
	
	// call when Bribe is used (random chance of bribe working has alr. been implemented)
	void Bribe(); 

	// increases bribe success by 10%(max 5 times not implemented here)
	void BoostBribeSuccess();

	// custom amount to increase MP
	void CustomIncreaseMP(float AmountToAdd);
	
	// custom amount to decrease MP
	void CustomDecreaseMP(float AmountToMinus);

	// getter for meter %
	int GetMP();

	// returns true when meter reaches 100% and can be used to check when game should end
	bool IsMeterFull();
};
