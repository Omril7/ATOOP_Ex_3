//
// Created by omril on 05/29/2022.
//

#include "Vehicle.h"

void Vehicle::update(Time t) {
    if(status != "Stopped") {
        Polar_vector pv;
        pv.theta = to_radians(course);
        pv.r = static_cast<double>(getSpeed()) / 100;
        Cartesian_vector cv(pv);
        setLocation(cv.delta_x + getLocation().x, cv.delta_y + getLocation().y);
    }
}

void Vehicle::setCourse(double angle, int speed) {
    course = angle;
    setDestination(INFINITY, INFINITY);
    stringstream ss;
    ss << "Heading on course " << angle << " deg";
    status = ss.str();
    if(speed != -1) {
        setSpeed(speed);
    }
    return;
}
void Vehicle::position(double x, double y, int speed) {
    Point p(x-getLocation().x, y-getLocation().y);
    Polar_vector pv(p);
    setCourse(to_degrees(pv.theta));
    setDestination(x,y);

    stringstream ss;
    ss << "Heading to (" << x << "," << y << ")";
    status = ss.str();
    if(speed != -1) {
        setSpeed(speed);
    }
    return;
}
void Vehicle::_destination(double x, double y, string warehouse) {
    Point p(x-getLocation().x, y-getLocation().y);
    Polar_vector pv(p);
    setCourse(to_degrees(pv.theta));
    setDestination(x,y);

    stringstream ss;
    ss << "Heading to " << warehouse;
    status = ss.str();
    if(speed != -1) {
        setSpeed(speed);
    }
    return;
}
void Vehicle::stop() {
    course = 0;
    setDestination(INFINITY,INFINITY);
    status = "Stopped";
    setSpeed(0);
}

void Truck::getStatus() const {
    cout << setprecision(2) << "Truck " << getName() << " at (" << getLocation().x << ", " << getLocation().y << "), "
    << status << ", Crates: " << inventory << endl;
}
void Truck::update(Time t) {
    if(status != "Stopped") {
        shared_ptr<node> dest = route.front();
        if(t.hour > dest->arrival.hour || (t.hour == dest->arrival.hour && t.minute >= dest->arrival.minute)) {
            dest->dest->add(dest->quantity);
            inventory -= dest->quantity;
            dest->quantity = 0;
            if(t.hour > dest->departure.hour || (t.hour == dest->departure.hour && t.minute >= dest->departure.minute)) {
                route.pop();
                check = true;
            }
        }
        if(check) {
            _destination(route.front()->dest->getLocation().x, route.front()->dest->getLocation().y, route.front()->dest->getName());
            double distance = getLocation().distance(getDestination()) * 10;
            int hDistance = route.front()->departure.hour - route.front()->arrival.hour;
            int s = static_cast<int>(distance / hDistance);
            setSpeed(s);
            check = false;
        }
        Polar_vector pv;
        pv.theta = to_radians(course);
        pv.r = static_cast<double>(getSpeed()) / 100;
        Cartesian_vector cv(pv);
        setLocation(cv.delta_x + getLocation().x, cv.delta_y + getLocation().y);
    }
}

//State_trooper::State_trooper(string name, string src_warehouse) {}
void State_trooper::getStatus() const {
    cout << setprecision(2) << "State_trooper " << getName() << " at (" << getLocation().x << ", " << getLocation().y << "), "
    << status << ", speed " << getSpeed() << " km/h" << endl;
}

//Chopper::Chopper(string name, float xPos, float yPos) {}
void Chopper::getStatus() const {
    cout << setprecision(2) << "Chopper " << getName() << " at (" << getLocation().x << ", " << getLocation().y << "), "
    << status << ", speed " << getSpeed() << " km/h" << endl;
}
void Chopper::attack(string attackedTruck) {} /// args will be different