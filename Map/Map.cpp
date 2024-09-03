#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include "Map.h"
#include "../Player/Player.h"

using std::vector;
using std::list;

//Below we will implement all the abstract functions we declared in the previous Map.h Header File

//========================================Territory Class======================================
//Default constructor
Territory::Territory() {
    territoryName = "Blank";
    continent = nullptr;
    ownerOfTerritory = nullptr;
    armyCount = 0;
    previousOwner = nullptr;
}

//Parameter Constructor
Territory::Territory(Player *ownerOfTerritory, string territoryName, Continent *continent, int armyCount) {
    this->ownerOfTerritory = ownerOfTerritory;
    this->territoryName = territoryName;
    this->continent = continent;
    this->armyCount = armyCount;
    previousOwner = nullptr;
}

Territory::Territory(std::string name) : Territory() {
    this->territoryName = name;
    previousOwner = nullptr;
}

//copy constructor
Territory::Territory(const Territory *territory) {
    this->ownerOfTerritory = territory->ownerOfTerritory;
    this->territoryName = territory->territoryName;
    this->continent = territory->continent;
    this->armyCount = territory->armyCount;
    this->previousOwner = territory->previousOwner;
}

//Override operator
Territory &Territory::operator=(const Territory &territory) {
    if (this == &territory) return *this; // self assignment guard
    this->ownerOfTerritory = territory.ownerOfTerritory;
    this->territoryName = territory.territoryName;
    this->continent = territory.continent;
    this->armyCount = territory.armyCount;
    return *this;
}

//destructor
Territory::~Territory() = default;


//getters and setters
Player *Territory::getTerritoryOwner() {
    return this->ownerOfTerritory;
}

void Territory::setTerritoryOwner(Player *player) {
    if (ownerOfTerritory != nullptr) {
        vector<Territory*> playerTerritories = this->ownerOfTerritory->getTerritories();
        playerTerritories.erase(std::remove_if(playerTerritories.begin(), playerTerritories.end(), [this](Territory* terr) {return terr == this;}), playerTerritories.end());
        this->ownerOfTerritory->setTerritories(playerTerritories);
    }
    player->addTerritory(this);
    ownerOfTerritory = player;
}

string Territory::getTerritoryName() const {
    return territoryName;
}

void Territory::setTerritoryName(string name) {
    this->territoryName = name;
}

Continent *Territory::getContinent() {
    return this->continent;
}

void Territory::setContinent(Continent *pContinent) {
    this->continent = pContinent;
}

int Territory::getArmyCount() const {
    return this->armyCount;
}

void Territory::setArmyCount(int count) {
    this->armyCount = count;
}

vector<Territory *> &Territory::getAdjTerritories() {
    return adjTerritories;
}

void Territory::setAdjTerritories(std::vector<Territory *> &territories) {
    this->adjTerritories = territories;
}

void Territory::addAdjTerritory(Territory *adjTerritory) {
    adjTerritories.push_back(adjTerritory);
}

std::ostream& operator<< (std::ostream& out, Territory& territory) {
    out << "Territory: " << territory.territoryName << ", ";
    out << "Continent: " << (territory.continent == nullptr ? "None" : territory.continent->getContinentName()) << ", ";
    out << "Owner: " << (territory.ownerOfTerritory == nullptr ? "None" : territory.ownerOfTerritory->getName()) << ", ";
    out << "Army Count: " << territory.armyCount << ", ";
    out << "Adjacent Territories: ";
    auto adjTerrIttr = territory.adjTerritories.begin();
    while (adjTerrIttr != territory.adjTerritories.end()) {
        out << (*adjTerrIttr)->territoryName << ", ";
        adjTerrIttr++;
    }
    return out;
}

//========================================Continent Class=========================================

//Default constructor
Continent::Continent() {
    continentName = "blank";
    ownerOfContinent = nullptr;
    bonusArmy = 0;
}

//parameter constructor
//Default constructor
Continent::Continent(string name, int bonus) {
    continentName = name;
    ownerOfContinent = nullptr;
    bonusArmy = bonus;
}

//copy constructor
Continent::Continent(const Continent &continent) {
    this->ownerOfContinent = continent.ownerOfContinent;
    this->continentName = continent.continentName;
    this->continentTerritories = continent.continentTerritories;
    this->adjacentContinents = continent.adjacentContinents;
    this->bonusArmy = continent.bonusArmy;
}

//Assignment operator
Continent &Continent::operator=(const Continent &continent) {
    this->ownerOfContinent = continent.ownerOfContinent;
    this->continentName = continent.continentName;
    this->continentTerritories = continent.continentTerritories;
    this->adjacentContinents = continent.adjacentContinents;
    return *this;
}


