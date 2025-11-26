#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <map>

struct Location { // define location
    std::string id;         // inner ID, e.g., "city_ruins"
    std::string name;       // name, "ruined city"
    int dangerLevel;        // danger level (1-5)

    std::map<std::string, double> lootTable; // probability of dropping item

    double enemyChance;       
    double specialEventChance;
    double emptyChance;
};

class Map {
private:
    // store location template
    std::vector<Location> locationPrototypes;

    // actually generated location on that day
    std::vector<Location> dailyLocations;
    
    // track already explored location, and you can't explore the same location two times
    std::vector<bool> locationCompleted;

    void initializePrototypes(); // load location template

public:
    Map(); // call initializePrototypes

    void generateDailyMap(int currentDay); // refresh location

    std::vector<Location*> getAvailableLocations(); // get the list of location, used in location menu

    void completeLocation(Location* loc); // remark a explored location
};
#endif