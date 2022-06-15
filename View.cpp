//
// Created by omril on 06/08/2022.
//

#include "View.h"
#include "Model.h"

View::View() {
    _pan = Point(-10, -10);
    _zoom = 2;
    _size = 25;
}

// View Methods
void View::_default() {
    size(25);
    zoom(2);
    pan(-10,-10);
}
void View::size(int s) {
    if(s<7) {
        cerr << "ERROR: New map size is too small." << endl;
    }
    else if(s>30) {
        cerr << "ERROR: New map size is too big." << endl;
    }
    else {
        _size = s;
    }
}               // 6 < s <= 30
void View::zoom(double z) {
    if(z<1) {
        cerr << "ERROR: New map scale must be positive." << endl;
    }
    else {
        _zoom = z;
    }
}            // 100km per cell
void View::pan(double x, double y) {
    _pan.x = x;
    _pan.y = y;
}   // set (x,y) in bottom left corner
void View::show() const {
    cout << setprecision(2) << "Display size: " << _size << ", scale: " << _zoom << ", origin: (" << _pan.x << ", " << _pan.y << ")" << endl;

    double range = _size*_zoom;
    double x_curr, y_curr;
    for(int i=0; i<_size; i++) {
        y_curr = _pan.y + range - i*_zoom - _zoom;
        if(i%3 == 0) {
            if(y_curr < -9) {
                cout << setprecision(0) << y_curr << " ";
            } // 3 digits
            else if(y_curr > 9 || (y_curr < 0 && y_curr > -10)) {
                cout << setprecision(0) << " " << y_curr << " ";
            } // 2 digits
            else {
                cout << setprecision(0) << "  " << y_curr << " ";
            } // 1 digit
        }
        else {
            cout << "    ";
        }
        for(int j=0; j<_size; j++) {
            x_curr = _pan.x + j*_zoom;
            string temp = Model::getInstance().emptySpot(x_curr, y_curr, _zoom);
            if(temp == ". ") {
                cout << temp;
            }
            else {
                cout << temp[0] << temp[1];
            }
        }
        cout << endl;
    }

    cout << "  ";
    for(int i=0; i<_size; i++) {
        if(i%3 == 0) {
            x_curr = _pan.x + i*_zoom;
            if(x_curr < -9) {
                cout << setprecision(0) << " " << x_curr;
            } // 3 digits
            else if(x_curr > 9 || (x_curr < 0 && x_curr > -10)) {
                cout << setprecision(0) << " " << x_curr << " ";
            } // 2 digits
            else {
                cout << setprecision(0) << "  " << x_curr << " ";
            } // 1 digit
        }
        else {
            cout << " ";
        }
    } // last line of scales
    cout << endl;
}              // draw()