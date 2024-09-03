#include <random>
#include "Cards.h"

using namespace std;

// DECK

// Default constructor - creates a deck with 5 of every card type
Deck::Deck()
{
    this->bomb = new Card("bomb", 5);
    this->reinforcement = new Card("reinforcement", 5);
    this->blockade = new Card("blockade", 5);
    this->airlift = new Card("airlift", 5);
    this->diplomacy = new Card("diplomacy", 5);
}

// Constructor with input
Deck::Deck(int bomb, int reinforcement, int blockade, int airlift, int diplomacy)
{
    this->bomb = new Card("bomb", bomb);
    this->reinforcement = new Card("reinforcement", reinforcement);
    this->blockade = new Card("blockade", blockade);
    this->airlift = new Card("airlift", airlift);
    this->diplomacy = new Card("diplomacy", diplomacy);
}

// Copy constructor
Deck::Deck(const Deck &p)
{
    this->bomb = new Card("bomb", p.bomb->getCardQuantity());
    this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
    this->blockade = new Card("blockade", p.blockade->getCardQuantity());
    this->airlift = new Card("airlift", p.airlift->getCardQuantity());
    this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
}

// Destructor
Deck::~Deck()
{
    delete this->bomb;
    this->bomb = NULL;

    delete this->reinforcement;
    this->reinforcement = NULL;

    delete this->blockade;
    this->blockade = NULL;

    delete this->airlift;
    this->airlift = NULL;

    delete this->diplomacy;
    this->diplomacy = NULL;

    cout << "Deck destructor called" << endl;
}

// Assignment operator
Deck &Deck::operator=(const Deck &p)
{
    // Check for self-assignment
    if (this != &p)
    {
        this->bomb = new Card("bomb", p.bomb->getCardQuantity());
        this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
        this->blockade = new Card("blockade", p.blockade->getCardQuantity());
        this->airlift = new Card("airlift", p.airlift->getCardQuantity());
        this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
    }
    return *this;
}

// Stream insertion operator
ostream &operator<<(ostream &out, const Deck &p)
{
    out << "Bomb cards remaining in deck: " << p.bomb->getCardQuantity() << endl;
    out << "Reinforcement cards remaining in deck: " << p.reinforcement->getCardQuantity() << endl;
    out << "Blockade cards remaining in deck: " << p.blockade->getCardQuantity() << endl;
    out << "Airlift cards remaining in deck: " << p.airlift->getCardQuantity() << endl;
    out << "Diplomacy cards remaining in deck: " << p.diplomacy->getCardQuantity() << endl;
    return out;
}

// Draw a card from the deck and put it into the given hand
bool Deck::draw(CardsCollection *hand)
{
    std::random_device rd;
    auto rng = default_random_engine(rd());
    int RNG = rng() % 5;
    string type = "";
    switch (RNG)
    {
    case 0:
        type = "bomb";
        break;
    case 1:
        type = "reinforcement";
        break;
    case 2:
        type = "blockade";
        break;
    case 3:
        type = "airlift";
        break;
    case 4:
        type = "diplomacy";
        break;
    default:
        cout << "Invalid type supplied in Deck draw! The RNG broke!\n"
             << endl;
    }
    // run RNG to choose a random card type (RNG int 0,1,2,3,4? -> assign int to card type)
    return exchangeCards(this, hand, type, "Drew");
}

// Display the deck
void Deck::displayDeck()
{
    displayCollection("deck");
}

// HAND

// Default constructor
Hand::Hand()
{
    // default constructor creates a hand with 0 of every card type
    this->bomb = new Card("bomb", 0);
    this->reinforcement = new Card("reinforcement", 0);
    this->blockade = new Card("blockade", 0);
    this->airlift = new Card("airlift", 0);
    this->diplomacy = new Card("diplomacy", 0);
}

// Constructor with input
Hand::Hand(int bomb, int reinforcement, int blockade, int airlift, int diplomacy)
{
    // create a new hand with custom quantities of each card type
    this->bomb = new Card("bomb", bomb);
    this->reinforcement = new Card("reinforcement", reinforcement);
    this->blockade = new Card("blockade", blockade);
    this->airlift = new Card("airlift", airlift);
    this->diplomacy = new Card("diplomacy", diplomacy);
}

// Copy constructor
Hand::Hand(const Hand &p)
{
    this->bomb = new Card("bomb", p.bomb->getCardQuantity());
    this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
    this->blockade = new Card("blockade", p.blockade->getCardQuantity());
    this->airlift = new Card("airlift", p.airlift->getCardQuantity());
    this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
}

// Destructor
Hand::~Hand()
{
    delete this->bomb;
    this->bomb = NULL;

    delete this->reinforcement;
    this->reinforcement = NULL;

    delete this->blockade;
    this->blockade = NULL;

    delete this->airlift;
    this->airlift = NULL;

    delete this->diplomacy;
    this->diplomacy = NULL;

    cout << "Hand destructor called" << endl;
}

