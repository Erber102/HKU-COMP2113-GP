#include "SaveSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

string SaveSystem::saveFileName = "savegame.dat";

void SaveSystem::serializePlayer(ofstream& file, Player& player) {
    // Simplified player serialization
    file.write(reinterpret_cast<const char*>(&player.hp), sizeof(player.hp));
    file.write(reinterpret_cast<const char*>(&player.stamina), sizeof(player.stamina));
    file.write(reinterpret_cast<const char*>(&player.money), sizeof(player.money));
}

bool SaveSystem::deserializePlayer(ifstream& file, Player& player) {
    // Simplified player deserialization
    file.read(reinterpret_cast<char*>(&player.hp), sizeof(player.hp));
    file.read(reinterpret_cast<char*>(&player.stamina), sizeof(player.stamina));
    file.read(reinterpret_cast<char*>(&player.money), sizeof(player.money));

    return !file.fail();
}

void SaveSystem::serializeInventory(ofstream& file, vector<Item*>& inventory) {
    // Compress inventory: count of items with the same name
    std::map<std::string, int> counts;
    for (Item* item : inventory) {
        if (!item) continue;
        counts[item->name] += 1;
    }

    int distinct = static_cast<int>(counts.size());
    file.write(reinterpret_cast<const char*>(&distinct), sizeof(distinct));

    for (const auto& pair : counts) {
        const std::string& name = pair.first;
        int count = pair.second;
        int nameLen = static_cast<int>(name.size());
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        file.write(name.c_str(), nameLen);
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }
}

bool SaveSystem::deserializeInventory(ifstream& file, Player& player) {
    // Clear existing inventory and rebuild items from name and count
    player.clearInventory();

    int distinct = 0;
    file.read(reinterpret_cast<char*>(&distinct), sizeof(distinct));
    if (!file.good() || distinct < 0) return false;

    for (int i = 0; i < distinct; ++i) {
        int nameLen = 0;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        if (!file.good() || nameLen <= 0 || nameLen > 256) return false;

        std::string name(nameLen, '\0');
        file.read(&name[0], nameLen);
        if (!file.good()) return false;

        int count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        if (!file.good() || count < 0) return false;

        for (int j = 0; j < count; ++j) {
            Item* item = createItem(name);
            if (item) {
                player.addItem(item);
            }
        }
    }

    return !file.fail();
}

void SaveSystem::saveGame(Player& player, int day) {
    ofstream file(saveFileName, ios::binary);

    if (!file.is_open()) {
        cerr << "Error: Cannot open save file for writing!" << endl;
        return;
    }

    try {
        // Write file header identifier
        const char* header = "SAVEGAME";
        file.write(header, 8);

        // Write version number
        int version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));

        // Write day count
        file.write(reinterpret_cast<const char*>(&day), sizeof(day));

        // Serialize player
        serializePlayer(file, player);

        // Serialize inventory
        serializeInventory(file, player.getInventory());

        file.close();
        cout << "Game saved successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error saving game: " << e.what() << endl;
    }
}

bool SaveSystem::loadGame(Player& player, int& day) {
    ifstream file(saveFileName, ios::binary);

    if (!file.is_open()) {
        cout << "No save file found!" << endl;
        return false;
    }

    try {
        // Verify file header
        char header[9];
        file.read(header, 8);
        header[8] = '\0';

        if (string(header) != "SAVEGAME") {
            cout << "Invalid save file format!" << endl;
            return false;
        }

        // Read version number
        int version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));

        // Read day count
        file.read(reinterpret_cast<char*>(&day), sizeof(day));

        // Deserialize player
        if (!deserializePlayer(file, player)) {
            cout << "Error loading player data!" << endl;
            return false;
        }

        // Deserialize inventory
        if (!deserializeInventory(file, player)) {
            cout << "Error loading inventory data!" << endl;
            return false;
        }

        file.close();
        cout << "Game loaded successfully!" << endl;
        return true;
    } catch (const exception& e) {
        cerr << "Error loading game: " << e.what() << endl;
        return false;
    }
}

bool SaveSystem::saveExists() {
    ifstream file(saveFileName);
    return file.good();
}

void SaveSystem::deleteSave() {
    if (remove(saveFileName.c_str()) == 0) {
        cout << "Save file deleted!" << endl;
    } else {
        cout << "No save file to delete!" << endl;
    }
}
