//
// Created by omril on 06/01/2022.
//

#ifndef EXERCISE_3_WAREHOUSE_H
#define EXERCISE_3_WAREHOUSE_H

#include "SimObj.h"

class Warehouse : public SimObj {
public:
    Warehouse(string name, double x, double y, int invent) : SimObj(name), inventory(invent) {
        setLocation(x,y);
    }
    void add(int boxes) { inventory += boxes; }
    void take(int boxes) { inventory -= boxes; }
    virtual void update(Time t);
    virtual void getStatus() const;             // <name> <location> <status>
private:
//    queue<node> route;
    int inventory;
};


#endif //EXERCISE_3_WAREHOUSE_H
