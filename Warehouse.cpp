//
// Created by omril on 06/01/2022.
//

#include "Warehouse.h"

void Warehouse::getStatus() const {
    cout << setprecision(2) << "Warehouse " << getName() << " at position (" << getLocation().x << ", " << getLocation().y << "), Inventory: " << inventory << endl;
}

void Warehouse::update(Time t) {
//    cout << "update warehouse: " << getName() << endl;
}