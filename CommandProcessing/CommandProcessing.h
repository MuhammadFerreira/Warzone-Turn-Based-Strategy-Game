#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "../GameLogObserver/LoggingObserver.h"
#include "../GameEngine/GameEngine.h"

//This class will inherit from Subject and ILoggable (found in GameLogObserver)
class Command : public Subject, public ILoggable{
public:
    std::string _command;
    std::string _effect;

    // constructors
    Command();
    Command(std::string command);
    Command(std::string command, std::string effect);
    Command(Command& c);

    // getters
    std::string getEffect() const { return _effect; }
    std::string getCommandString() const { return _command; }

    // assignment operator
    Command& operator =(const Command& c);

    // stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, Command& command);

    void saveEffect(std::string effect);

    //Overriden stringToLog() method fromILoggable
    string stringToLog() override;

    void Attach(Observer* o) override;
    void Detach(Observer* o) override;
    void Notify() override;

};



//This class will inherit from Subject and ILoggable (found in GameLogObserver)
class CommandProcessor: public Subject, public ILoggable{
private:
    // read command from console in
    virtual std::string readCommand();

public:
    std::list<Command*> commands;

    // get command from readCommand function and return Command with command string
    Command* getCommand();

    // save commands into a list
    void saveCommand(Command* command);

    // stream insertion operator
    friend std::ostream &operator<<(std::ostream &out, const CommandProcessor &cmdP);

    // validate command in current state
    bool validate(Command& command, GameState currentState);
    //Overriding the stringtoLog from ILoggable
    string stringToLog() override;
    //Overriding the attach from Subject
    void Attach(Observer* o) override;
    //Overriding the detach from Subject
    void Detach(Observer* o) override;
    //Overriding the Notify from Subject
    void Notify() override;

};

class FileLineReader{
private:
    std::ifstream* file;
public:
    FileLineReader(const std::string& path);
    ~FileLineReader();
    void loadFile(const std::string& path);
    bool good();
    std::string readLineFromFile();
};

class FileCommandProcessorAdapter: public CommandProcessor{
private:
    FileLineReader* reader;
public:
    FileCommandProcessorAdapter(const std::string& path);
    ~FileCommandProcessorAdapter();
    std::string readCommand() override;
};


void testCommandProcessor();