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

enum Type {truck, state_trooper, chopper};

class Vehicle : public SimObj {
public:
    Vehicle(string name) : SimObj(name) {}
    virtual void update(Time t);

    void setCourse(double angle, int speed = -1);
    void position(double x, double y, int speed = -1);
    void _destination(double x, double y, string warehouse);

    void stop();
    virtual void offRoad();
    int getSpeed() const { return speed; }
    void setSpeed(int s) { speed = s; }
    void setDestination(double x, double y) { destination.x = x; destination.y = y; }
    Point getDestination() const { return destination; }
    virtual void getStatus() const = 0;             // <name> <location> <status>
    virtual void addNode(shared_ptr<node> np) = 0;
    virtual void setInventory(int i) = 0;
    virtual void setRoute(vector<shared_ptr<Warehouse> > warehouses) {}
    virtual void attack(shared_ptr<Vehicle> truck, vector<shared_ptr<Vehicle> > v_list) {}
    Type getType() const { return t; }

protected:
    string status = "Stopped"; // {"Stopped","parked","Off road","Moving to.."}
    int speed = 90;
    double course;      // in degrees
    Point destination;  // where to
    Type t;
};

class Truck : public Vehicle {
public:
    Truck(string name, double x, double y) : Vehicle(name) {
        setSpeed(90);
        setLocation(x, y);
        status = " ";
        t = truck;
    }
    void getStatus() const;
    virtual void update(Time t);
    virtual void offRoad() { inventory = 0; }
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
        t = state_trooper;
    }
    virtual void setRoute(vector<shared_ptr<Warehouse> > warehouses);
    int findClosestPointIndex(vector<shared_ptr<Warehouse> > warehouses, vector<bool> chosen, int curr);
    void getStatus() const;
    virtual void update(Time t);
    virtual void addNode(shared_ptr<node> np) {}
    virtual void setInventory(int i) {}

private:
    queue<shared_ptr<Warehouse> > route;
};

class Chopper : public Vehicle {
public:
    Chopper(string name, double xPos, double yPos) : Vehicle(name) {
        setLocation(xPos,yPos);
        t = chopper;
    }

    void getStatus() const;
    virtual void attack(shared_ptr<Vehicle> truck, vector<shared_ptr<Vehicle> > v_list);
    virtual void addNode(shared_ptr<node> np) {}
    virtual void setInventory(int i) {}
private:
    int attackRange = 2;
};


#endif //EXERCISE_3_VEHICLE_H
