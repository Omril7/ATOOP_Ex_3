//
// Created by omril on 06/03/2022.
//

#ifndef EXERCISE_3_FACTORY_H
#define EXERCISE_3_FACTORY_H

#include "Warehouse.h"
#include "Vehicle.h"

using namespace std;

class Factory { /// ABSTRACT FACTORY
public:
    virtual shared_ptr<Warehouse> create(string name, float x, float y, int invent) const =0;
    virtual shared_ptr<Vehicle> create(string name, string type, double x, double y) const =0;
};

class SimObjFactory : Factory {
public:
    virtual shared_ptr<Warehouse> create(string name, float x, float y, int invent) const {
        return make_shared<Warehouse>(name, x, y, invent);
    }
    virtual shared_ptr<Vehicle> create(string name, string type, double x, double y) const {
        if(type == "State_trooper") {
            return make_shared<State_trooper>(name, x, y);
        }
        else if(type == "Chopper") {
            return make_shared<Chopper>(name, x, y);
        }
        else {
            return make_shared<Truck>(name, x, y);
        }
    }
};

#endif //EXERCISE_3_FACTORY_H