// Assignment operator
Hand &Hand::operator=(const Hand &p)
{
    // Check for self-assignment
    if (this != &p)
    {
        this->bomb = new Card("bomb", p.bomb->getCardQuantity());
        this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
        this->blockade = new Card("blockade", p.blockade->getCardQuantity());
        this->airlift = new Card("airlift", p.airlift->getCardQuantity());
        this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
    }
    return *this;
}

// Stream insertion operator
ostream &operator<<(ostream &out, const Hand &p)
{
    out << "Bomb cards remaining in hand: " << p.bomb->getCardQuantity() << endl;
    out << "Reinforcement cards remaining in hand: " << p.reinforcement->getCardQuantity() << endl;
    out << "Blockade cards remaining in hand: " << p.blockade->getCardQuantity() << endl;
    out << "Airlift cards remaining in hand: " << p.airlift->getCardQuantity() << endl;
    out << "Diplomacy cards remaining in hand: " << p.diplomacy->getCardQuantity() << endl;
    return out;
}

// Display the hand
void Hand::displayHand()
{
    displayCollection("hand");
}

// Remove a card from the hand, add its order to the order list, and add the card back into the deck
bool Hand::play(CardsCollection *deck, string type)
{
    return exchangeCards(this, deck, type, "Played");
}

// CARD

// Default constructor
Card::Card()
{
    this->type = "bomb";
    this->quantity = 0;
}

// Constructor with input
Card::Card(string type, int quantity)
{
    if (type == "bomb" || type == "reinforcement" || type == "blockade" || type == "airlift" || type == "diplomacy")
    {
        this->type = type;
    }
    else
    {
        cout << "Invalid name type supplied in Card constructor, defaulting to bomb\n"
             << endl;
        this->type = "bomb";
    }
    this->quantity = quantity;
}

// Copy constructor
Card::Card(const Card &p)
{
    this->type = p.type;
    this->quantity = p.quantity;
}

// Destructor
Card::~Card()
{
    this->type.clear();
}

// Assignment operator
Card &Card::operator=(const Card &p)
{
    // Check for self-assignment
    if (this != &p)
    {
        this->type = p.type;
        this->quantity = p.quantity;
    }

    return *this;
}

// Stream insertion operator
ostream &operator<<(ostream &out, const Card &p)
{
    out << "Type of card: " << p.type << endl;
    out << "Quantity remaining in collection: " << p.quantity << endl;
    return out;
}

// Return the number of copies of this card in the given CardsCollection
int Card::getCardQuantity()
{
    return this->quantity;
}

// Set the number of copies of this card in the given CardsCollection
void Card::setCardQuantity(int quantity)
{
    this->quantity = quantity;
}

// CARDSCOLLECTION

// Default constructor
CardsCollection::CardsCollection()
{
    this->bomb = new Card("bomb", 0);
    this->reinforcement = new Card("reinforcement", 0);
    this->blockade = new Card("blockade", 0);
    this->airlift = new Card("airlift", 0);
    this->diplomacy = new Card("diplomacy", 0);
}

// Constructor with input
CardsCollection::CardsCollection(int bomb, int reinforcement, int blockade, int airlift, int diplomacy)
{
    this->bomb = new Card("bomb", bomb);
    this->reinforcement = new Card("reinforcement", reinforcement);
    this->blockade = new Card("blockade", blockade);
    this->airlift = new Card("airlift", airlift);
    this->diplomacy = new Card("diplomacy", diplomacy);
}

// Copy constructor
CardsCollection::CardsCollection(const CardsCollection &p)
{
    this->bomb = new Card("bomb", p.bomb->getCardQuantity());
    this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
    this->blockade = new Card("blockade", p.blockade->getCardQuantity());
    this->airlift = new Card("airlift", p.airlift->getCardQuantity());
    this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
}

// Destructor
CardsCollection::~CardsCollection()
{
    delete this->bomb;
    this->bomb = NULL;

    delete this->reinforcement;
    this->reinforcement = NULL;

    delete this->blockade;
    this->blockade = NULL;

    delete this->airlift;
    this->airlift = NULL;

    delete this->diplomacy;
    this->diplomacy = NULL;

    cout << "CardsCollection destructor called" << endl;
}

// Assignment operator
CardsCollection &CardsCollection::operator=(const CardsCollection &p)
{
    // Check for self-assignment
    if (this != &p)
    {
        this->bomb = new Card("bomb", p.bomb->getCardQuantity());
        this->reinforcement = new Card("reinforcement", p.reinforcement->getCardQuantity());
        this->blockade = new Card("blockade", p.blockade->getCardQuantity());
        this->airlift = new Card("airlift", p.airlift->getCardQuantity());
        this->diplomacy = new Card("diplomacy", p.diplomacy->getCardQuantity());
    }
    return *this;
}

