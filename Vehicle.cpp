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
    double angle = to_degrees(pv.theta) + 90;
    if( angle >= 360 ) {
        angle -= 360;
    }
    setCourse(angle);
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
    double angle = to_degrees(pv.theta);
    setCourse(angle);
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
void Vehicle::offRoad() {
    stop();
    status = "Off road";
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

void State_trooper::getStatus() const {
    cout << setprecision(2) << "State_trooper " << getName() << " at (" << getLocation().x << ", " << getLocation().y << "), "
    << status << ", speed " << getSpeed() << " km/h" << endl;
}
void State_trooper::update(Time t) {
    if(status != "Stopped") {
        double next_r = static_cast<double>(getSpeed()) / 100;
        double dist = getLocation().distance(getDestination());
        Polar_vector pv;
        if(next_r > dist) {
            route.push(route.front());
            route.pop();
            _destination(route.front()->getLocation().x, route.front()->getLocation().y, route.front()->getName());
        }
        pv.theta = to_radians(course);
        pv.r = next_r;
        Cartesian_vector cv(pv);
        setLocation(cv.delta_x + getLocation().x, cv.delta_y + getLocation().y);
    }
}
void State_trooper::setRoute(vector<shared_ptr<Warehouse> > warehouses) {
    vector<bool> chosen(warehouses.size());
    for(int i=0; i<static_cast<int>(chosen.size()); i++) {
        chosen[i] = false;
    }
    int curr;
    int next;
    for(int i=0; i<static_cast<int>(warehouses.size()); i++) {
        if(warehouses[i]->getLocation() == getLocation()) {
            route.push(warehouses[i]);
            chosen[i] = true;
            curr = i;
        }
    } // init first Point

    for(int i=0; i<static_cast<int>(warehouses.size()) - 1; i++) {
        next = findClosestPointIndex(warehouses, chosen, curr);
        route.push(warehouses[next]);
        chosen[next] = true;
        curr = next;
    }

    route.push(route.front());
    route.pop();

    _destination(route.front()->getLocation().x, route.front()->getLocation().y, route.front()->getName());
}
int State_trooper::findClosestPointIndex(vector<shared_ptr<Warehouse> > warehouses, vector<bool> chosen, int curr) {
    int idx;
    double minDist = INFINITY;
    for(int i=0; i<static_cast<int>(warehouses.size()); i++) {
        if(chosen[i] || i == curr) {
            continue;
        }
        else if(minDist > warehouses[curr]->getLocation().distance(warehouses[i]->getLocation())) {
            idx = i;
            minDist = warehouses[curr]->getLocation().distance(warehouses[i]->getLocation());
        }
    }
    return idx;
}

void Chopper::getStatus() const {
    cout << setprecision(2) << "Chopper " << getName() << " at (" << getLocation().x << ", " << getLocation().y << "), "
    << status << ", speed " << getSpeed() << " km/h" << endl;
}
void Chopper::attack(shared_ptr<Vehicle> truck, vector<shared_ptr<Vehicle> > v_list) {
    if(getLocation().distance(truck->getLocation()) >  attackRange ) {
        return;
    }
    for(int i=0; i<static_cast<int>(v_list.size()); i++) {
        if(v_list[i]->getType() == state_trooper) {
            if(getLocation().distance(v_list[i]->getLocation()) > 10) {
                // success
                truck->offRoad();
                attackRange++;
            }
            else {
                attackRange--;
            }
            stop();
        }
    }
}