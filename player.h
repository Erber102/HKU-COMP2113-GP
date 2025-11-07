class player{
private:
    int pHealth,pStrength,pShield,pState;// the player's health, strength, shield, state 
    int vShield;// the amount of added shield per defence 
    int weakTag;// number of rounds it attaches "weaken";
public:
    void pAttach();
    void pWeaken();
    void pDefence();
};