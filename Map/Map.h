#pragma once
#include <iostream>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

class Continent; // Forward reference
class Player;

using namespace std;

//Territory class will be used to create territory objects, will have functions to handle said objects as well
class Territory{
private:
    //this will be the player object that holds the territory
    Player* ownerOfTerritory;
    //this will hold the name of the territory itself
    std::string territoryName;
    //this will hold pointer to continent it belongs to
    Continent* continent;
    //this will hold the army number
    int armyCount;
    //this variable will hold all adjacent territories of this territory object
    std::vector<Territory*> adjTerritories;
    // keep track of previous owner after neutral player takes over
    Player* previousOwner;

    friend std::ostream& operator<< (std::ostream& out, Territory& territory);

public:
    //Default Constructor
    Territory();
    //Constructor with only territory name
    Territory(std::string name);
    //Constructor to set the private variables (PLayer, territoryName, Continent, armyCount)
    Territory(Player* player, std::string name, Continent* continent, int army);
    //Copy constructor 
    Territory(const Territory* t);
    Territory &operator=(const Territory& t);
    //destructor 
    ~Territory();

    //getters and setters
    Player* getTerritoryOwner();
    void setTerritoryOwner(Player*);
    Player* getPreviousOwner();
    void setPreviousOwner(Player* previousOwner);
    std::string getTerritoryName() const;
    void setTerritoryName(std::string);
    Continent* getContinent();
    void setContinent(Continent*);
    int getArmyCount() const;
    void setArmyCount(int);
    vector<Territory *>& getAdjTerritories();
    void setAdjTerritories(std::vector<Territory *> &territories);
    void addAdjTerritory(Territory *adjTerritory);

};

//continent class
class Continent{
private:
    //this will be the player object that holds the territory (for now the placeholder player class)
    Player* ownerOfContinent;
    //this will hold the name of the continent itself
    std::string continentName;
    //this variable will hold all adjacent territories of this territory object
    std::vector<Territory*> continentTerritories;
    //this variable will hold all adjacent territories of this territory object
    std::vector<Continent*> adjacentContinents;
    // bonus army for owning whole continent
    int bonusArmy;

    friend std::ostream& operator<< (std::ostream& out, Continent& continent);

public:
    //Default Constructor
    Continent();
    //parameter constructor
    Continent(std::string name, int bonus);
    //Copy constructor
    Continent(const Continent&);
    Continent &operator=(const Continent&);
    bool operator==(const Continent&);
    //destructor
    ~Continent();

    //getters and setters
    Player* getContinentOwner() const;
    void setContinentOwner(Player*);
    std::string getContinentName() const;
    void setContinentName(std::string);
    vector<Territory *> getContinentTerritories();
    void setContinentTerritories(std::vector<Territory*>);
    void addContinentTerritory(Territory* territory);
    vector<Continent *> getAdjacentContinents();
    void setAdjacentContinents(std::vector<Continent*>);
    void addAdjacentContinents(Continent*);
    int getArmyBonus() const;
    void setArmyBonus(int bonus);
    void checkIfOwner();

    //helper functions

};
//The above 2 classes essentially are just object classes, they contain getters and setters and doesn't do much else except define objects
//This class will be where all the functionality of part one lies.
class Map{
private:
    //this will keep count of the number of vertices encountered
    int vertCount = 0;
    //flags on map state
    bool validated = false; // true if the map was successfully validated
    bool terrConnected = false; // true if all territories are connected (set during validation)
    bool contConnected = false; // true if all continents are connected (set during validation)
    bool singleContTerr = false; // true if all territories belong to one continent (set during validation)
    //Input filename
    std::string fileName;
    //vector all object territories
    std::vector<Territory*> territoryObjects;

    //set for all continent objects
    std::vector<Continent*> continentObjects;


    friend std::ostream& operator<< (std::ostream& out, Map& map);
    

public:
    //default constructor
    Map();
    //constructor that accepts number of vertices
    Map(int);
    //copy constructor
    Map(const Map*);
    //Override '=' operator for assignment
    Map& operator = (const Map&);
    //Destructor
    ~Map();

   //getters and setters
   int getVertCount() const;
   void setVertCount(int count);
   std::vector<Territory*> getAllTerritories();
   void setAllTerritories(std::vector<Territory*> territories);
   std::vector<Continent*> getContinents();
   void setContinents(std::vector<Continent*> continents);
   void updateAllContinentOwners();

   //Will be used to verify whether an edge is connected in a graph or not
   bool validate();
   //this function will populate the array of territoryNames with territory names
   void loadMapFile(const std::string &path);
};

inline void testLoadMaps() {
    //init filePath
    std::string filePath = "001_I72_Ghtroc 720 .txt";
    //initializing Map object
    Map* map = new Map();
    //Loading map from filePath
    map->loadMapFile(filePath);
    // print out
    cout << *map << endl;
    //validating map
    map->validate();
    // print again
    cout << *map << endl;
    //delete map object
    delete map;
}