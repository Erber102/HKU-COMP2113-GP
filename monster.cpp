#include "monster.h"
#include "player.h"

void msSlime::mAttach(int nHarm){// monster being attach
    if(nHarm>msSlime::mShield){
        msSlime::mShield=0;
        nHarm-=msSlime::mShield;
    }
    else {
        msSlime::mShield-=nHarm;
        nHarm=0;
    }
    msSlime::mHealth-=nHarm;
}

void msSlime::mWeaken(int Round){// monster being weaken
    msSlime::weakTag+=Round;
}

void msSlime::mDefence(){// monster defensing itself
    msSlime::mShield+=msSlime::vShield;
    return ;
}