// Stream insertion operator
ostream &operator<<(ostream &out, const CardsCollection &p)
{
    out << "Bomb cards remaining: " << p.bomb->getCardQuantity() << endl;
    out << "Reinforcement cards remaining: " << p.reinforcement->getCardQuantity() << endl;
    out << "Blockade cards remaining: " << p.blockade->getCardQuantity() << endl;
    out << "Airlift cards remaining: " << p.airlift->getCardQuantity() << endl;
    out << "Diplomacy cards remaining: " << p.diplomacy->getCardQuantity() << endl;
    return out;
}

// helper function to increment/decrement the card collections - executed whenever a card is drawn or played
bool CardsCollection::exchangeCards(CardsCollection *source, CardsCollection *target, string type, string typeOfExchange)
{
    if (!strcmp(type.c_str(), "bomb"))
    {
        if (source->getCard("bomb")->getCardQuantity() > 0)
        {
            source->getCard("bomb")->setCardQuantity(source->getCard("bomb")->getCardQuantity() - 1);
            target->getCard("bomb")->setCardQuantity(target->getCard("bomb")->getCardQuantity() + 1);
            cout << typeOfExchange << " a bomb card!" << endl;
            return true;
        }
        else
        {
            cout << "Failed to " << typeOfExchange << " a bomb card, source collection does not have any bomb cards" << endl;
            return false;
        }
    }
    else if (!strcmp(type.c_str(), "reinforcement"))
    {
        if (source->getCard("reinforcement")->getCardQuantity() > 0)
        {
            source->getCard("reinforcement")->setCardQuantity(source->getCard("reinforcement")->getCardQuantity() - 1);
            target->getCard("reinforcement")->setCardQuantity(target->getCard("reinforcement")->getCardQuantity() + 1);
            cout << typeOfExchange << " a reinforcement card!" << endl;
            return true;
        }
        else
        {
            cout << "Failed to " << typeOfExchange << " a reinforcement card, source collection does not have any reinforcement cards." << endl;
            return false;
        }
    }
    else if (!strcmp(type.c_str(), "blockade"))
    {
        if (source->getCard("blockade")->getCardQuantity() > 0)
        {
            source->getCard("blockade")->setCardQuantity(source->getCard("blockade")->getCardQuantity() - 1);
            target->getCard("blockade")->setCardQuantity(target->getCard("blockade")->getCardQuantity() + 1);
            cout << typeOfExchange << " a blockade card!" << endl;
            return true;
        }
        else
        {
            cout << "Failed to " << typeOfExchange << " a blockade card, source collection does not have any blockade cards." << endl;
            return false;
        }
    }
    else if (!strcmp(type.c_str(), "airlift"))
    {
        if (source->getCard("airlift")->getCardQuantity() > 0)
        {
            source->getCard("airlift")->setCardQuantity(source->getCard("airlift")->getCardQuantity() - 1);
            target->getCard("airlift")->setCardQuantity(target->getCard("airlift")->getCardQuantity() + 1);
            cout << typeOfExchange << " a airlift card!" << endl;
            return true;
        }
        else
        {
            cout << "Failed to " << typeOfExchange << " a airlift card, source collection does not have any airlift cards." << endl;
            return false;
        }
    }
    else if (!strcmp(type.c_str(), "diplomacy"))
    {
        if (source->getCard("diplomacy")->getCardQuantity() > 0)
        {
            source->getCard("diplomacy")->setCardQuantity(source->getCard("diplomacy")->getCardQuantity() - 1);
            target->getCard("diplomacy")->setCardQuantity(target->getCard("diplomacy")->getCardQuantity() + 1);
            cout << typeOfExchange << " a diplomacy card!" << endl;
            return true;
        }
        else
        {
            cout << "Failed to " << typeOfExchange << " a diplomacy card, source collection does not have any diplomacy cards." << endl;
            return false;
        }
    }
    else
    {
        cout << "Invalid name type supplied in exchange!" << endl;
             return false;
    }
    return false;
}

// Helper method to display a given card collection - executed whenever a deck/hand is displayed
void CardsCollection::displayCollection(string title)
{
    cout << "Displaying " << title << ":\nbomb: " << this->getCard("bomb")->getCardQuantity()
         << "\nreinforcement: " << this->getCard("reinforcement")->getCardQuantity()
         << "\nblockade: " << this->getCard("blockade")->getCardQuantity()
         << "\nairlift: " << this->getCard("airlift")->getCardQuantity()
         << "\ndiplomacy: " << this->getCard("diplomacy")->getCardQuantity()
         << endl;
}

Card *CardsCollection::getCard(string type)
{
    if (!strcmp(type.c_str(), "bomb"))
    {
        return this->bomb;
    }
    else if (!strcmp(type.c_str(), "reinforcement"))
    {
        return this->reinforcement;
    }
    else if (!strcmp(type.c_str(), "blockade"))
    {
        return this->blockade;
    }
    else if (!strcmp(type.c_str(), "airlift"))
    {
        return this->airlift;
    }
    else if (!strcmp(type.c_str(), "diplomacy"))
    {
        return this->diplomacy;
    }
    else
    {
        cout << "Invalid card type supplied in getCard!\n"
             << endl;
    }
    return new Card();
}
