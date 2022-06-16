//
// Created by omril on 06/04/2022.
//

#include "Controller.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include "Factory.h"

double getX(string stringX) {
    double x;
    stringX.erase(remove(stringX.begin(), stringX.end(), '('), stringX.end());
    stringX.erase(remove(stringX.begin(), stringX.end(), ','), stringX.end());

    istringstream ssx(stringX);
    ssx >> x;
    return x;
}
double getY(string stringY) {
    double y;
    stringY.erase(remove(stringY.begin(), stringY.end(), ')'), stringY.end());

    istringstream ssy(stringY);
    ssy >> y;
    return y;
}
double getInt(string stringInt) {
    int i;
    istringstream s(stringInt);
    s >> i;
    return i;
}
double getDouble(string stringDouble) {
    double d;
    istringstream s(stringDouble);
    s >> d;
    return d;
}

void Controller::run(int argc, char* argv[]) {
    cout << "Start run in Controller" << endl;

    if(argc < 5) {
        cerr << "Not enough information" << endl;
        exit(-1);
    }

    SimObjFactory factory;

    ifstream warehouse_file(argv[2]);
    if (!warehouse_file) {
        cerr << "error: invalid input." << endl;
        exit(-1);
    }
    string warehouse;
    stringstream ss;
    while(getline(warehouse_file, warehouse, '\n')) {
        warehouse.erase(remove(warehouse.begin(), warehouse.end(), '('), warehouse.end());
        warehouse.erase(remove(warehouse.begin(), warehouse.end(), ')'), warehouse.end());
        warehouse.erase(remove(warehouse.begin(), warehouse.end(), ','), warehouse.end());

        ss.clear();
        ss.str(warehouse);
        vector<string> buffers;
        string buffer;

        while(getline(ss, buffer, ' ')) {
            buffers.push_back(buffer);
        }

        if(buffers.size() == 4) {
            double x, y;
            int inventory;
            istringstream ssx(buffers[1]);
            istringstream ssy(buffers[2]);
            istringstream inv(buffers[3]);
            ssx >> x;
            ssy >> y;
            inv >> inventory;
            Model::getInstance().addWarehouse(factory.create(buffers[0], x, y, inventory));
        }

    } // set warehouses

    vector<char*> truck_files;
    int count=4;
    while(count<argc) {
        ifstream file(argv[count]);
        if (!file) {
            cerr << "error: invalid input." << endl;
            exit(-1);
        }
        char temp[sizeof(argv[count])];
        strncpy(temp, argv[count], strlen(argv[count]) - 4);
        string name(temp);
        string line, buffer, src;
        getline(file, line);
        stringstream ssLine(line);
        int i = 0, t;
        while(getline(ssLine, buffer, ',')) {
            if(i == 0 && Model::getInstance().isWarehouse(buffer)) {
                src = buffer;
            }
            else {
                istringstream s(buffer);
                s >> t;
            }
            i++;
        } // set source

        double x = Model::getInstance().getWarehouse(src)->getLocation().x;
        double y = Model::getInstance().getWarehouse(src)->getLocation().y;

        shared_ptr<Vehicle> truck = factory.create(name, "Truck",  x, y);

        int inv = 0;
        int hourStart, hourEnd;
        queue<shared_ptr<node> > route;
        while(getline(file, line, '\n')) {
            i=0;
            shared_ptr<node> n = make_shared<node>();
            stringstream ss(line);
            while(getline(ss, buffer, ',')) {
                if(i==0 && Model::getInstance().isWarehouse(buffer)) {
                    n->dest = Model::getInstance().getWarehouse(buffer);
                }
                else if(i==1) {
                    string a,b;
                    stringstream temp(buffer);
                    getline(temp, a, ':');
                    istringstream issa(a);
                    issa >> n->arrival.hour;
                    getline(temp, b, ':');
                    istringstream issb(b);
                    issb >> n->arrival.minute;
                }
                else if(i==2) {
                    istringstream iss(buffer);
                    iss >> n->quantity;
                    inv += n->quantity;
                }
                else if(i==3) {
                    string a,b;
                    stringstream temp(buffer);
                    getline(temp, a, ':');
                    istringstream issa(a);
                    issa >> n->departure.hour;
                    getline(temp, b, ':');
                    istringstream issb(b);
                    issb >> n->departure.minute;
                }
                i++;
            }
            truck->addNode(n);
        }
        truck->setInventory(inv);
        Model::getInstance().getWarehouse(src)->take(inv);
        Model::getInstance().addVehicle(truck);

        count++;
    } // set trucks

    int time = 0;
    string command;
    cout << "Time 0: Enter command: ";
    getline(cin, command);
    while(command != "exit") {
        queue<string> commands;
        while(command != "go") {
            if(command == "exit") {
                exit(-1);
            }
            else if(command == "default") {
                view_ptr->_default();
            }
            else if(command == "show") {
                view_ptr->show();
            } // if the command is "show" so its shows at the moment
            else if(command == "status") {
                Model::getInstance().status();
            } // if the command is "status" so its show the status at the moment
            else {
                stringstream ssCommand(command);

                vector<string> buffers;
                string buffer;
                while(getline(ssCommand, buffer, ' ')) {
                    buffers.push_back(buffer);
                } // put in vector string by string from command

                if(buffers.size() == 2 && buffers[0] == "size") {
                    int s = getInt(buffers[1]);
                    view_ptr->size(s);
                }
                else if(buffers.size() == 2 && buffers[0] == "zoom") {
                    double z = getDouble(buffers[1]);
                    view_ptr->zoom(z);
                }
                else if(buffers.size() == 3 && buffers[0] == "pan") {
                    double x = getDouble(buffers[1]);
                    double y = getDouble(buffers[2]);
                    view_ptr->pan(x, y);
                }
                else if(buffers[0] == "create") {
                    if(Model::getInstance().isVehicle(buffers[1])) {
                        cerr << buffers[1] << " is already on map!" << endl;
                    }
                    else {
                        double x, y;
                        if(buffers.size() == 4 && buffers[2] == "State_trooper") {
                            x = Model::getInstance().getWarehouse(buffers[3])->getLocation().x;
                            y = Model::getInstance().getWarehouse(buffers[3])->getLocation().y;
                        }
                        else if(buffers.size() == 5 && buffers[2] == "Chopper") {
                            x = getX(buffers[3]);
                            y = getY(buffers[4]);
                        }
                        Model::getInstance().addVehicle(factory.create(buffers[1], buffers[2], x, y));
                    }
                }
                else { // Vehicle command
                    if(Model::getInstance().isVehicle(buffers[0])) {
                        commands.push(command);
                    }
                    else {
                        cerr << buffers[0] << "is not part of the map" << endl;
                    }
                }
            } // insert all commands to queue

            cout << "Time " << time << ": Enter command: ";
            cin.clear();
            getline(cin, command);
        }
        while(!commands.empty()) {      // start make all the command from the commands queue
            string current = commands.front();
            commands.pop();

            stringstream ssCurrent(current);

            vector<string> buffers;
            string buffer;
            while(getline(ssCurrent, buffer, ' ')) {
                buffers.push_back(buffer);
            } // put in vector string by string from command

            // Vehicle
            if(buffers.size() >= 3 && buffers[1] == "course") {
                double angle = getDouble(buffers[2]);
                if(buffers.size() == 4) {
                    double speed = getDouble(buffers[3]);
                    Model::getInstance().getVehicle(buffers[0])->setCourse(angle, speed);
                }
                else {
                    Model::getInstance().getVehicle(buffers[0])->setCourse(angle);
                }
            }
            else if(buffers.size() >= 4 && buffers[1] == "position") {
                double x = getX(buffers[2]);
                double y = getX(buffers[3]);
                if(buffers.size() == 5) {
                    double speed = getDouble(buffers[4]);
                    Model::getInstance().getVehicle(buffers[0])->position(x,y,speed);
                }
                else {
                    Model::getInstance().getVehicle(buffers[0])->position(x,y);
                }
            }
            else if(buffers.size() == 3 && buffers[1] == "destination") {
                double x = Model::getInstance().getWarehouse(buffers[2])->getLocation().x;
                double y = Model::getInstance().getWarehouse(buffers[2])->getLocation().y;
                Model::getInstance().getVehicle(buffers[0])->_destination(x,y, buffers[2]);
            }
            else if(buffers.size() == 3 && buffers[1] == "attack") {
                shared_ptr<Vehicle> chopper = Model::getInstance().getVehicle(buffers[0]);
                shared_ptr<Vehicle> truck = Model::getInstance().getVehicle(buffers[2]);

                cout << buffers[0] << " attacks " << buffers[2] << endl;
            }      /*** NEED TO IMPLEMENT ***/
            else if(buffers.size() == 2 && buffers[1] == "stop") {
                Model::getInstance().getVehicle(buffers[0])->stop();
            }

        }
        Time t(time+1);
        Model::getInstance().update(t);

        time++;
        cout << "Time " << time << ": Enter command: ";
        cin.clear();
        getline(cin, command);
    }
    exit(-1);
}



