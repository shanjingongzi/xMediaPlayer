#ifndef PLAYER_H
#define PLAYER_H
#include "avdecode.h"
#include<queue>
#include<mutex>


class Player
{
public:
    Player();
private:
    avdecode *decode;

};

#endif // PLAYER_H
