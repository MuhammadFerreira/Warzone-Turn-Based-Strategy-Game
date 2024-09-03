#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "CommandProcessing.h"

// --------------- Command Class ---------------
// default constructor, uses default initialization for all members
Command::Command() = default;

// initialize only command, effect gets written later
Command::Command(std::string command): _command(command) {}

// init both command and effect
Command::Command(std::string command, std::string effect): _command(command), _effect(effect) {}

// copy constructor
Command::Command(Command& c){
    this->_command = c._command;
    this->_effect = c._effect;
}

// assignment operator, use copy constructor because assignment is trivial
Command &Command::operator=(const Command &c) = default;

// stream operator
std::ostream& operator<<(std::ostream& out, Command& command) {
    out << "Command: " << command._command << ", Effect: " << command._effect;
    return out;
}

void Command::saveEffect(std::string effect) {
    this->_effect = effect;
    //Notify from Subject called
    Notify();
}

//Overriden stringToLog() method fromILoggable
string Command::stringToLog() {
    string log = "Observing Command Phase information is: \n Command: " + _command + "\n Command Effect: " + _effect;
    return log;
}

//Implementation of Attach method (removes an observer from the observers list)
void Command::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void Command::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void Command::Notify(){
    list<Observer*>::iterator i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}

// --------------- CommandProcessor Class ---------------
// private function, reads command from console and saves to list and also outputs it;
std::string CommandProcessor::readCommand() {
    std::string input;
    std::cout << "Enter Command: ";
    std::getline(std::cin, input);
    return input;
}

Command* CommandProcessor::getCommand() {
    auto input = readCommand();
    Command* cmd = new Command(input);
    saveCommand(cmd);
    return cmd;
}

void CommandProcessor::saveCommand(Command* command) {
    commands.push_back(command);
    //Notify from Subject called
    command->Notify();
}


bool CommandProcessor::validate(Command &command, GameState currentState) {
    switch (currentState) {
        case GameState::Start: // only allowed loadmap
            if (command.getCommandString().rfind("loadmap ", 0) == 0) {// check if at start of string
                return true;
            }
            else {
                command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
                return false;
            }
        case GameState::MapLoaded: // loadmap or validatemap
            if (command.getCommandString().rfind("loadmap ", 0) == 0 || command.getCommandString() == "validatemap") {
                return true;
            }
            else {
                command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
                return false;
            }
        case GameState::MapValidated: // only add players
            if (command.getCommandString().rfind("addplayer ", 0) == 0) {// check if at start of string
                return true;
            }
            else {
                command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
                return false;
            }
        case GameState::PlayersAdded: // add players or start game
            if (command.getCommandString().rfind("addplayer ", 0) == 0 || command.getCommandString() == "gamestart") {
                return true;
            }
            else {
                command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
                return false;
            }
        case GameState::Win: // replay or quit
            if (command.getCommandString() == "replay" || command.getCommandString() == "quit") {
                return true;
            }
            else {
                command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
                return false;
            }
        default:
            command.saveEffect("Invalid command in current state: " + GameEngine::GameStateToString(currentState));
            return false;
    }
}

std::ostream &operator<<(std::ostream &out, const CommandProcessor &cmdP) {
    out << "Command Processor:" << std::endl;
    for (auto command : cmdP.commands) {
        out << "\t" << *command << std::endl;
    }
    return out;
}

//Overriden stringToLog() method fromILoggable
string CommandProcessor::stringToLog() {

    string log = "Observing Command Processing Phase: \n ";
    return log;
}

//Implementation of Attach method (removes an observer from the observers list)
void CommandProcessor::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void CommandProcessor::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void CommandProcessor::Notify(){
    list<Observer*>::iterator i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}

// --------------- FileLineReader Class ---------------
FileLineReader::FileLineReader(const std::string& path) {
    file = new std::ifstream(path);
}

void FileLineReader::loadFile(const std::string& path) {
    file = new std::ifstream(path);
}

std::string FileLineReader::readLineFromFile() {
    if(*file && file->good()){
        std::string input;
        std::getline(*file, input);
        return input;
    }
    else return "";
}

bool FileLineReader::good(){
    return file->good();
}

FileLineReader::~FileLineReader() {
    delete file;
}

// --------------- FileCommandProcessorAdapter Class ---------------
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& path): CommandProcessor() {
    reader = new FileLineReader(path);
}

std::string FileCommandProcessorAdapter::readCommand() {
    std::string input;
    if (reader->good()) {
        return reader->readLineFromFile();
    }
    return "";
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete reader;
}


void testCommandProcessor() {
    using namespace std;

    CommandProcessor* cmdP = new CommandProcessor();
    GameEngine* engine = new GameEngine();

    cout << "\n== Random command ==" << endl;
    Command* cmd = cmdP->getCommand();
    cout << *cmd << endl;

    bool result = cmdP->validate(*cmd, engine->getState());
    cout << "Validation Result: " << result << ", Command After Validation " << *cmd << endl;

    cout << "\n==Test correct command==" << endl;
    while (!result){
        cmd = cmdP->getCommand();
        result = cmdP->validate(*cmd, engine->getState());
        cout << *cmd << endl;
    }
    if (cmd->getCommandString().rfind("loadmap ", 0) == 0) {
        engine->transition("loadmap");
        cmd->saveEffect("Loaded game map");
    }

    cout << "Game engine after valid command: " << *engine << endl;
    cout << "Validation Result: " << result << ", Command After Validation " << *cmd << endl;

    delete cmdP;
    std::string fileName;
    cout << "\n==Read From File Commands==" << endl;
    cout << "Enter file name: ";
    cin >> fileName;
    cout << fileName;

    cmdP = new FileCommandProcessorAdapter(fileName);

    while (!cmd->getCommandString().empty()) {
        cmd = cmdP->getCommand();
        cout << *cmd << endl;
        result = cmdP->validate(*cmd, engine->getState());
        cout << "Validation Result: " << result << ", Command After Validation " << *cmd << endl;
    }
}