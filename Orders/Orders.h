#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "../GameLogObserver/LoggingObserver.h"
using namespace std;

class Territory;
class Player;



class Order : public Subject, public ILoggable
{
    public:

        // default constructor
        Order();
        // constructor with order type
        Order(Player* orderPlayer, string orderType);
        // copy constructor
        Order(Order &o);
        // executes an order
        virtual void execute();
        // Overload the << operator to describe the order
        friend ostream & operator << (ostream& os, Order& order);
        // checks if an order is valid
        bool validate();
        bool getIsExecuted() const;
        void setIsExecuted(bool isExecuted);
        void printOrderNarrative();

        string getOrderType();
        void setOrderType(string orderType);
        Player* getOrderPlayer();
        // override the assignment operator
        Order& operator=(Order& o);

        //Overriden stringToLog() method fromILoggable
        string stringToLog() override;
        void Attach(Observer* o) override;
        void Detach(Observer* o) override;
        void Notify() override;

    private:

        string orderType;
        bool isExecuted;
        Player* orderPlayer;

};

// specific order classes:
class DeployOrder : public Order 
{
    public:

        Territory* targetTerritory;
        int numberToDeploy;

        // default constructor
        DeployOrder(Player* orderPlayer, Territory* targetTerritory, int numberToDeploy);

        void execute();
};
class AdvanceOrder :public  Order 
{
        Territory* sourceTerritory;
        Territory* targetTerritory;
        int numberToAdvance;
        
    public:
        // default constructor
        AdvanceOrder(Player* orderPlayer, Territory* sourceTerritory, Territory* targetTerritory, int numberToAdvance);

        void execute();
};
class AirliftOrder : public Order
{

    public:
        Territory* sourceTerritory;
        Territory* targetTerritory;
        int numberToAirlift;

        // default constructor
        AirliftOrder(Player* orderPlayer, Territory* sourceTerritory, Territory* targetTerritory, int numberToAirlift);

        void execute();
};

class BombOrder : public Order 
{
    public:
        Territory* targetTerritory;
        // default constructor
        BombOrder(Player* orderPlayer, Territory* targetTerritory);

        void execute();
};
class BlockadeOrder : public Order 
{
    public:
        Territory* targetTerritory;
        // default constructor
        BlockadeOrder(Player* orderPlayer, Territory* targetTerritory);
        
        void execute();
};
class NegotiateOrder : public Order 
{
    public:

        Player* targetPlayer;

        NegotiateOrder(Player* orderPlayer, Player* targetPlayer);

        void execute();
};


class OrdersList : public Subject, public ILoggable
{

    private:
        std::vector<Order *> orders;

    public:

        // default constructor
        OrdersList();

        // copy constructor
        OrdersList(OrdersList &ol);

        // destructor
        ~OrdersList();

        std::vector<Order *> getOrders();

        Order* createOrder(std::string orderType, Player* currentPlayer, std::vector<Player*>& gamePlayers);

        // add an order to the list
        void addOrder(Order *order);

        // change the position of an order in the order list
        void move(int listPosition1, int targerList);

        // remove an order from the order list
        void remove(int listPosition);
        void remove(Order* o);

        // prints all the orders in the list
        void printOrdersList();
        
        // Overload the << operator to describe the order list
        friend std::ostream& operator<<(std::ostream& os, OrdersList& ordersList);
        
        // override the assignment operator
        OrdersList& operator=(OrdersList& ol);
        //Overriding the stringtoLog from ILoggable
        string stringToLog() override;
        //Overriding the attach from Subject
        void Attach(Observer* o) override;
        //Overriding the detach from Subject
        void Detach(Observer* o) override;
        //Overriding the Notify from Subject
        void Notify() override;
};

void testOrders();