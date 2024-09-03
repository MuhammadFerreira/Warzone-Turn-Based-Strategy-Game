#include "Player.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"

using namespace std;

Player::Player()
{
  this->name = "Player 1";
  this->cards = new Hand();
  this->orders = new OrdersList();
  this->reinforcements = 0;
  this->isNeutral = true;
  this->canPickUp = false;
  this->isFirstTurn = true;
}

Player::Player(std::string name, bool isNeutral)
{
  this->name = name;
  this->cards = new Hand();
  this->orders = new OrdersList();
  this->reinforcements = 0;
  this->isNeutral = isNeutral;
  this->canPickUp = false;
  this->isFirstTurn = true;
}

Player::Player(string name, vector<Territory *> territories,
               Hand *cards, OrdersList *orders, int reinforcements, bool isNeutral)
{
  this->name = name;
  this->territories = territories;
  this->cards = cards;
  this->orders = orders;
  this->reinforcements = reinforcements;
  this->isNeutral = isNeutral;
  this->canPickUp = false;
  this->isFirstTurn = true;
}

// Copy constructor
Player::Player(const Player &p)
{
  this->name = p.name;
  this->territories = p.territories;
  this->cards = p.cards;
  this->orders = new OrdersList(*p.orders);
  this->reinforcements = p.reinforcements;
  this->isNeutral = p.isNeutral;
  this->canPickUp = p.canPickUp;
  this->isFirstTurn = p.isFirstTurn;
}

// Destructor
Player::~Player()
{
  string nameTemp = this->name;
  this->name.clear();
  this->territories.clear();

  delete this->orders;
  delete this->cards;

  cout << nameTemp << " destroyed\n"
       << endl;
}

// Assignment operator
Player &Player::operator=(const Player &p)
{
  // Check for self-assignment
  if (this != &p)
  {
    this->name = p.name;
    this->territories = p.territories;
    this->cards = p.cards;
    this->orders = new OrdersList(*p.orders);
  }

  return *this;
}

// Stream insertion operator
ostream &operator<<(ostream &out, const Player &p)
{
  out << "Player name: " << p.name << endl;
  out << "Player territories: " << endl;
  for (auto *terr : p.territories)
  {
    out << *terr << endl;
  }
  out << "Player cards: " << endl;
  out << *p.cards << endl;
  out << "Player orders: " << endl;
  out << *p.orders << endl;
  out << "Player reinforcements: " << p.reinforcements << endl;
  return out;
}

string Player::getName() { return name; }

void Player::setName(string name) { this->name = name; }

void Player::setTerritories(std::vector<Territory*> territories){
    this->territories = territories;
}

vector<Territory *> Player::toDefend()
{
    cout << "Please enter the defending priority of each of the following defendable territories.\n"
            "Enter priority between 1 (top priority) and " << territories.size() << " (lowest priority)\n" << endl;

    // Initialize all priority territories to null
    vector<Territory*> territoriesPrioritized(territories.size(), nullptr);

    for (auto territory : this->territories)
    {
        cout << territory->getTerritoryName() << "\t";

        int priority;
        cin >> priority;

        while (territoriesPrioritized[priority - 1] != nullptr)
        {
          if (priority > territories.size() || priority < 1)
            cout << "Invalid priority. Please try again." << endl;
          else
            cout << "You have already assigned a priority to that territory. Please try again." << endl;

            cout << territory->getTerritoryName() << "\t";
            cin >> priority;
        }

        // Found an empty priority index, add current territory to that index
        territoriesPrioritized[priority - 1] = territory;
    }

    return territoriesPrioritized;
}

