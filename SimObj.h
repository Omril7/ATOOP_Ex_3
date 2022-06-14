//
// Created by omril on 06/01/2022.
//

#ifndef EXERCISE_3_SIMOBJ_H
#define EXERCISE_3_SIMOBJ_H

#include "Geometry.h"
#include <queue>
#include <memory>

using namespace std;

struct Time {
    int hour;
    int minute;
    Time() : hour(0), minute() {}
    Time(int h) : hour(h), minute(0) {}
};

class SimObj {
protected:
    string name;
    Point location;
public:
    SimObj(string n) { name = n; }
    virtual void update(Time t) = 0;

    string getName() const { return name; }
    Point getLocation() const { return location; }
    void setName(string name) { this->name = name; } // NOT NEEDED
    void setLocation(double x, double y) { // maybe void setLocation(Point p);
        location.x = x;
        location.y = y;
    }
    virtual void getStatus() const = 0;             // <name> <location> <status>

    void broadcastCurrentState(); /// ?????????
};


#endif //EXERCISE_3_SIMOBJ_H
