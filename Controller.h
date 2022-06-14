//
// Created by omril on 06/04/2022.
//

#ifndef EXERCISE_3_CONTROLLER_H
#define EXERCISE_3_CONTROLLER_H

#include "View.h"
#include "Model.h"

using namespace std;

class Controller {
public:
    Controller() { view_ptr = make_shared<View>(); }
    void run(int argc, char* argv[]);

private:
    shared_ptr<View> view_ptr;
};


#endif //EXERCISE_3_CONTROLLER_H
