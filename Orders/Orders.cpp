#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../GameLogObserver/LoggingObserver.h"


// list<Order *> orders = ol->getOrders(); // Make a copy of the list to avoid iterator issues

using namespace std;

// default order constructors:
// default order constructors:

// generic order
Order::Order(){
    isExecuted = false;
    setOrderType("Generic");
    setIsExecuted(false);
}

// order with type
Order::Order(Player* orderPlayer, string orderType){
    isExecuted = false;
    this->orderPlayer = orderPlayer;
    this->setOrderType(orderType);
    setIsExecuted(false);
}

Player* Order::getOrderPlayer()
{
    return orderPlayer;
}

// deploy order
DeployOrder::DeployOrder(Player* orderPlayer, Territory* targetTerritory, int numberToDeploy)
: Order(orderPlayer, "Deploy")
{
    this->targetTerritory = targetTerritory;
    this->numberToDeploy = numberToDeploy;
}

// advance order
AdvanceOrder::AdvanceOrder(Player* orderPlayer, Territory* sourceTerritory, Territory* targetTerritory, int numberToAdvance)
    : Order(orderPlayer, "Advance")
{
    this->sourceTerritory = sourceTerritory;
    this->targetTerritory = targetTerritory;
    this->numberToAdvance = numberToAdvance;
}

// airlift order
AirliftOrder::AirliftOrder(Player* orderPlayer, Territory* sourceTerritory, Territory* targetTerritory, int numberToAirlift)
        : Order(orderPlayer, "Airlift")
{
    this->sourceTerritory = sourceTerritory;
    this->targetTerritory = targetTerritory;
    this->numberToAirlift = numberToAirlift;
}

// bomb order
BombOrder::BombOrder(Player* orderPlayer, Territory* targetTerritory)
    : Order(orderPlayer, "Bomb")
{
    this->targetTerritory = targetTerritory;
}

// blockade order
BlockadeOrder::BlockadeOrder(Player* orderPlayer, Territory* targetTerritory)
    : Order(orderPlayer, "Blockade")
{
    this->targetTerritory = targetTerritory;
}


// negotiate order
NegotiateOrder::NegotiateOrder(Player* orderPlayer, Player* targetPlayer)
: Order(orderPlayer, "Negotiate")
{
    this->targetPlayer = targetPlayer;
}


// copy constructor
Order::Order(Order &o)
{
    setOrderType(o.getOrderType());
}

string Order::getOrderType() { return orderType; }

void Order::setOrderType(string type) { orderType = type; }

bool Order::getIsExecuted() const { return isExecuted; }

void Order::setIsExecuted(bool isExecuted) { this->isExecuted = isExecuted; }

bool Order::validate()
{
    return true;
}

// from the reinforcement pool to deploy to a target territory owned by the player
void DeployOrder::execute()
{
    cout << getOrderPlayer()->getName() << " deploys " << numberToDeploy << " to " + targetTerritory->getTerritoryName() << std::endl;
    cout << "Executing " << this->getOrderType() << std::endl;
        setIsExecuted(true);
        //Notify from Subject called
        Notify();

    if(targetTerritory->getTerritoryOwner() != getOrderPlayer())
    {
        cout << "Order is not valid (territory does not belong to player)" <<  endl;
        
    }
    else
    {
        targetTerritory->setArmyCount(targetTerritory->getArmyCount() + numberToDeploy);
    }

    setIsExecuted(true);
}

