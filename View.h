//
// Created by omril on 06/08/2022.
//

#ifndef EXERCISE_3_VIEW_H
#define EXERCISE_3_VIEW_H

#include "Geometry.h"

class View {
public:
    View();
    void _default();
    void size(int s);            // 6 < s <= 30
    void zoom(double z);            // 100km per cell
    void pan(double x, double y);   // set (x,y) in bottom left corner
    void show() const;              // draw()
private:
    Point _pan;
    int _zoom;
    int _size;
};


#endif //EXERCISE_3_VIEW_H