vector<Territory *> Player::toAttack()
{

  // Get adjacent territories of all territories owned by other players
  vector<Territory *> attackableTerritories;
  bool adjacentTerritorySeen = false;
  for (auto terr : this->territories)
  {
    for (auto adjTerr : terr->getAdjTerritories())
    {
      if (adjTerr->getTerritoryOwner() != this)
      {
        for (auto attTerr : attackableTerritories)
        {
          // Avoid adding duplicates to the territory vector
          if (attTerr == adjTerr)
          {
            adjacentTerritorySeen = true;
            break;
          }
        }
        if (!adjacentTerritorySeen)
          attackableTerritories.push_back(adjTerr);

        adjacentTerritorySeen = false;
      }
    }
  }

  if (attackableTerritories.empty())
  {
    cout << "No attackable territories found." << endl;
    return attackableTerritories;
  }

  cout << "Please enter the attacking priority of each of the following attackable territories.\n"
            "Enter priority between 1 (top priority) and " << attackableTerritories.size() << " (lowest priority)\n" << endl;

  // Initialize all priority territories to null
  vector<Territory*> territoriesPrioritized(attackableTerritories.size(), nullptr);

  for (auto territory : attackableTerritories)
  {
      cout << territory->getTerritoryName() << "\t";

      int priority;
      cin >> priority;

      while (territoriesPrioritized[priority - 1] != nullptr)
      {
          cout << "\nYou have already assigned a priority to that territory. Please try again." << endl;
          cout << territory->getTerritoryName() << "\t";

          cin >> priority;
      }

      // Found an empty priority index, add current territory to that index
      territoriesPrioritized[priority - 1] = territory;
  }
  return territoriesPrioritized;
}

//Order *Player::issueOrder(OrderType &o)
//{
//    return orders->createOrder(o);
//    // Ask player for attacking territory priority
//    vector<Territory*> attackableTerritories = toAttack();
//
//    // Ask player for defending territory priority
//    vector<Territory*> defendableTerritories = toDefend();
//
//    // -------------------- DEPLOYING -----------------------
//    cout << "\nDeploy army units\n" << endl;
//
//  // Deploy troops until you run out
//  while(this->getReinforcements() > 0)
//  {
//      int numReinforcements = this->getReinforcements();
//      cout << "Enter how many army units you would like to deploy to each territory.\n"
//              "You currently have " << numReinforcements << " units remaining." << endl;
//
//      for (auto territory : defendableTerritories)
//      {
//          cout << territory->getTerritoryName() << "\t";
//
//          int selectedNumUnits;
//          cin >> selectedNumUnits;
//
//          if (selectedNumUnits > numReinforcements)
//              selectedNumUnits = numReinforcements;
//
//          setReinforcements(numReinforcements - selectedNumUnits);
//
//          cout << getReinforcements() << " units remaining." << endl;
//      }
//  }

  // -------------------- ADVANCING -----------------------

//  cout << "Territories you can attack:" << endl;
//  for (int i = 0; i < attackableTerritories.size(); i++)
//  {
//    cout << attackableTerritories[i]->getTerritoryName() << endl;
//  }
//  bool stillAttacking = true;
//  while (stillAttacking)
//  {
//    cout << "Select a Territory to attack, enter 'done' if you are done selecting territories: ";
//    string chosenTerritory;
//    cin >> chosenTerritory;
//
//    capitalizeFirstLetter(chosenTerritory); // TODO test - verify that the string changes to have the correct captilization
//
//    if(chosenTerritory == "Done")
//      stillAttacking = false;
//  }
  
//}

Order *Player::issueOrder(string &o) { return this->issueOrder(o); }

void Player::addTerritory(Territory *territory)
{
  territories.push_back(territory);
}

Hand *Player::getHand()
{
  return this->cards;
}

int Player::getReinforcements() const
{
  return this->reinforcements;
}

OrdersList *Player::getOrdersList()
{
  return this->orders;
}

vector<Territory *> Player::getTerritories()
{
  return this->territories;
}

void Player::setReinforcements(int army)
{
  this->reinforcements = army;
}

bool Player::getIsNeutral() const {
    return this->isNeutral;
}

void Player::setIsNeutral(bool isNeutral) {
    this->isNeutral = isNeutral;
}

void capitalizeFirstLetter(string &s)
{
  s[0] = toupper(s[0]);
  for (int i = 1; i < s.length(); i++)
    s[i] = tolower(s[i]);
}
OrdersList* Player::getOrders()
{
    return orders;
}

void Player::setCanPickUp(bool canPickUp)
{
    this->canPickUp = canPickUp;
}

bool Player::getCanPickUp()
{
    return canPickUp;
}

bool Player::getIsFirstTurn() const {
    return isFirstTurn;
}

void Player::setIsFirstTurn(bool isFirstTurn) {
    this->isFirstTurn = isFirstTurn;
}

vector<Player*> Player::getNegotiatingWith()
{
    return negotiatingWith;
}

void Player::addNegotiator(Player* p)
{
    negotiatingWith.push_back(p);
}

void Player::resetNegotiators()
{
    negotiatingWith.clear();
}