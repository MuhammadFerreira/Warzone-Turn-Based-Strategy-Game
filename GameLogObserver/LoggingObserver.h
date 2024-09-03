#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <list>
#include <iterator>
#include <string>

using namespace std;

//An Interface ILoggable. This interface will be inherited by all classes that can be the subject
//of the logging mechanism
#ifndef I_LOGGABLE_HPP
#define I_LOGGABLE_HPP
class ILoggable{
    public:
        //virtual ~ILoggable();
        //Will create and return a string to be output to the log file
        virtual string stringToLog()=0;
};
#endif

//#ifndef SUBJECT
//#define SUBJECT
class Observer;

//Subject Class will have a list of Observers and methods to add/remove and notify observers
class Subject{
    private:
        //list of Observers
        list<Observer*> *observers;

    public:
        //default contructor and destructor
        Subject();
        ~Subject();
        
        //Methods to add, remove, and notify Observers
        virtual void Attach(Observer* o)=0;
        virtual void Detach(Observer* o)=0;
        virtual void Notify()=0;
        //string stringToLog() override;

        //friend classes
        friend class GameEngine;
        friend class OrdersList;
        friend class Order;
        friend class Command;
        friend class CommandProcessor;
};
//#endif

//#ifndef OBSERVER
//#define OBSERVER
//Observer Class will have a method to update the state of the observer
class Observer{
    private:

    public:
        //destructor
        ~Observer();
        //method to update state of observer
        virtual void Update(Subject* subject) = 0;
    
    protected:
        //default constructor
        Observer();
};
//#endif


//#ifndef LOG_OBSERVER
//#define LOG_OBSERVER
class GameEngine;


//This class will override the update method to write the state of the subject to the log file
class LogObserver: public Observer{
    private:
        //file to write to
        ofstream logFile;

    public:
        LogObserver();
        //LogObserver(GameEngine* aSubjectEngine);
        ~LogObserver();
        //Copy constructor
        LogObserver(const LogObserver* o);
        //overloaded operator
        LogObserver &operator=(const LogObserver& o);
        //update method
        void Update(Subject* subject) override;

        //To display player information and current pahse information
        void logToGameLog(const string& logEntry);

};
//#endif

void testLoggingObserver();