//Override == operator
bool Continent::operator==(const Continent &continent) {
    if (continentName == continent.continentName) {
        return true;
    } else {
        return false;
    }
}

//destructor
Continent::~Continent() = default;


//getters and setters
Player *Continent::getContinentOwner() const {
    return this->ownerOfContinent;
}

void Continent::setContinentOwner(Player *player) {
    this->ownerOfContinent = player;
}

string Continent::getContinentName() const {
    return this->continentName;
}

void Continent::setContinentName(string name) {
    this->continentName = name;
}

vector<Territory *> Continent::getContinentTerritories() {
    return this->continentTerritories;
}

void Continent::setContinentTerritories(vector<Territory *> territories) {
    this->continentTerritories = territories;
}

void Continent::addContinentTerritory(Territory * territory) {
    this->continentTerritories.push_back(territory);
}

vector<Continent *> Continent::getAdjacentContinents() {
    return this->adjacentContinents;
}

void Continent::setAdjacentContinents(vector<Continent *> continents) {
    this->adjacentContinents = continents;
}

void Continent::addAdjacentContinents(Continent *adjacentContinent) {
    adjacentContinents.push_back(adjacentContinent);
}

int Continent::getArmyBonus() const {
    return this->bonusArmy;
}

void Continent::setArmyBonus(int bonus) {
    this->bonusArmy = bonus;
}

//functions

std::ostream& operator<< (std::ostream& out, Continent& continent) {
    out << "Continent: " << continent.continentName << ", ";
    out << "Bonus Army: " << continent.bonusArmy << ", ";
    out << "Owner: " << (continent.ownerOfContinent == nullptr ? "None" : continent.ownerOfContinent->getName()) << ", ";
    out << "Adjacent Continents: ";
    auto adjContIttr = continent.adjacentContinents.begin();
    while (adjContIttr != continent.adjacentContinents.end()) {
        out << (*adjContIttr)->continentName << ", ";
        adjContIttr++;
    }
    out << "Number of territories: " << continent.continentTerritories.size();
    return out;
}

void Continent::checkIfOwner() {
    // get the player of the first territory, if they are the same throughout, they own the continent
    // else no one owns the continent
    Player* possibleOwner = this->continentTerritories[0]->getTerritoryOwner();
    bool owns = true; // assume owner
    for (auto terr : continentTerritories) {
        if (!owns)
            break;
        owns = (terr->getTerritoryOwner() == possibleOwner);
    }
    if (owns)
        this->ownerOfContinent = possibleOwner;
    else
        this->ownerOfContinent = nullptr;
}

//========================================Map Class=========================================

//default constructor
Map::Map() = default;

//parameter constructor
Map::Map(int vertCount) {
    this->vertCount = vertCount;
}

//copy constructor 
Map::Map(const Map *map) {
    this->vertCount = map->vertCount;
}

//assignment operator
Map &Map::operator=(const Map &map) {
    this->vertCount = map.vertCount;
    return *this;
}

//destructor
Map::~Map() {}

// getters and setters
int Map::getVertCount() const {
    return this->vertCount;
}

void Map::setVertCount(int count) {
    this->vertCount = count;
}

std::vector<Territory*> Map::getAllTerritories() {
    return this->territoryObjects;
}

void Map::setAllTerritories(std::vector<Territory*> territories) {
    this->territoryObjects = territories;
}

std::vector<Continent*> Map::getContinents() {
    return this->continentObjects;
}

void Map::setContinents(std::vector<Continent*> continents) {
    this->continentObjects = continents;
}

