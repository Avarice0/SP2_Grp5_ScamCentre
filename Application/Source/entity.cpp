#include "entity.h"

entity::entity(float X, float Y, float Z) {
    stationtier = 1;
    workertier = 1;
    ECoords[0] = X; ECoords[1] = Y; ECoords[2] = Z;
}
entity::~entity() {

}
int entity::getstationtier(void) {
    return stationtier;
}
void entity::setstationtier(int newtier) {
    stationtier = newtier;
}

int entity::getworkertier(void) {
    return workertier;
}
void entity::setworkertier(int newtier) {
    workertier = newtier;
}

float entity::getprofit(void) {
    profit = stationtier * workertier;
    return profit;
}

void entity::setstationcost(float newcost) {
    stationcost = newcost;
}
float entity::getstationcost(void) {
    return stationcost;
}
void entity::setworkercost(float newcost) {
    workercost = newcost;
}
float entity::getworkercost() {
    return workercost;
}