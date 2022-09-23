#include <iostream>
#include <fstream>
#include "ocp/OCPBuilderAL.hpp"
#include <string>
using namespace fatrop;

int main(int argc, char **argv)
{
    // if (argc == 3)
    if (true)
    {
        
        #ifdef ENABLE_MULTITHREADING
            cout << "Multithreading enabled" << endl;
        #else
            cout << "Multithreading disabled" << endl;
        #endif

        // OCPBuilder ocpbuilder(argv[1], argv[2]);
        // OCPBuilderAL ocpbuilder("../robot.so","../robot.json");
        OCPBuilderAL ocpbuilder("../Rocket_example.so","../Rocket_example.json");

        ocpbuilder.fatropalg->Optimize();
    }
    else
    {
        cout << "run me as RunFatrop f.so f.json" << endl;
    }
}
