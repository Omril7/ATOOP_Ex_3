//
// Created by omril on 06/01/2022.
//

#include "Model.h"

Model::Model() {
    warehouse_list.push_back(make_shared<Warehouse>("Frankfurt", 40.0, 10.0, 100000));
    view = make_shared<View>();
}

Model& Model::getInstance() {
    static Model m;
    return m;
}

void Model::update(Time t) {
    for(int i=0; i<getWarehouseListSize(); i++) {
        warehouse_list[i]->update(t);
    }
    for(int i=0; i<getVehicleListSize(); i++) {
        vehicle_list[i]->update(t);
    }
}
void Model::status() const {
    for(int i=0; i<getWarehouseListSize(); i++) {
        warehouse_list[i]->getStatus();
    }
    for(int i=0; i<getVehicleListSize(); i++) {
        vehicle_list[i]->getStatus();
    }
}

shared_ptr<Warehouse> Model::getWarehouse(string name) const {
    for(int i=0; i<static_cast<int>(warehouse_list.size()); i++) {
        if(warehouse_list[i]->getName() == name) {
            return warehouse_list[i];
        }
    }
    return nullptr;
}
shared_ptr<Vehicle> Model::getVehicle(string name) const {
    for(int i=0; i<static_cast<int>(vehicle_list.size()); i++) {
        if(vehicle_list[i]->getName() == name) {
            return vehicle_list[i];
        }
    }
    return nullptr;
}

bool Model::isWarehouse(string name) const {
    for(int i=0; i<static_cast<int>(warehouse_list.size()); i++) {
        if(warehouse_list[i]->getName() == name) {
            return true;
        }
    }
    return false;
}
bool Model::isVehicle(string name) const {
    for(int i=0; i<static_cast<int>(vehicle_list.size()); i++) {
        if(vehicle_list[i]->getName() == name) {
            return true;
        }
    }
    return false;
}

string Model::emptySpot(double x, double y, int zoom) {
    /* check all top right per zoom cube */
    // example : if zoom = 3  --> check 3x3 top right cube
    for(int i=0; i<static_cast<int>(warehouse_list.size()); i++) {
        double wx = warehouse_list[i]->getLocation().x;
        double wy = warehouse_list[i]->getLocation().y;
        for(int zy=0; zy<zoom; zy++) {
            for(int zx=0; zx<zoom; zx++) {
                if(wx >= x+zx && wx < x+zx+1 && wy >= y+zy && wy < y+zy+1) {
                    return warehouse_list[i]->getName();
                }
            }
        }
    }
    for(int i=0; i<static_cast<int>(vehicle_list.size()); i++) {
        double vx = vehicle_list[i]->getLocation().x;
        double vy = vehicle_list[i]->getLocation().y;
        for(int zy=0; zy<zoom; zy++) {
            for(int zx=0; zx<zoom; zx++) {
                if(vx >= x+zx && vx < x+zx+1 && vy >= y+zy && vy < y+zy+1) {
                    return vehicle_list[i]->getName();
                }
            }
        }
    }
    return ". ";
}