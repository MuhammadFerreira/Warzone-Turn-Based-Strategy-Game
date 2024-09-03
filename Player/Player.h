#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../GameEngine/GameEngine.h"

using namespace std;

class Territory;
class Hand;
class Card;
class Order;
class OrdersList;

class Player {
public:
  Player(); // default constructor
  Player(string name, vector<Territory*> territories, Hand* cards,
    OrdersList* orders, int reinforcements, bool isNeutral); // constructor
  Player(string name, bool isNeutral);
  Player(const Player &p);            // copy constructor
  ~Player();                          // destructor
  Player &operator=(const Player &p); // assignment operator
  friend ostream &operator<<(ostream &out,
                             const Player &p); // stream insertion operator

  string getName();
  void setName(string name);


  vector<Territory *> toDefend(); // Territories to be defended
  vector<Territory *> toAttack(); // Territories to be attacked

  Order *issueOrder(string &o); // Issue order (string)

  void addTerritory(Territory* territory); // Add territory to player

  Hand* getHand(); // get the hand of the player;

  OrdersList *getOrdersList(); // Get order list

  vector<Territory *> getTerritories(); // get the territories of the player

  int getReinforcements() const;
  void setReinforcements(int army);

  OrdersList* getOrders();

  vector<Player*> getNegotiatingWith();
  void addNegotiator(Player* p);
  void resetNegotiators();

  // All data members of user-defined class type must be of pointer type.
private:
  string name;
  vector<Territory *> territories;
  Hand* cards;
  OrdersList* orders;
  int reinforcements;
  bool isNeutral;
  bool canPickUp;
  bool isFirstTurn;
public:
    bool getIsNeutral() const;
    void setIsNeutral(bool isNeutral);
    void setCanPickUp(bool canPickUp);
    bool getCanPickUp();

    bool getIsFirstTurn() const;
    void setIsFirstTurn(bool isFirstTurn);
    void setTerritories(std::vector<Territory*> territories);

    vector<Player*> negotiatingWith;
};

void capitalizeFirstLetter(string &s);

// commented this out because of outside class usage
//inline void testPlayers() {
//    // Test constructor
//    vector<string *> territories;
//    string *t1 = new string("T1");
//    string *t2 = new string("T2");
//    territories.push_back(t1);
//    territories.push_back(t2);
//
//    vector<string *> cards;
//    OrdersList* orders = new OrdersList();
//    Player *p2 = new Player("Player 2", territories, cards, orders);
//    cout << "Player constructor used.\n" << *p2 << endl;
//
//    // Test copy constructor
//    Player *p3 = new Player(*p2);
//    cout << "Player copy constructor used.\n" << *p3 << endl;
//
//    // Test destructor
//    delete p2;
//
//    // Test assignment operator
//    Player *p4 = new Player();
//    p4 = p3;
//
//    cout << "Assigning new player to Player 3.\n" << *p4 << endl;
//
//    // Test setName()
//    p3->setName("Jake");
//    cout << "Set Player 3 name to Jake" << endl;
//
//    // Test getName()
//    cout << "Get Player 3 name: " << p3->getName() << endl;
//
//    // Test toDefend()
//    cout << "Player 3 territories to defend: " << endl;
//    for (auto *terr : p3->toDefend()) {
//        cout << *terr << endl;
//    }
//
//    // Test toAttack()
//    cout << "Player 3 territories to attack: " << endl;
//    for (auto *terr : p3->toAttack()) {
//        cout << *terr << "\n";
//    }
//
//    // Test issueOrder()
//    string order = "Deploy";
//    p3->issueOrder(order);
//    cout << "Added Deploy order. Updated player:\n" << *p3 << endl;
//
////  delete orders;
//    delete p3;
//}