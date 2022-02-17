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

    //void setstationcost(float newcost);
    float getstationcost(void);/*
    void setworkercost(float newcost);*/
    float getworkercost(void);

    float getprofit(void);

    float ECoords[3];

private:
    float stationtier = 0;          //5 levels, 0 is locked, 1-4 for better station
    float workertier = 0;           //0 for empty, 1 for weak, 2 for normal, 3 for expert
    float WorkerUpgrade0 = 200, WorkerUpgrade1 = 400, Workerupgrade2 = 800;
    float StationUpgrade0 = 200, StationUpgrade1 = 400, StationUpgrade2 = 800, StationUpgrade3 = 1200;
    //if either station is bought, or if the station does not have a hire, then it wont earn money
    //plan to add the profit as a floating value above worker's head... more visual way of knowing income
    float profit;


};