void AdvanceOrder::execute()
{
    cout << getOrderPlayer()->getName() << " advances " << numberToAdvance << " armies from " +
    sourceTerritory->getTerritoryName() << " to " << targetTerritory->getTerritoryName() << std::endl;

    // number of army units move from a source territory to a target adjacent territory

    // check if the source territory belongs to the player and check if the two chosen territories are adjacent
    vector<Territory*>& adjacentTerritories = sourceTerritory->getAdjTerritories();
    if(std::find(adjacentTerritories.begin(), adjacentTerritories.end(), targetTerritory) == adjacentTerritories.end())
    {
        cout << "Order is no valid (territories are not adjacent)" << std::endl;
    }
    else if(sourceTerritory->getTerritoryOwner() != getOrderPlayer())
    {
        cout << "Order is not valid (source territory does not belong to player)" <<  endl;
    }
    else if(sourceTerritory->getArmyCount() < numberToAdvance)
    {
        cout << "Order is not valid (not enough armies to advance)" << endl;
    }
    else
    {
        vector<Player*> negotiators = getOrderPlayer()->getNegotiatingWith();
        if(std::find(negotiators.begin(), negotiators.end(), targetTerritory->getTerritoryOwner()) != negotiators.end())
        {
            cout << "Order is not valid (players are negotiating)";
        }
        else if(targetTerritory->getTerritoryOwner() == getOrderPlayer())
        {
            sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - numberToAdvance);
            targetTerritory->setArmyCount(targetTerritory->getArmyCount() + numberToAdvance);
        }
        // attack if the target territory belongs to another player
        else
        {
            sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - numberToAdvance);

            // create a random generator object
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 10);

            // decide how many of the opponent's army to kill
            int opponentDeathNumber = 0;
            for(int i = 0; i < numberToAdvance; i++)
            {
                int randomNumber = dis(gen);
                if(randomNumber <= 6)
                {
                    opponentDeathNumber++;
                }
            }
            // decide how many of the player's army to kill
            int playerDeathNumber = 0;
            for(int i = 0; i < targetTerritory->getArmyCount(); i++)
            {
                int randomNumber = dis(gen);
                if(randomNumber <= 7)
                {
                    playerDeathNumber++;
                }
            }

            // remove the killed opponents
            targetTerritory->setArmyCount(targetTerritory->getArmyCount() - opponentDeathNumber);

            // give the territory to the attacker if there are no defenders left
            if(targetTerritory->getArmyCount() <= 0)
            {
                targetTerritory->setTerritoryOwner(getOrderPlayer());
                targetTerritory->setArmyCount(numberToAdvance-playerDeathNumber);

                cout << targetTerritory->getTerritoryName() << " now belongs to " << getOrderPlayer()->getName() << std::endl;

                getOrderPlayer()->setCanPickUp(true);
            }
        }
    }
    setIsExecuted(true);
}

// Certain number of army units taken from a source territory to  a target territory
void AirliftOrder::execute()
{
    cout << getOrderPlayer()->getName() << " airlifts " << numberToAirlift << " armies from " +
        sourceTerritory->getTerritoryName() << " to " << targetTerritory->getTerritoryName() << std::endl;
    if(sourceTerritory->getTerritoryOwner() != getOrderPlayer())
    {
        cout << "Order is not valid (source territory does not belong to player)" <<  endl;
    }
    else if(targetTerritory->getTerritoryOwner() != getOrderPlayer())
    {
        cout << "Order is not valid (source territory does not belong to player)" <<  endl;
    }
    else if(sourceTerritory->getArmyCount() < numberToAirlift)
    {
        cout << "Order is not valid (not enough armies to airlift)" << endl;
    }
    else
    {
        sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - numberToAirlift);
        targetTerritory->setArmyCount(targetTerritory->getArmyCount() + numberToAirlift);
    }

    setIsExecuted(true);
}

// remove half the units from an opponent's territory
void BombOrder::execute()
{
    cout << getOrderPlayer()->getName() << " bombs " << targetTerritory->getTerritoryName() << std::endl;
    // check if the player has an adjacent territory to the target territory
    vector<Territory*>& adjacentTerritories = targetTerritory->getAdjTerritories();
    bool hasAdjacent = false;
    for(Territory* t: adjacentTerritories)
    {
        if(t->getTerritoryOwner() == getOrderPlayer())
            hasAdjacent = true;
    }
    if(!hasAdjacent)
    {
        cout << "Order is not valid (target territory is not adjacent to a player's territory)" << std::endl;
    }
    else if(targetTerritory->getTerritoryOwner() == getOrderPlayer())
    {
        cout << "Order is not valid (target territory belongs to player)" << std::endl;
    }
    else
    {
        targetTerritory->setArmyCount(targetTerritory->getArmyCount()/2);
    }

    setIsExecuted(true);
}

// doubles the amount of armies on a player's territory
void BlockadeOrder::execute()
{
    cout << getOrderPlayer()->getName() << " blockades " << targetTerritory->getTerritoryName() << std::endl;

    if((targetTerritory->getTerritoryOwner() != getOrderPlayer()) && targetTerritory->getTerritoryOwner() != nullptr)
    {
        cout << "Order is not valid (target territory belongs to enemy)" << std::endl;
    }
    else
    {
        targetTerritory->setArmyCount(targetTerritory->getArmyCount()*2);
        targetTerritory->setTerritoryOwner(getOrderPlayer());
        targetTerritory->setTerritoryOwner(new Player("Neutral", true));
        targetTerritory->setPreviousOwner(getOrderPlayer());
    }

    setIsExecuted(true);

}

