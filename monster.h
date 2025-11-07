class msSlime{
private:
    int mHealth=20,mStrength=5,mShield=0,mState=0;// the monster's health, strength, shield, state 
    int vShield=5;// the amount of added shield per defence
    double vWeanken=1.05;// amplification of harm when attached "weaken" 
    int weakTag=0;// number of rounds it attaches "weaken";
public:
    void mAttach(int nHarm);
    void mWeaken(int Round);
    void mDefence();
};