#include "entity.h"

entity::entity(float X, float Y, float Z) {
    stationtier = 0;
    workertier = 0;
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