// two players can't attack each other for the remainder of the turn
void NegotiateOrder::execute()
{
    cout << getOrderPlayer()->getName() << " and " << targetPlayer->getName() << " are negotiating." << endl;
    if(getOrderPlayer() == targetPlayer)
    {
        cout << "Order is not valid (target player is the player)";
    }
    else
    {
        getOrderPlayer()->addNegotiator(targetPlayer);
        targetPlayer->addNegotiator(getOrderPlayer());
    }

    setIsExecuted(true);
}

// stream operator order
std::ostream &operator<<(std::ostream &os, Order &order)
{
    os << order.getOrderType() << " order. ";

    if (order.getIsExecuted())
    {
        os << "Order was executed";
    }
    os << std::endl;

    return os;
}

// assignment operater order
Order& Order::operator=(Order& o)
{
    o.setOrderType(getOrderType());
    o.setIsExecuted(getIsExecuted());
    return *this;
}

void Order::execute() {

}


//Overriden stringToLog() method fromILoggable
string Order::stringToLog() {
    string log = "Observing Issueing Orders Phase, Order to be executed is: \n Orders: " + orderType;
    return log;
}

//Implementation of Attach method (removes an observer from the observers list)
void Order::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void Order::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void Order::Notify(){
    list<Observer*>::iterator i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}


// Order list:

// default constructor
OrdersList::OrdersList() {}

// destructor
OrdersList::~OrdersList()
{
    for (Order *order : orders)
    {
        delete order;
    }
    orders.clear();
}

// copy constructor
OrdersList::OrdersList(OrdersList &ol)
{
    this->orders = ol.getOrders();
}

// stream operator order list
ostream &operator<<(ostream &os, OrdersList &ol)
{
    int i = 0;
    for (Order *order : ol.getOrders())
    {
        os << ++i << ": ";
        os << *order;
    }
    return os;
}

vector<Order *> OrdersList::getOrders()
{
    return this->orders;
}

