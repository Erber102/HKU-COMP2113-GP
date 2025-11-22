#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player:public Character{
private:
    int harmony;
    int combo;
    int extraTurns;
    void handleInput(char key,Character &target);
    void activeRune(int runeIndex,Character &target);
    void applyRuneEffect(const Rune &rune,Character &target);
    void rest();
    void showNoteInputMenu();
    void applyNoteEffect(Note note,Character &target);
public:
    Player(const string &name);
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