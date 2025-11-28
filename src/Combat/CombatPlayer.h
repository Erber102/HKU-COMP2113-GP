#ifndef COMBATPLAYER_H
#define COMBATPLAYER_H

#include "character.h"
#include <functional>

class CombatPlayer:public Character{
private:
    int harmony;
    int combo;
    int extraTurns;
    int weaponBonusDamage;
    std::string equippedWeaponName;
    bool weaponNeedsAmmo;
    int weaponDurability;
    std::function<int()> consumeHealingItemFunc;
    std::function<bool()> consumeAmmoCallback;
    std::function<void()> openBackpackCallback;
    std::function<void()> weaponUsageCallback;
    bool handleInput(char key,Character &target);
    int getWeaponBonusDamage();
    void activeRune(int runeIndex,Character &target);
    void applyRuneEffect(const Rune &rune,Character &target);
    void rest();
    void showNoteInputMenu();
    void applyNoteEffect(Note note,Character &target);
public:
    CombatPlayer(const string &name);
    void setWeaponBonus(int bonus, const std::string& weaponName, bool needsAmmo = false, int durability = -1);
    void setHealingCallback(std::function<int()> func);
    void setAmmoCallback(std::function<bool()> func);
    void setBackpackCallback(std::function<void()> func);
    void setWeaponUsageCallback(std::function<void()> func);
    void increaseHarmony();
    void increaseCombo();
    void resetCombo();
    int getCombo();
    int getHarmony();
    bool hasExtraTurns();
    void useExtraTurn();
    void takeTurn(Character &target);
    void resetTurn();
    bool activateRune(Character &target);
};

#endif