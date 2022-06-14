//
// Created by omril on 06/01/2022.
//

#ifndef EXERCISE_3_MODEL_H
#define EXERCISE_3_MODEL_H

#include "Warehouse.h"
#include "Vehicle.h"
#include "View.h"

class Model {
public:
    // Singleton access.
    static Model& getInstance();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(const Model&&) = delete;
    Model& operator=(const Model&&) = delete;

    void update(Time t);
    void status() const;

    shared_ptr<Warehouse> getWarehouse(string name) const;
    shared_ptr<Vehicle> getVehicle(string name) const;
    int getWarehouseListSize() const { return static_cast<int>(warehouse_list.size()); }
    int getVehicleListSize() const { return static_cast<int>(vehicle_list.size()); }
    bool isWarehouse(string name) const;
    bool isVehicle(string name) const;

    void addWarehouse(shared_ptr<Warehouse> w) { warehouse_list.push_back(w); }
    void addVehicle(shared_ptr<Vehicle> v) { vehicle_list.push_back(v); }
    string emptySpot(double x, double y, int zoom);

    void attach();
    void detach();

private:
    Model();
    vector<shared_ptr<Warehouse> > warehouse_list;  // list of all warehouses
    vector<shared_ptr<Vehicle> > vehicle_list;      // list of all vehicles
    shared_ptr<View> view;               /// list of all views (?)
};


#endif //EXERCISE_3_MODEL_H