// functions
//verify if all territories are connected, all continents are connected, territories belong to one continent
bool Map::validate() {
    list<Territory*> territoryOpenList;
    list<Territory*> territoryClosedList;
    list<Continent*> continentOpenList;
    list<Continent*> continentClosedList;

    // territory BFS, at the end, closed list should be the same size as the total list of territories (vertCount)
    territoryOpenList.push_back(territoryObjects[0]); // start at first node
    while (!territoryOpenList.empty()){
        auto currTerritory = territoryOpenList.front();
        territoryOpenList.pop_front();
        auto adjTerrVect = currTerritory->getAdjTerritories();
        auto ittr = adjTerrVect.begin();
        while (ittr != adjTerrVect.end()) {
            // check if in open list
            bool exists = false;
            auto adjTerrIttr = find(territoryOpenList.begin(), territoryOpenList.end(), (*ittr));
            if (adjTerrIttr != territoryOpenList.end()) { // found, we can skip this
                exists = true;
            }
            // check closed list
            adjTerrIttr = find(territoryClosedList.begin(), territoryClosedList.end(), (*ittr));
            if (adjTerrIttr != territoryClosedList.end()) { // found, we can skip this
                exists = true;
            }
            // if we did not find this territory, it is new and therefore should be added to the list;
            if (!exists) territoryOpenList.push_back(*ittr);
            ittr++;
        }
        territoryClosedList.push_back(currTerritory); // add visited node to closed list
    }
    if (territoryClosedList.size() == vertCount) {
        terrConnected = true;
        cout << "This is a connected graph" << endl;
    }
    else { cout << "This is not a connected graph " << territoryClosedList.size() << "!=" << vertCount << endl; }

    // continent BFS, same procedure as before
    continentOpenList.push_back(continentObjects[0]); // first node
    while (!continentOpenList.empty()){
        auto currContinent = continentOpenList.front();
        continentOpenList.pop_front();
        auto adjContVect = currContinent->getAdjacentContinents();
        for (auto continent : adjContVect) {
            // check if in open list
            bool exists = false;
            auto adjTerrIttr = find(continentOpenList.begin(), continentOpenList.end(), continent);
            if (adjTerrIttr != continentOpenList.end()) { // found, we can skip this
                exists = true;
            }
            // check closed list
            adjTerrIttr = find(continentClosedList.begin(), continentClosedList.end(), continent);
            if (adjTerrIttr != continentClosedList.end()) { // found, we can skip this
                exists = true;
            }
            // if we did not find this continent, it is new and therefore should be added to the list;
            if (!exists) continentOpenList.push_back(continent);
        }
        continentClosedList.push_back(currContinent); // add visited node to closed list
    }
    if (continentClosedList.size() == continentObjects.size()) {
        contConnected = true;
        cout << "This is a connected subgraph" << endl;
    }
    else { cout << "This is not a connected subgraph " << continentClosedList.size() << "!=" << continentObjects.size() << " Continent Open List: " << continentOpenList.size() << endl; }

    // check that continent territories are unique
    // add up all continent territories and compare to territory list count, both should be the same
    int territoryCount = 0;
    for (auto continent : continentObjects) {
        territoryCount += continent->getContinentTerritories().size();
    }
    if (territoryCount == territoryObjects.size()) {
        singleContTerr = true;
        cout << "All territories are unique and belong to one continent" << endl;
    }
    validated = terrConnected && contConnected && singleContTerr;
    return validated;
}

