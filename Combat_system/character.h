#ifndef CHARACTER_H
#define CHARACTER_H

#include "utils.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

using namespace std;

struct NoteEffect {
    string name;
    string description;
    int power;
    string color;
    NoteEffect(const string &n,const string &desc,int p,const string &c):name(n),description(desc),power(p),color(c){}
};

struct Rune {
    string name;
    vector<Note> sequence;
    string effect;
    int power;
    string color;
    Rune(const string &n,const vector<Note> &seq,const string &e,int p,const string &c):name(n),sequence(seq),effect(e),power(p),color(c){}
};

class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int resonance;
    int defense;
    vector<unique_ptr<Rune>> runes;
    vector<Note> melody;
    map<Note,unique_ptr<NoteEffect>> noteEffect;
public:
    Character(const string &n,int h):name(n),health(h),maxHealth(h),resonance(50),defense(0){}
    virtual ~Character()=default;
    Character(const Character&)=delete;
    Character &operator=(const Character&)=delete;
    Character(Character&&)=default;
    Character &operator=(Character&&)=default;
    string getName()const;
    int getHealth()const;
    int getMaxHealth()const;
    int getResonance()const;
    int getDefense()const;
    void takeDamage(int damage);
    void heal(int amount);
    void changeResonance(int amount);
    void setDefense(int def);
    void addRune(unique_ptr<Rune> rune);
    const vector<unique_ptr<Rune>>& getRunes()const;
    void addNoteEffect(Note note,unique_ptr<NoteEffect> effect);
    const NoteEffect *getNoteEffect(Note note)const;
    void addNoteToMelody(Note note);
    void clearMelody();
    const vector<Note>& getMelody()const;
    void takeTurn(Character &target);
    bool isAlive()const;
    //check if the melody suits the rune
    int checkMelody();
    string getHealthBar()const;
    string getResonanceBar()const;
    string getMelodyDisplay()const;
    // virtual void resetTurn();
    void applyNoteEffect(Note note,Character &target);
};

#endif