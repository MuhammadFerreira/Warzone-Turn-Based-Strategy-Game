#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../GameLogObserver/LoggingObserver.h"

enum GameState
{
  Start,
  MapLoaded,
  MapValidated,
  PlayersAdded,
  AssignReinforcement,
  IssueOrders,
  ExecuteOrders,
  Win,
  End
};

// forward references
class Map;
class Player;
class CommandProcessor;
class Deck;

// forward references
class Map;
class Player;
class CommandProcessor;
class Deck;

class GameEngine : public Subject, public ILoggable{

  private:
    GameState _currentState;
    Map* _gameMap;
    std::vector<Player*> _gamePlayers;
    CommandProcessor* _commandProcessor;
    Deck* _gameDeck;

  public:
    // Start the game
    GameEngine();

    //Game with command processor
    GameEngine(CommandProcessor* processor);

    // copy constructor
    GameEngine(const GameEngine &game);

    // assignment operator
    GameEngine &operator=(const GameEngine &game);

    // stream insertion operator
    friend std::ostream &operator<<(std::ostream &out, const GameEngine &game);

    // Returns the current state
    GameState getState();

    Map* getMap();
    void setMap(Map* map);

    std::vector<Player*>& getPlayers();
    void setPlayers(std::vector<Player*> players);
    void addPlayer(Player* player);

    Deck* getDeck();
    void setDeck(Deck* deck);

    // Transitions from the current state to the new state with the given command if it exists. 
    // Returns the state after the transition (could be the same state as before if the command doesn't exist).
    GameState transition(const std::string& command);

    void startupPhase();

    static std::string GameStateToString(GameState state);

    // Gain new army units
    void reinforcementPhase();

    // Issue orders
    void issueOrdersPhase();

    // Execute orders
    void executeOrdersPhase();

    // Remove players without any territories
    void removeDead();

    // Check if player has won
    bool checkWin();

    void playPhase();
    //Overriden stringToLog method fromILoggable
    string stringToLog() override;
    //Overriding the attach from Subject
    void Attach(Observer* o) override;
    //Overriding the detach from Subject
    void Detach(Observer* o) override;
    //Overriding the Notify from Subject
    void Notify() override;
};

void testGameStates();

void testStartupPhase(CommandProcessor* cmdP);

void testMainGameLoop(CommandProcessor* cmdP);
// void testMainGameLoop();