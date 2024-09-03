#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <ctime>

using namespace std;

class Card
{
public:
    Card();                                                  // Default constructor
    Card(string type, int quantity);                         // Constructor with input
    Card(const Card &p);                                     // Copy constructor
    ~Card();                                                 // Destructor
    Card &operator=(const Card &p);                          // Assignment operator
    friend ostream &operator<<(ostream &out, const Card &p); // Stream insertion operator

    int getCardQuantity();
    void setCardQuantity(int quantity);

private:
    string type;
    int quantity;
};
class CardsCollection
{
public:
    CardsCollection();                                                                      // Default constructor
    CardsCollection(int bomb, int reinforcement, int blockade, int airlift, int diplomacy); // Constructor with input
    CardsCollection(const CardsCollection &p);                                              // Copy constructor
    ~CardsCollection();                                                                     // Destructor
    CardsCollection &operator=(const CardsCollection &p);                                   // Assignment operator
    friend ostream &operator<<(ostream &out, const CardsCollection &p);                     // Stream insertion operator

    bool exchangeCards(CardsCollection *source, CardsCollection *target, string type, string typeOfExchange);
    void displayCollection(string title);
    Card *getCard(string type);

protected:
    Card *bomb;
    Card *reinforcement;
    Card *blockade;
    Card *airlift;
    Card *diplomacy;
};
class Deck : public CardsCollection
{
public:
    Deck();                                                                      // Default constructor
    Deck(int bomb, int reinforcement, int blockade, int airlift, int diplomacy); // Constructor with input
    Deck(const Deck &p);                                                         // Copy constructor
    ~Deck();                                                                     // Destructor
    Deck &operator=(const Deck &p);                                              // Assignment operator
    friend ostream &operator<<(ostream &out, const Deck &p);                     // Stream insertion operator

    bool draw(CardsCollection *hand);
    void displayDeck();
};

class Hand : public CardsCollection
{
public:
    Hand();                                                                      // Default constructor
    Hand(int bomb, int reinforcement, int blockade, int airlift, int diplomacy); // Constructor with input
    Hand(const Hand &p);                                                         // Copy constructor
    ~Hand();                                                                     // Destructor
    Hand &operator=(const Hand &p);                                              // Assignment operator
    friend ostream &operator<<(ostream &out, const Hand &p);                     // Stream insertion operator

    void displayHand();
    bool play(CardsCollection *deck, string card);
};

inline void testCards()
{
    // Test Deck constructor
    Deck *d1 = new Deck(2, 3, 4, 5, 6);
    cout << "\nCreate deck with custom amounts and display it: " << endl;
    d1->displayDeck();

    // Test Deck copy constructor
    Deck *d2 = new Deck(*d1);
    cout << "\nCreate copy of deck, and display the copy: " << endl;
    d2->displayDeck();

    // Test Deck destructor
    cout << "\nDelete copy of deck: " << endl;
    delete d2;

    // Test default deck constructor
    Deck *d3 = new Deck();
    cout << "\nCreate new default deck, and display the default deck: " << endl;
    d3->displayDeck();

    // Delete default deck
    cout << "\nDelete default deck: " << endl;
    delete d3;

    // Test assignment operator
    Deck *d4 = new Deck();
    *d4 = *d1;
    cout << "\nAssign new deck to the first created deck, display the deck, then delete it: " << endl;
    d4->displayDeck();

    cout << "\nDelete new deck: " << endl;
    delete d4;

    // Test hand constructor
    Hand *h1 = new Hand();
    cout << "\nCreate empty hand, and display the hand: " << endl;
    h1->displayHand();

    Hand *h2 = new Hand(2, 2, 2, 2, 2);
    cout << "\nCreate hand with cards, and display the hand: " << endl;
    h2->displayHand();

    cout << "\nDelete empty hand: " << endl;
    delete h1;

    cout << "\nDraw card into hand, and display the hand: " << endl;
    d1->draw(h2);
    h2->displayHand();

    cout << "\nPlay a bomb card from the hand, and display the hand: " << endl;
    h2->play(d1, "bomb");
    h2->displayHand();

    cout << "\nAlso display the deck to prove that the played card was added to the deck: " << endl;
    d1->displayDeck();

    cout << "\nDelete remaining hand and deck: " << endl;
    delete h2;
    delete d1;

    cout << "\nExecution of CardsDriver complete!" << endl;
}