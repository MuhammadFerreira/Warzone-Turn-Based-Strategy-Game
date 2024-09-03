#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "GameEngine.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Cards/Cards.h"
#include "../Orders//Orders.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../GameLogObserver/LoggingObserver.h"

GameEngine::GameEngine() : Subject(),
                            _currentState(GameState::Start),
                           _gameMap(new Map()),
                           _commandProcessor(new CommandProcessor()),
                           _gameDeck(new Deck(5, 5, 5, 5, 5))
{
}

GameEngine::GameEngine(CommandProcessor *processor) : Subject(), _currentState(GameState::Start),
                                                      _gameMap(new Map()),
                                                      _commandProcessor(processor),
                                                      _gameDeck(new Deck(5, 5, 5, 5, 5))
{
}

GameEngine::GameEngine(const GameEngine &game)
{
  this->_currentState = game._currentState;
  this->_gameMap = game._gameMap;
  this->_gamePlayers = game._gamePlayers;
  this->_commandProcessor = game._commandProcessor;
  this->_gameDeck = game._gameDeck;
}

GameEngine &GameEngine::operator=(const GameEngine &game) = default;

std::ostream &operator<<(std::ostream &out, const GameEngine &game)
{
  out << "GameEngine:" << endl;
  out << "Game is in state: " << GameEngine::GameStateToString(game._currentState) << std::endl;
  out << "Map:" << *(game._gameMap) << endl;
  out << "Players:" << endl;
  for (auto player : game._gamePlayers)
  {
    out << "\t" << player->getName() << endl;
  }
  out << "Deck:" << endl;
  out << *(game._gameDeck) << endl;
  out << *(game._commandProcessor) << endl;
  return out;
}

// getters and setters
GameState GameEngine::getState()
{
  return _currentState;
}

Map *GameEngine::getMap()
{
  return this->_gameMap;
}

void GameEngine::setMap(Map *map)
{
  this->_gameMap = map;
}

std::vector<Player *> &GameEngine::getPlayers()
{
  return this->_gamePlayers;
}

void GameEngine::setPlayers(std::vector<Player *> players)
{
  this->_gamePlayers = players;
}

void GameEngine::addPlayer(Player *player)
{
  this->_gamePlayers.push_back(player);
}

Deck *GameEngine::getDeck()
{
  return this->_gameDeck;
}

void GameEngine::setDeck(Deck *deck)
{
  this->_gameDeck = deck;
}