//This method will parse through a given filepath, and from there read all the territories and continents, and
//populate the Map Object. 
void Map::loadMapFile(const std::string &filePath) {
    vertCount = 0;
    fileName = filePath;
    ifstream inputFile(filePath);
    //if we cant find the file to open, return invalid
    if (!inputFile) {
        cerr << "Invalid Input, failed to open file." << endl;
        return;
    }

    //this will be used to parse through the file line by line and store the lines temporarily
    string line;
    //this will be a temp map of territoryStrings, we will assign this to the map object at the end
    vector<Territory *> objTerritories;

    //this will be our territory equivalents for the continent sub graph
    vector<Continent *> objContinents;

    //we will use this to determine if the file is valid or not, as well as to know whether to start parsing the file for data or not.
    bool parsingTerritories = false;
    bool continentParse = false;

    while (getline(inputFile, line)) {
        //remove leading and trailing whitespaces from the line
        line.erase(line.find_last_not_of(" \t") + 1);

        //skip all empty lines
        if (line.empty()) {
            continue;
        }
        if (line == "[Continents]") {
            continentParse = true;
            continue;
        }

        //check for [Territories] tag
        if (line == "[Territories]") {
            parsingTerritories = true;
            continentParse = false;
            //we are skipping this line to the next one
            continue;
        }
        if (continentParse) {
            istringstream iss(line);
            string continentName;
            string bonus;

            getline(iss, continentName, '=');
            getline(iss, bonus, '=');
            //creating continent object
            Continent *currContinent = new Continent(continentName, stoi(bonus));
            objContinents.push_back(currContinent);
        }

        //if parsingTerritories is true, we will begin paring the territoryStrings
        if (parsingTerritories) {
            istringstream iss(line);
            string territoryName;
            string pixel1;
            string pixel2;
            string continent;
            bool existingTerritory = false;

            //capture values using ',' as a delimiter
            getline(iss, territoryName, ',');
            getline(iss, pixel1, ',');
            getline(iss, pixel2, ',');
            getline(iss, continent, ',');

            // check if that territory was already created by the adjacency to an already visited territory
            Territory *currentTerritory;
            auto territoryIterator = std::find_if(objTerritories.begin(), objTerritories.end(),
                                                  [&territoryName](const Territory *terr) {
                                                      return terr->getTerritoryName() == territoryName;
                                                  });
            if (territoryIterator != objTerritories.end()) {
                // found a territory
                currentTerritory = *territoryIterator;
                existingTerritory = true;
            } else { currentTerritory = new Territory(); }

            // find the continent that contains this territory
            auto continentIterator = std::find_if(objContinents.begin(), objContinents.end(),
                                                  [&continent](const Continent *cont) {
                                                      return cont->getContinentName() == continent;
                                                  });
            if (continentIterator == objContinents.end()) {
                // this should never happen, all continents should be named and created before this point
                cerr << "ERROR: Could not find continent when making territories" << endl;
                parsingTerritories = false;
                break;
            }
            //add territory info
            currentTerritory->setTerritoryName(territoryName);
            currentTerritory->setContinent(*continentIterator);

            //add territory to continent
            (*continentIterator)->addContinentTerritory(currentTerritory);

            //Read adjacent Territories until EOL
            while (getline(iss, territoryName, ',')) {
                // get the territory name, if that territory already exists in our territory list, add the pointer to that territory, else create it ourselves
                territoryIterator = std::find_if(objTerritories.begin(), objTerritories.end(),
                                                 [&territoryName](const Territory *terr) {
                                                     return terr->getTerritoryName() == territoryName;
                                                 });
                if (territoryIterator != objTerritories.end()) {
                    // found a territory, add its pointer to the list of adjacent territories
                    currentTerritory->addAdjTerritory(*territoryIterator);
                    // check if the territory has a continent and that it is different from current territory
                    // if true, check if that continent is in the current territory's continent's adjacency list
                    // and do the same in the other direction
                    auto adjCont = (*territoryIterator)->getContinent();
                    auto currentCont = currentTerritory->getContinent();
                    if (adjCont != nullptr && adjCont->getContinentName() != currentCont->getContinentName()) {
                        // idk why but adding variables to the vectors makes it work (probably some pass by value shit)
                        auto adjContAdjConts = adjCont->getAdjacentContinents();
                        auto currentContAdjConts = currentCont->getAdjacentContinents();

                        // find current continent
                        auto findCurrContIttr = find(adjContAdjConts.begin(), adjContAdjConts.end(), currentCont);
                        // find adjacent continent
                        auto findAdjContIttr = find(currentContAdjConts.begin(), currentContAdjConts.end(), adjCont);
                        if (findCurrContIttr == adjContAdjConts.end()) {
                            // current continent is not in adjacent continent list
                            adjCont->addAdjacentContinents(currentCont);
                        }
                        if (findAdjContIttr == currentContAdjConts.end()) {
                            // adjacent continent is not in current continient list
                            currentCont->addAdjacentContinents(adjCont);
                        }
                    }
                }
                else {
                    // not found, this is a new territory, make a new one and add it to the list of territories and its adjacency list
                    Territory *tempTerritory = new Territory(territoryName);
                    objTerritories.push_back(tempTerritory);
                    currentTerritory->addAdjTerritory(tempTerritory);
                }

            }

            //inserting currentTerritory into territoryStrings if it is a new territory
            if (!existingTerritory) objTerritories.push_back(currentTerritory);
            vertCount++;
        }
    }

    //if we do not find the [Territories] tag, we consider this file to be invalid
    if (!parsingTerritories) {
        cerr << "Invalid map file. Please try again with a valid map file." << endl;
        return;
    }

    //close the file
    inputFile.close();
    //store territories into map object
    territoryObjects = objTerritories;

    //store continent objects
    continentObjects = objContinents;
}

void Map::updateAllContinentOwners(){
    for (auto cont : continentObjects)
        cont->checkIfOwner();
}

std::ostream& operator<< (std::ostream& out, Map& map) {
    auto terrIterator = map.territoryObjects.begin();
    while (terrIterator != map.territoryObjects.end()) {
        out << **terrIterator;
        terrIterator++;
        out << endl;
    }
    auto contItterator = map.continentObjects.begin();
    while (contItterator != map.continentObjects.end()) {
        out << **contItterator;
        contItterator++;
        out << endl;
    }
    out << "Map Path: " << map.fileName << "\n";
    out << "This map has " << (map.validated ? "" : "NOT ") << "been validated\n";
    out << "Territory(Vertex) Count: " << map.vertCount;
    return out;
}

Player* Territory::getPreviousOwner()
{
    return previousOwner;
}

void Territory::setPreviousOwner(Player* previousOwner)
{
    this->previousOwner = previousOwner;
}
