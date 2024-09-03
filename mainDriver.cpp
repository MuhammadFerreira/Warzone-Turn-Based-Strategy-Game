#include <iostream>

#include "./CommandProcessing/CommandProcessing.h"
#include "./Cards/Cards.h"
#include "./GameEngine/GameEngine.h"
#include "./Map/Map.h"
#include "./Orders/Orders.h"
#include "./Player/Player.h"


using namespace std;

int main(int argc, char** argv) {
    // gameplay flags
    bool testing = false;
    bool consoleInput = true;
    string commandInputFile;

    cout << "You have entered " << argc << " arguments:" << endl;
    for (int i = 0; i < argc; ++i) {
        cout << argv[i] << "\n";
        if (strcmp(argv[i], "-console") == 0) {
            cout << "console input is on" << endl;
            consoleInput = true;
        }
        else if (strcmp(argv[i], "-test") == 0) {
            cout << "testing is on" << endl;
            testing = true;
        }
        else if (strcmp(argv[i], "-file") == 0) {
            // check that there is at least more than one argument left, if there is not, stay on console input
            if (i == argc-1) {
                cout << "No input file provided, console mode enabled" << endl;
            }
            else {
                commandInputFile = argv[i+1];
                cout << "Input file is enabled: " << commandInputFile << endl;
                consoleInput = false;
            }
        }
    }

    CommandProcessor* cmdP;
    if (consoleInput) {
        cmdP = new CommandProcessor();
    }
    else {
        cmdP = new FileCommandProcessorAdapter(commandInputFile);
    }
    if (testing) {
         cout << "============================== Command Processor ==============================" << endl;
         testCommandProcessor();
//         cout << "============================== Startup Phase ==============================" << endl;
//         testStartupPhase(cmdP);
         cout << "============================== Logging ==============================" << endl;
         testLoggingObserver();
         cout << "============================== Order ==============================" << endl;
         testOrders();
         cout << "============================== Main Game ==============================" << endl;
         testMainGameLoop(cmdP);

    }

    cout << "done" << endl;
    return 0;
}