// check the command and transition if the command was valid in the current state
GameState GameEngine::transition(const std::string &command)
{
  switch (_currentState)
  {
  case GameState::Start:
    if (command == "loadmap")
    {
      _currentState = GameState::MapLoaded;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::MapLoaded:
    if (command == "loadmap")
    {
      _currentState = GameState::MapLoaded;
    }
    else if (command == "validatemap")
    {
      _currentState = GameState::MapValidated;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::MapValidated:
    if (command == "addplayer")
    {
      _currentState = GameState::PlayersAdded;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::PlayersAdded:
    if (command == "addplayer")
    {
      _currentState = GameState::PlayersAdded;
    }
    else if (command == "assigncountries")
    {
      _currentState = GameState::AssignReinforcement;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::AssignReinforcement:
    if (command == "issueorder")
    {
      _currentState = GameState::IssueOrders;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::IssueOrders:
    if (command == "issueorder")
    {
      _currentState = GameState::IssueOrders;
    }
    else if (command == "endissueorders")
    {
      _currentState = GameState::ExecuteOrders;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::ExecuteOrders:
    if (command == "execorder")
    {
      _currentState = GameState::ExecuteOrders;
    }
    else if (command == "endexecuteorders")
    {
      _currentState = GameState::AssignReinforcement;
    }
    else if (command == "win")
    {
      _currentState = GameState::Win;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  case GameState::Win:
    if (command == "end")
    {
      _currentState = GameState::End;
    }
    else if (command == "play")
    {
      _currentState = GameState::Start;
    }
    else
      std::cout << "Invalid command: " << command << " in state " << GameStateToString(_currentState) << std::endl;
    break;
  default:
    std::cout << "Invalid State";
    break;
  }
  //Notify from Subject called
  Notify();
  return _currentState;
}

// Removes players without any territories
void GameEngine::removeDead()
{
    this->_gamePlayers.erase(std::remove_if(_gamePlayers.begin(),
                                            _gamePlayers.end(),
                                            [](Player *p) { return p->getTerritories().empty();}),
                             _gamePlayers.end());
}

// Checks if a player has won
bool GameEngine::checkWin() {
    // Count number of neutral players (should always be 1)
    int numNeutralPlayers = count_if(_gamePlayers.begin(), _gamePlayers.end(), [](Player* p) {return p->getIsNeutral();});

    // Only one non-neutral player left
    if (this->_gamePlayers.size() - numNeutralPlayers == 1)
    {
        Player* finalPlayer = this->_gamePlayers.front();

        // Ensure that player owns all the territories
        for (auto territory : this->_gameMap->getAllTerritories())
        {
            // That player doesn't own all territories... Something went wrong!
            if (territory->getTerritoryOwner() != finalPlayer)
                return false;
        }

        std::cout << "Player " << finalPlayer->getName() << " won!" << endl;
        return true;
    }
    return false;
}

// Reinforcement phase
void GameEngine::reinforcementPhase()
{
  std::cout << "\n--------- Reinforcement Phase ----------\n" << endl;
    for (auto player : _gamePlayers)
  {
    int unitsToAdd = 0;
    cout << "Player " << player->getName() << " is reinforcing." << endl;
    // Give players units based on the number of territories they own
    int numTerritoryReinforcements = floor(player->getTerritories().size() / 3);
    cout << "\t" << player->getName() << " has gained " << numTerritoryReinforcements << " units for territories they own." << endl;
    unitsToAdd += numTerritoryReinforcements;
//    player->setReinforcements(player->getReinforcements() + numTerritoryReinforcements);

    // Give players units based on the number of continents they own
    int continentBonus = 0;
    for (Continent *continent : this->_gameMap->getContinents())
    {
      if (continent->getContinentOwner() == player)
//        player->setReinforcements(player->getReinforcements() + continent->getArmyBonus());
        {
            continentBonus += continent->getArmyBonus();
            unitsToAdd += continentBonus;
        }
    }

    cout << "\t" << player->getName() << " has gained " << continentBonus << " units for continents they own." << endl;

    // Minimum of 3 units
//      if (player->getReinforcements() < 3) {
      if (player->getReinforcements() < 3) {
              cout << "\t" << player->getName() << " now has 3 reinforcements, the minimum number of reinforcements." << endl;
//              player->setReinforcements(player->getReinforcements() + 3);
          unitsToAdd = 3;
      }
    player->setReinforcements(player->getReinforcements() + unitsToAdd);
    std::cout << "TOTAL: Player " << player->getName() << " has " << player->getReinforcements() << " reinforcements.\n" << endl;
  }
}

bool allPlayersDone(const std::vector<bool> &players)
{
    return std::all_of(players.begin(), players.end(), [](bool done) {return done;});
}

// Issue order phase
void GameEngine::issueOrdersPhase()
{
  std::cout << "\n--------- Issue Orders Phase ----------\n" << endl;

  // Ensure that all players *on the same turn* don't want to issue more orders
  vector<bool> playersDone(_gamePlayers.size(), false);
  int playerIndex = 0;
  while (!allPlayersDone(playersDone))
  {
    playerIndex = 0;
    for (auto player : _gamePlayers)
    {
        cout << "\n=============== Player " << player->getName() << "'s turn ===============\n" << endl;
        // Ask player for attacking territory priority
        vector<Territory*> attackableTerritories = player->toAttack();

        // Ask player for defending territory priority
        vector<Territory*> defendableTerritories = player->toDefend();

        // Player has troops left -> they gotta use them
        if (player->getReinforcements() > 0)
        {
            std::cout << "\nYou have " << player->getReinforcements() << " reinforcements left.\n"
                    "You must deploy them before issuing any other orders.\n" << endl;
            
            // -------------------- DEPLOYING -----------------------
            std::cout << "\n***** DEPLOY ORDER ***** \n" << endl;

            std::cout << "Enter how many army units you would like to deploy to each territory.\n"
                    "You currently have " << player->getReinforcements() << " units remaining." << endl;

            for (auto territory : defendableTerritories)
            {
                int numReinforcements = player->getReinforcements();
                std::cout << territory->getTerritoryName() << "\t";

                int selectedNumUnits;
                std::cin >> selectedNumUnits;

                if (selectedNumUnits > numReinforcements)
                    selectedNumUnits = numReinforcements;
                else if (selectedNumUnits < 0)
                    selectedNumUnits = 0;

                // ! Unclear if loop should break after a single deploy order, or wait until all reinforcements are used
                cout << "Issuing order: Deploy " << selectedNumUnits << " units to " << territory->getTerritoryName() << "." << endl;
                player->setReinforcements(numReinforcements - selectedNumUnits);
                player->getOrders()->addOrder(
                        new DeployOrder(player, territory, selectedNumUnits));

                if (player->getReinforcements() <= 0)
                {
                    std::cout << "All units have been deployed." << endl;
                    break;
                }

                std::cout << player->getReinforcements() << " units remaining." << endl;

                // Remove territory that was just entered (first element in vector)
                // defendableTerritories.erase(defendableTerritories.begin());
            }
            if (player->getIsFirstTurn()) {
                playersDone[playerIndex] = true;
                player->setIsFirstTurn(false);
            }
            playerIndex++;
            continue;
        }

        // -------------------- ADVANCING -----------------------
//      stillIssuingOrders = false;
      bool orderIssued = false;
      while (!orderIssued){
          string order;
          std::cout << "Select an order to issue. Your options are:\n"
                    <<   "Advance\n" << *player->getHand() << endl;

          std::cout << "Enter order or 'Done': ";
          std::cin >> order;

          // Capitalize first letter of order, lowercase all others
          order[0] = toupper(order[0]);
          for (int i = 1; i < order.length(); i++)
              order[i] = tolower(order[i]);

          if (order == "Done") {
              playersDone[playerIndex] = true;
              playerIndex++;
              orderIssued = true;
          }
          else if (order == "Advance")
          {
              std::cout << "Your territories:" << endl;
              for (auto territory : player->getTerritories())
              {
                  std::cout << territory->getTerritoryName() << "\t" << territory->getArmyCount() << " units.\n";
              }

              std::cout << "Enter the territory to advance from: ";
              string fromTerritoryName;
              std::cin >> fromTerritoryName;
              vector<Territory*> playerTerritories = player->getTerritories();
              while (std::all_of(playerTerritories.begin(),
                                 playerTerritories.end(),
                                 [&fromTerritoryName](Territory* terr) {
                                     return terr->getTerritoryName() != fromTerritoryName;
                                 })) {
                  std::cout << "Could not find that territory. Please try again:";
                  std::cin >> fromTerritoryName;
                  std::cout << endl;
              }

              // Get attackable territories *adjacent to* the territory to advance from
              std::vector<Territory*> adjacentFromTerritories;

              // Get the territory object from the player's territories

              auto fromTerritoryIter = std::find_if(playerTerritories.begin(),
                                                    playerTerritories.end(),
                                                    [&fromTerritoryName](Territory* terr) {
                                                        return terr->getTerritoryName() == fromTerritoryName;
                                                    });

              if (fromTerritoryIter != playerTerritories.end())
              {
                  adjacentFromTerritories = (*fromTerritoryIter)->getAdjTerritories();
              }

              // Filter adjacent territories by only enemy territories
              std::vector<Territory*> adjacentEnemyTerritories;
              std::copy_if(adjacentFromTerritories.begin(),
                           adjacentFromTerritories.end(),
                           std::back_inserter(adjacentEnemyTerritories),
                           [&player](Territory* terr) {
                               return terr->getTerritoryOwner() != player;
                           });

              if (adjacentEnemyTerritories.empty())
              {
                  std::cout << "No territories can be attacked." << endl;
              }
              else {
                  std::cout << "Territories you can attack:" << endl;
                  for (auto attTerr: adjacentEnemyTerritories) {
                      std::cout << attTerr->getTerritoryName() << endl;
                  }
              }
              vector<Territory*> adjacentFriendlyTerritories;
              std::copy_if(adjacentFromTerritories.begin(),
                           adjacentFromTerritories.end(),
                           std::back_inserter(adjacentFriendlyTerritories),
                           [&player, &fromTerritoryName](Territory* terr) {
                               return terr->getTerritoryOwner() == player && terr->getTerritoryName() != fromTerritoryName;
                           });

              if (adjacentFriendlyTerritories.empty())
              {
                  std::cout << "No territories to move to." << endl;
              }
              else {
                  std::cout << "Territories you can move to:" << endl;
                  for (auto defTerr: adjacentFriendlyTerritories) {
                      std::cout << defTerr->getTerritoryName() << endl;
                  }
              }

              // Ask for which territory to advance to
              std::cout << "Enter the territory to advance to: ";
              string toTerritoryName;
              std::cin >> toTerritoryName;

              while (!std::any_of(attackableTerritories.begin(),
                                  attackableTerritories.end(),
                                  [&toTerritoryName](Territory* terr) {
                                      return terr->getTerritoryName() == toTerritoryName;
                                  }) && !std::any_of(defendableTerritories.begin(),
                                                     defendableTerritories.end(),
                                                     [&toTerritoryName](Territory* terr) {
                                                         return terr->getTerritoryName() == toTerritoryName;
                                                     }))
              {
                  std::cout << "Invalid territory. Please try again:";
                  std::cin >> toTerritoryName;
                  std::cout << endl;
              }

              auto gameTerritories = _gameMap->getAllTerritories();
              auto fromTerritory = find_if(gameTerritories.begin(), gameTerritories.end(), [&fromTerritoryName](const Territory& obj) {return obj.getTerritoryName() == fromTerritoryName;})[0];
              auto toTerritory = find_if(gameTerritories.begin(), gameTerritories.end(), [&toTerritoryName](const Territory& obj) {return obj.getTerritoryName() == toTerritoryName;})[0];

              std::cout << "The source territory currently has " << fromTerritory->getArmyCount() << " army units." << endl;
              std::cout << "Enter the amount of army units you will advance with: ";
              int numAdvanceArmies;
              std::cin >> numAdvanceArmies;
              std::cout << endl;

              while (numAdvanceArmies > fromTerritory->getArmyCount() || numAdvanceArmies < 0)
              {
                  std::cout << "Invalid number of armies. Please try again: ";
                  std::cin >> numAdvanceArmies;
                  std::cout << endl;
              }
              std::cout << "Issued advance order to advance from " << fromTerritoryName << " to " << toTerritoryName << "." << endl;
              player->getOrders()->addOrder(new AdvanceOrder(player, fromTerritory, toTerritory, numAdvanceArmies));
              playerIndex++;
              orderIssued = true;
          }
          else if (order == "Bomb" || order == "Reinforcement" || order == "Blockade" || order == "Airlift" || order == "Diplomacy")
          {
              order[0] = tolower(order[0]);
              if (player->getHand()->play(this->_gameDeck, order)) {
                  order[0] = toupper(order[0]);
                  // Diplomacy card runs negotiate order
                  if (order == "Diplomacy") order = "Negotiate";
                  player->getOrders()->createOrder(order, player, _gamePlayers);
                  std::cout << "Issued order: " << order << endl;
                  orderIssued = true;

                  playerIndex++;
              }
          }
          else
              cout << "Invalid Command, try again." << endl;
      }
    }
  }
}

// Execute order phase
void GameEngine::executeOrdersPhase()
{
  std::cout << "\n--------- Execute Orders Phase ----------\n" << endl;
  bool stillExecutingOrders = true;
  while (stillExecutingOrders)
  {
    stillExecutingOrders = false;
    vector<Order*> unexecutedOrders;
    vector<Order*> deployOrders;

    for (auto player : _gamePlayers)
    {
        vector<Order*> playerOrders = player->getOrdersList()->getOrders();

        // Get unexecuted orders for all player's orders
        std::copy_if(playerOrders.begin(), playerOrders.end(), std::back_inserter(unexecutedOrders),
                     [](Order* ord) {return !ord->getIsExecuted();});

        // Copy deploy orders to new deployOrders list
        std::copy_if(unexecutedOrders.begin(), unexecutedOrders.end(), std::back_inserter(deployOrders), [](Order* ord){return ord->getOrderType() == "Deploy";});

        // Remove deploy orders from unexecuted orders
        unexecutedOrders.erase(std::remove_if(unexecutedOrders.begin(), unexecutedOrders.end(), [](Order* ord){return ord->getOrderType() == "Deploy";}), unexecutedOrders.end());
    }

      // Execute deploy orders before anything else!
      for (auto deployOrder : deployOrders)
      {
          if (deployOrder != nullptr)
          {
              deployOrder->execute();
              stillExecutingOrders = true;
          }
      }

      // Execute all other (unexecuted) orders
      for (auto order : unexecutedOrders) {
          if (order != nullptr)
          {
              order->execute();
              stillExecutingOrders = true;
          }
      }
  }
}

std::string GameEngine::GameStateToString(GameState state)
{
  switch (state)
  {
  case GameState::Start:
    return "Start";
  case GameState::MapLoaded:
    return "MapLoaded";
  case GameState::MapValidated:
    return "MapValidated";
  case GameState::PlayersAdded:
    return "PlayersAdded";
  case GameState::AssignReinforcement:
    return "AssignReinforcement";
  case GameState::IssueOrders:
    return "IssueOrders";
  case GameState::ExecuteOrders:
    return "ExecuteOrders";
  case GameState::Win:
    return "Win";
  case GameState::End:
    return "End";
  default:
    return "undefined state";
  }
}

// startup phase for game,
// need to: Load map, validate map, get players, shuffle players, assign territories and add cards to each player
void GameEngine::startupPhase()
{
  int emptyCmdCnt = 0;
  string cmdString;
  // get loadmap command
  bool cmdGood = false;
  Command *currCmd;

  while (_currentState != GameState::AssignReinforcement)
  {
    // 4 possible states before main game loop: Start, MapLoaded, MapValidated, PlayersAdded
    cmdGood = false;
    currCmd = _commandProcessor->getCommand();
    cmdGood = _commandProcessor->validate(*currCmd, _currentState);
    // check for empty commands, if we get stuck constantly reading, we should exit the program
    if (currCmd->getCommandString().empty())
    {
      emptyCmdCnt++;
      currCmd->saveEffect("Empty Command, nothing happened");
      if (emptyCmdCnt >= 10)
      {
        cerr << "ERROR: Too many empty commands in a row, quitting...";
        exit(1);
      }
    }
    else
      emptyCmdCnt = 0;

    switch (_currentState)
    {
    case GameState::Start:
      // can only load map
      if (cmdGood)
      {
        auto strStream = stringstream(currCmd->getCommandString());
        getline(strStream, cmdString, ' '); // this should be loadmap, we'll add checking logic so that it looks the same in every state
        if (cmdString == "loadmap")
        {
          getline(strStream, cmdString); // this should be the map path
          _gameMap->loadMapFile(cmdString);
          currCmd->saveEffect("Loaded Game Map");
          cout << "Loaded Game Map" << endl;
          this->transition("loadmap");
        }
      }
      break;
    case GameState::MapLoaded:
      // can load map or validate map
      if (cmdGood)
      {
        auto strStream = stringstream(currCmd->getCommandString());
        getline(strStream, cmdString, ' '); // this should be loadmap, or validatemap
        if (cmdString == "loadmap")
        {
          getline(strStream, cmdString); // this should be the map path
          _gameMap->loadMapFile(cmdString);
          currCmd->saveEffect("Loaded Game Map");
          cout << "Loaded Game Map" << endl;
          this->transition("loadmap");
        }
        else if (cmdString == "validatemap")
        {
          bool validated = false;
          validated = _gameMap->validate();
          if (validated)
          {
            currCmd->saveEffect("Validated Map");
            cout << "Validated Map" << endl;
            this->transition("validatemap");
          }
        }
      }
      break;
    case GameState::MapValidated:
      // can only add players
      if (cmdGood)
      {
        auto strStream = stringstream(currCmd->getCommandString());
        getline(strStream, cmdString, ' '); // this should be addplayer
        if (cmdString == "addplayer")
        {
          getline(strStream, cmdString); // this should be the player name
          this->addPlayer(new Player(cmdString, false));
          currCmd->saveEffect("Added Player");
          cout << "Added Player " << cmdString << endl;
          this->transition("addplayer");
        }
      }
      break;
    case GameState::PlayersAdded:
      // can add another player or start game (gamestart)
      if (cmdGood)
      {
        auto strStream = stringstream(currCmd->getCommandString());
        getline(strStream, cmdString, ' '); // this should be addplayer or gamestart
        if (cmdString == "addplayer")
        {
          getline(strStream, cmdString); // this should be the player name
          this->addPlayer(new Player(cmdString, false));
          currCmd->saveEffect("Added Player");
          cout << "Added Player " << cmdString << endl;
          this->transition("addplayer");
        }
        else if (cmdString == "gamestart")
        {
          cout << "Starting Game!" << endl;
          // shuffle players, give each player 50 army, evenly distribute territories and give each player 2 cards
          std::random_device rd;
          auto rng = std::default_random_engine(rd());
          vector<Territory *> shuffledMap = _gameMap->getAllTerritories();
          std::shuffle(shuffledMap.begin(), shuffledMap.end(), rng); // this method mutates the passed in vector
          int playerIndex = 0;

          for (auto territory : shuffledMap)
          { // distribute territories
            if (playerIndex >= _gamePlayers.size())
              playerIndex = 0;
            territory->setTerritoryOwner(_gamePlayers[playerIndex]);
//            _gamePlayers[playerIndex]->addTerritory(territory);
            playerIndex++;
          }

          for (auto player : _gamePlayers)
          { // give players 50 reinforcements and add 2 cards
            player->setReinforcements(50);
            _gameDeck->draw(player->getHand());
            _gameDeck->draw(player->getHand());
          }
          // shuffle players
          std::shuffle(_gamePlayers.begin(), _gamePlayers.end(), rng);

          currCmd->saveEffect("Distributed territories to players, added reinforcements and cards and shuffled players");
          this->transition("assigncountries");
        }
      }
      break;
    default:
      break;
    }
  }
//  this->playPhase();
}

void GameEngine::playPhase()
{
    // change any neutral territories back to the player's
//    for(Territory *t : _gameMap->getAllTerritories())
//    {
//        if(t->getTerritoryOwner()->getIsNeutral())
//        {
//            t->setTerritoryOwner(t->getPreviousOwner());
//        }
//    }
//
//    for (auto player : _gamePlayers)
//    {
//        player->resetNegotiators();
//
//        if(player->getCanPickUp())
//        {
//            // pick up card
////            player->getHand()->getCard()
//        }
//    }

    while (_currentState != GameState::Win)
    {
        _gameMap->updateAllContinentOwners();
        switch(_currentState)
        {
            case GameState::AssignReinforcement:
                reinforcementPhase();
                this->transition("issueorder");
                break;
            case GameState::IssueOrders:
                issueOrdersPhase();
                this->removeDead();
                this->transition("endissueorders");
                break;
            case GameState::ExecuteOrders:
                executeOrdersPhase();
                this->removeDead();

                // If a player won, transition to win state
                if (this->checkWin())
                    this->transition("win");
                else
                    this->transition("endexecuteorders");
                break;
            default:
                break;
        }
    }
    std::cout << "Game over!" << endl;
}

//Overriden stringToLog method fromILoggable
string GameEngine::stringToLog() {
    string log = "Observing Game State Transition Phase information is: \n "+GameStateToString(_currentState);
    return log;
}

//Implementation of Attach method (removes an observer from the observers list)
void GameEngine::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void GameEngine::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void GameEngine::Notify(){
    auto i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}






// TEST METHODS HERE

void testGameStates() {
    using namespace std;

    GameEngine* gameEngine = new GameEngine();
    string input;

    while(1) {
        std::cout << *gameEngine << endl;
        if (gameEngine->getState() == GameState::End) {
            return;
        }
        std::cout << "Enter transition command: ";
        std::cin >> input;
        gameEngine->transition(input);
    }

    delete gameEngine;
    gameEngine = nullptr;
}

void testStartupPhase(CommandProcessor* cmdP) {
    using namespace std;

    GameEngine* gameEngine = new GameEngine(cmdP);

    gameEngine->startupPhase();
    std::cout << *gameEngine << endl;
}

void testMainGameLoop(CommandProcessor* cmdP)
{
  // Player receives correct number of army units in reinforcement phase

  std::cout << "Testing main game loop\n" << endl;

  GameEngine *gameEngine = new GameEngine(cmdP);
  
  std::cout << "Startup phase\n" << endl;
  gameEngine->startupPhase();

  std::cout << "Play phase\n" << endl;
  gameEngine->playPhase();

  // Player can *only* use deploy orders if they have army units left
  // Player can issue advance (defend or attack) orders based on toDefend and toAttack lists
  // Player can play cards to issue orders
  // Player that does not control any territory is removed from the game
  // Game ends when one player controls all territories
}