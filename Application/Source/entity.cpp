#include "entity.h"

entity::entity() {
    type = 9;
    stationtier = 10;
    workertier = 10;
}
entity::~entity() {

}

int entity::gettype(void) {
    return type;
}
void entity::settype(int newtype) {
    type = newtype;
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
    profit = (type + 1) * (stationtier) * (workertier);
    return profit;
}