#include "Controller.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Set output to show two decimal places
    cout.setf(ios::fixed, ios::floatfield);
//    cout.precision(2);

    Controller controller;
    controller.run(argc, argv);

    return 0;
}
