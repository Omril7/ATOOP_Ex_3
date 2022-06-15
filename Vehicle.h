//
// Created by omril on 05/29/2022.
//

#ifndef EXERCISE_3_VEHICLE_H
#define EXERCISE_3_VEHICLE_H

#include <vector>
#include "Warehouse.h"

using namespace std;

struct node {
    shared_ptr<Warehouse> dest;
    Time arrival;
    int quantity;
    Time departure;
};

class Vehicle : public SimObj {
public:
    Vehicle(string name) : SimObj(name) {}
    virtual void update(Time t);

    void setCourse(double angle, int speed = -1);           // status = "Moving on course"
    void position(double x, double y, int speed = -1);      // status = "Moving to (x,y)"
    void _destination(double x, double y, string warehouse); // status = "Moving to warehouse

    void stop();
    int getSpeed() const { return speed; }
    void setSpeed(int s) { speed = s; }
    void setDestination(double x, double y) { destination.x = x; destination.y = y; }
    Point getDestination() const { return destination; }
    virtual void getStatus() const = 0;             // <name> <location> <status>
    virtual void addNode(shared_ptr<node> np) = 0;
    virtual void setInventory(int i) = 0;

protected:
    string status = "Stopped"; // {"Stopped","parked","Off road","Moving to.."}
    int speed = 90;
    double course;      // in degrees
    Point destination;  // where to
};

class Truck : public Vehicle {
public:
    Truck(string name, double x, double y) : Vehicle(name) {
        setSpeed(90);
        setLocation(x, y);
        status = " ";
    }
    void getStatus() const;                              // <name> <location> <status>
    virtual void update(Time t);
    void addNode(shared_ptr<node> np) { route.push(np); }
    virtual void setInventory(int i) { inventory = i; }
private:
    bool check = true;
    int inventory;
    queue<shared_ptr<node> > route;
};

class State_trooper : public Vehicle {
public:
    State_trooper(string name, double x, double y) : Vehicle(name) { // speed = 90
        setSpeed(90);
        setLocation(x, y);
    }

    void getStatus() const;
    virtual void addNode(shared_ptr<node> np) {}
    virtual void setInventory(int i) {}
};

class Chopper : public Vehicle {
public:
    Chopper(string name, double xPos, double yPos) : Vehicle(name) {
        setLocation(xPos,yPos);
    }

    void getStatus() const;
    void attack(string attackedTruck);
    virtual void addNode(shared_ptr<node> np) {}
    virtual void setInventory(int i) {}
private:
    int attackRange = 2;
};


#endif //EXERCISE_3_VEHICLE_H