Order *OrdersList::createOrder(std::string orderType, Player* currentPlayer, std::vector<Player*>& gamePlayers)
{
    Order *newOrder;
    // Handled manually
    if (orderType == "Deploy")
    {
        newOrder = new DeployOrder(new Player(), new Territory(), 3);
    }
    // Handled manually
    else if(orderType == "Advance")
    {
        newOrder = new AdvanceOrder(new Player(), new Territory(), new Territory(), 3);
    }
    else if(orderType == "Airlift")
    // player can move some units from one territory they own to another
    {
        vector<Territory*> playerTerritories = currentPlayer->getTerritories();
        cout << "Territories you may airlift from ✈️:" << endl;

        for (auto terr : playerTerritories) {
            if (terr->getArmyCount() != 0)
                cout << terr->getTerritoryName() << "\t" << terr->getArmyCount() << " units." << endl;
        }
        cout << "Enter the territory to airlift units from: ";
        string territoryName;
        cin >> territoryName;
        auto territoryIttr = find_if(playerTerritories.begin(), playerTerritories.end(), [&territoryName](Territory* terr){return terr->getTerritoryName() == territoryName && terr->getArmyCount() > 0;});
        while (territoryIttr == playerTerritories.end()){
            cout << "Cannot find territory, please try again: ";
            cin >> territoryName;
            territoryIttr = find_if(playerTerritories.begin(), playerTerritories.end(), [&territoryName](Territory* terr){return terr->getTerritoryName() == territoryName && terr->getArmyCount() > 0;});
        }
        // we have source territory
        Territory* source = *territoryIttr;

        cout << "Territories you may airlift to ✈️:" << endl;
        for (auto terr : playerTerritories) {
            if (terr != source)
                cout << terr->getTerritoryName() << "\t" << terr->getArmyCount() << " units." << endl;
        }
        cout << "Enter the territory to airlift units to:" << endl;
        cin >> territoryName;
        territoryIttr = find_if(playerTerritories.begin(), playerTerritories.end(), [&territoryName, &source](Territory* terr){return terr->getTerritoryName() == territoryName && terr != source;});
        while (territoryIttr == playerTerritories.end()){
            cout << "Cannot find territory, please try again: ";
            cin >> territoryName;
            territoryIttr = find_if(playerTerritories.begin(), playerTerritories.end(), [&territoryName, &source](Territory* terr){return terr->getTerritoryName() == territoryName && terr != source;});
        }
        Territory* target = *territoryIttr;

        int armyToMove;
        cout << "Enter in the number of units to airlift: ";
        cin >> armyToMove;
        while (armyToMove > source->getArmyCount() || armyToMove < 0){
            cout << "Invalid number, please try again: ";
            cin >> armyToMove;
        }

        newOrder = new AirliftOrder(currentPlayer, source, target, armyToMove);
    }
    else if(orderType == "Bomb")
    {
        // player can bomb a territory they do not own as long as they are adjacent to a territory they do own (toAttack)
        vector<Territory*> BOMBEABLEterritories = currentPlayer->toAttack();

        cout << "Territories you may bomb 💣: " << endl;
        for (auto territory : BOMBEABLEterritories)
        {
            cout << territory->getTerritoryName() << endl;
        }
        cout << "Enter territory to bomb: ";
        string territoryName;

        while (!any_of(BOMBEABLEterritories.begin(), BOMBEABLEterritories.end(), [&territoryName](Territory* terr) {return terr->getTerritoryName() == territoryName;}))
        {
            cout << "That is not a valid territory name. Please try again: ";
            cin >> territoryName;
        }

        auto targetTerritory = find_if(BOMBEABLEterritories.begin(), BOMBEABLEterritories.end(), [&territoryName](Territory* terr) {return terr->getTerritoryName() == territoryName;});
        if (targetTerritory != BOMBEABLEterritories.end())
            newOrder = new BombOrder(currentPlayer, *targetTerritory);
        else
            newOrder = new BombOrder(new Player(), new Territory());
    }
    else if(orderType == "Blockade")
    {
        // player can choose a territory they own to blockade
        vector<Territory*> playerTerritories = currentPlayer->getTerritories();

        cout << "Territories you may blockade: " << endl;
        for (auto territory : playerTerritories)
        {
            cout << territory->getTerritoryName() << endl;
        }
        cout << "Enter territory to blockade: ";
        string territoryName;

        while (!any_of(playerTerritories.begin(), playerTerritories.end(), [&territoryName](Territory* terr) {return terr->getTerritoryName() == territoryName;}))
        {
            cout << "That is not a valid territory name. Please try again: ";
            cin >> territoryName;
        }

        auto targetTerritory = find_if(playerTerritories.begin(), playerTerritories.end(), [&territoryName](Territory* terr) {return terr->getTerritoryName() == territoryName;});
        if (targetTerritory != playerTerritories.end())
            newOrder = new BlockadeOrder(currentPlayer, *targetTerritory);
        else
            newOrder = new BlockadeOrder(new Player(), new Territory());
    }
    else if(orderType == "Negotiate")
    {
        cout << "Players you may negotiate with: " << endl;
        for (auto player : gamePlayers)
        {
            if (player != currentPlayer)
            {
                cout << player->getName() << endl;
            }
        }
        cout << "Enter player to negotiate with: ";
        string targetPlayerName;
        cin >> targetPlayerName;

        while (!any_of(gamePlayers.begin(), gamePlayers.end(), [&targetPlayerName](Player* pl) {return pl->getName() == targetPlayerName;}))
        {
            cout << "That is not a valid player name. Please try again: ";
            cin >> targetPlayerName;
        }

        auto targetPlayer = find_if(gamePlayers.begin(), gamePlayers.end(), [&targetPlayerName](Player* pl) {return pl->getName() == targetPlayerName;});
        if (targetPlayer != gamePlayers.end())
            newOrder = new NegotiateOrder(currentPlayer, *targetPlayer);
        else
            newOrder = new NegotiateOrder(new Player(), new Player());
    }
    else
    {
        newOrder = new Order();
    }
    this->addOrder(newOrder);
    return newOrder;
}

void OrdersList::addOrder(Order *order)
{
    orders.push_back(order);
    Notify();
}

void OrdersList::move(int initialListPosition, int targetListPosition)
{
    // verifiy that the index positions are valid
    int listSize = orders.size();
    if (initialListPosition > listSize || initialListPosition < 1 || targetListPosition > listSize || targetListPosition < 1)
    {
        cout << "Invalid move" << std::endl;
    }
    else
    {
        // remove the order
        vector<Order *>::iterator itr1 = orders.begin();
        advance(itr1, initialListPosition - 1);
        Order *o = *itr1; // save a pointer to the order before removing it
        orders.erase(itr1);

        // add it back in the new index
        vector<Order *>::iterator itr2 = orders.begin();
        advance(itr2, targetListPosition - 1);
        orders.insert(itr2, o);

        cout << "Order moved" << std::endl;
    }
}

void OrdersList::remove(int listPosition)
{

    vector<Order *>::iterator itr = orders.begin();
    advance(itr, listPosition - 1);
    orders.erase(itr);

    cout << "Order removed" << std::endl;
}

//void OrdersList::remove(Order* o)
//{
//    orders.erase(std::remove(orders.begin(), orders.end(), o), orders.end());
//
//    cout << "removed";
//
//}

