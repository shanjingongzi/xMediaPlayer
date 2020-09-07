#ifndef PLAYER_H
#define PLAYER_H
#include "avdecode.h"
#include<queue>
#include<mutex>


class Player
{
public:
    virtual void Play();
    virtual void Pause();
    virtual void Replay();
private:
    avdecode *decode;

};

#endif // PLAYER_H
