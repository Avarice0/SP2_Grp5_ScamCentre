#pragma once
class entity
{
    //entity is the work station
public:
    entity();
    ~entity();

    int gettype(void);
    void settype(int newtype);

    int getstationtier(void);
    void setstationtier(int newtier);

    int getworkertier(void);
    void setworkertier(int newtier);

    float getprofit(void);

private:
    float type;            //0 for mail, 1 for phone, 2 for computer, 3 for hacking
    float stationtier;    //5 levels, 0 is locked, 1-5 for better station
    float workertier;        //0 for empty, 1 for weak, 2 for normal, 3 for expert
    //if either station is bought, or if the station does not have a hire, then it wont earn money
    //plan to add the profit as a floating value above worker's head... more visual way of knowing income
    float profit;
};