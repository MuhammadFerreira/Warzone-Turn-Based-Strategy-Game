#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include "LoggingObserver.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../GameEngine/GameEngine.h"
#include "../Orders/Orders.h"

using std::vector;
using std::list;

//Below we will implement all the abstract functions we declared in the previous LogginObserver.h Header File

//========================================ILoggable Class======================================
//Default constructor



//========================================Subject Class======================================
//Default constructor

Subject::Subject(){
    observers = new list<Observer*>;
}


//destructor
Subject::~Subject(){
    delete observers;
}

/*
//Implementation of Add method (adds an observer to the observers list)
void Subject::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void Subject::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void Subject::Notify(){
    list<Observer*>::iterator i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}
*/

//========================================Observer Class======================================
//Destructor
Observer::Observer(){}
Observer::~Observer(){}

//========================================LogObserver Class======================================
//Default constructor (will open file)
LogObserver::LogObserver(){
    logFile.open("gamelog.txt", ios::app);
    logFile << "Opened" << endl;
}


//Copy Constructor
LogObserver::LogObserver(const LogObserver* o){
    logFile.open("gamelog.txt", ios::app);
    logFile << "Opened" << endl;
}


//Overloaded Operator
LogObserver &LogObserver::operator=(const LogObserver &o){
    if(this == &o) return *this;
    logFile.open("gamelog.txt", ios::app);
    logFile << "Opened" << endl;
    return *this;
}


//Destructor
LogObserver::~LogObserver(){
    logFile.close();
}


//Overriding the update method from Observer (will grab relevant information and update file)
void LogObserver::Update(Subject* subject){
    ILoggable* loggable = dynamic_cast<ILoggable*>(subject);
    if(loggable){
        string logEntry = loggable -> stringToLog();
        logToGameLog(logEntry);
    }
    else{
        cout << "was not loggable"<<endl;
    }
    
}

//implementing loToGameLog function (will write to file)
void LogObserver::logToGameLog(const string& logEntry){
    logFile << logEntry << endl;
}

void testLoggingObserver(){

    //redefinition is occuring between CommandProcessor and GameEngine
    //CommandProcessor calls on GameEngine, both have the same overriden classes

    //create instances of classes
    LogObserver * logObserver = new LogObserver();
    GameEngine * gameEngine = new GameEngine();
    CommandProcessor * cmdProcessor = new CommandProcessor();
    Order * order = new Order();
    OrdersList * orderList = new OrdersList();

    string input="loadmap";

    //Add the observer to the subjects
    gameEngine->Attach(logObserver);
    cmdProcessor->Attach(logObserver);
    order->Attach(logObserver);
    orderList->Attach(logObserver);
    Command* cmd = new Command();

    //Trigger actions that log entries
    cmdProcessor->saveCommand(cmd);
    gameEngine->transition(input);
    cmd->saveEffect("Loaded game map");
    order->execute();
    orderList->addOrder(new Order());
}

