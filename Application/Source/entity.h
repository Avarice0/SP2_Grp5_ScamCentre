#pragma once
class entity
{
    //entity is the work station
public:
    entity(float X, float Y, float Z);
    ~entity();

    int getstationtier(void);
    void setstationtier(int newtier);

    int getworkertier(void);
    void setworkertier(int newtier);

    void setstationcost(float newcost);
    float getstationcost(void);
    void setworkercost(float newcost);
    float getworkercost(void);

    float getprofit(void);

    float ECoords[3];

private:
    float stationtier = 0;          //5 levels, 0 is locked, 1-4 for better station
    float stationcost = 100;
    float workertier = 0;           //0 for empty, 1 for weak, 2 for normal, 3 for expert
    float workercost = 100;
    //if either station is bought, or if the station does not have a hire, then it wont earn money
    //plan to add the profit as a floating value above worker's head... more visual way of knowing income
    float profit;


};