void OrdersList::printOrdersList()
{
    int i = 0;
    for (Order *order : orders)
    {
        cout << ++i << ": ";
        cout << *order;
    }
}

// assignment operater order
OrdersList& OrdersList::operator=(OrdersList& ol)
{
    for (Order* order : ol.getOrders()) {
        // Assuming that Order has a copy constructor
        orders.push_back(new Order(*order));
    }
    return *this;
}


void testOrders()
{
    // create players with territories

    Territory* mtl = new Territory("Montreal");
    Territory* otw = new Territory("Ottawa");
    Territory* qc = new Territory("Quebec City");
    Territory* nm = new Territory("New Mexico");
    Territory* bln = new Territory("Berlin");
    Territory* sbk = new Territory("Sherbrooke");

    vector<Territory*> adjacents;
    adjacents.push_back(mtl);
    adjacents.push_back(otw);
    adjacents.push_back(qc);
    adjacents.push_back(sbk);

    mtl->setAdjTerritories(adjacents);
    otw->setAdjTerritories(adjacents);
    qc->setAdjTerritories(adjacents);
    sbk->setAdjTerritories(adjacents);

    vector<Territory*> playerTerritories;
    playerTerritories.push_back(mtl);
    playerTerritories.push_back(otw);
    playerTerritories.push_back(nm);
    vector<Territory*> opponentTerritories;
    opponentTerritories.push_back(bln);
    opponentTerritories.push_back(qc);
    opponentTerritories.push_back(sbk);

    Player* player = new Player("Steve", playerTerritories, nullptr,
                                nullptr, 105, false);
    Player* opponent = new Player("Spongebob", opponentTerritories , nullptr,
                                  nullptr, 25, false);

    mtl->setTerritoryOwner(player);
    otw->setTerritoryOwner(player);
    nm->setTerritoryOwner(player);
    bln->setTerritoryOwner(opponent);
    qc->setTerritoryOwner(opponent);
    sbk->setTerritoryOwner(opponent);

    cout << "Initial territories:" << std::endl;
    cout << *mtl << std::endl;
    cout << *otw << std::endl;
    cout << *qc << std::endl;
    cout << *nm << std::endl;
    cout << *sbk << std::endl;
    cout << *bln << "\n" << std::endl;

    // create and execute orders:

    // create an order list and add every type of oder to it
    OrdersList * ol = new OrdersList();
    ol->addOrder(new DeployOrder(player, mtl, 100));
    ol->addOrder(new DeployOrder(opponent, qc, 5));
    ol->addOrder(new DeployOrder(player, bln, 5));
    ol->addOrder(new DeployOrder(opponent, sbk, 20));
    ol->addOrder(new AdvanceOrder(player, mtl, otw, 5));
    ol->addOrder(new AdvanceOrder(player, otw, qc, 1));
    ol->addOrder(new AdvanceOrder(player, mtl, qc, 15));
    ol->addOrder(new AirliftOrder(player, mtl, nm, 15));
    ol->addOrder(new AirliftOrder(player, mtl, bln, 15));
    ol->addOrder(new BombOrder(player, sbk));
    ol->addOrder(new BombOrder(player, mtl));
    ol->addOrder(new BlockadeOrder(player, mtl));
    ol->addOrder(new BlockadeOrder(player, sbk));
    ol->addOrder(new NegotiateOrder(player, opponent));
    ol->addOrder(new NegotiateOrder(player, player));

    for(Order* o: ol->getOrders())
    {
        o->execute();
    }

    cout << "\n\nFinal territories:" << std::endl;
    cout << *mtl << std::endl;
    cout << *otw << std::endl;
    cout << *qc << std::endl;
    cout << *nm << std::endl;
    cout << *sbk << std::endl;
    cout << *bln << "\n" << std::endl;


}

//Overriden stringToLog() method fromILoggable
string OrdersList::stringToLog() {
    string val = "Orders list is empty, error occured";
    if(!orders.empty()){
        val = orders.back()->getOrderType();
    }
    string log = "Observing Processing Orders Phase, order to added is: \n"+val;
    return log;
}

void OrdersList::Attach(Observer* o){
    observers->push_back(o);
}

//Implementation of Remove method (removes an observer from the observers list)
void OrdersList::Detach(Observer* o){
    observers->remove(o);
}

//Implementation of the Notify method (will call on the observers update method)
void OrdersList::Notify(){
    list<Observer*>::iterator i = observers->begin();
    for(;i != observers->end(); ++i){
        //using "this" to pass the subject to the update method
        (*i)->Update(this);
    }
}

