//
// Created by badger on 6/13/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_NETWORK_H
#define BOMBERMAN_SEMESTRALKA_NETWORK_H

#include "enviroment.h"

/**
* Abstract class for the network-remoted games
*/

class NetworkGame : public AbstractGame{
public:
    NetworkGame();
    /**
    * Function inits the network-based game
    *
    * @param[in] address
    * @param[in] socket
    * @throw InitException if something gets wrong
    */ 
    void InitNetwork(const char *, int);

    /**
    * Function init the server/client part of the init process
    */
    
    virtual void InitMode() = 0;

protected:
    addrinfo * ai;
    int s;
};

/**
* Class which runs game server and provides tools that server needs
*/

class GameServer : public NetworkGame{
public:
    GameServer(std::string);
    virtual void InitMode();
    virtual void GameLoop();
private:
    virtual void UpdateHUD() const;
    virtual void InterpretMapFormat(int,int,int);
    Player * clientPlayer;
    int clientSock;
};

/**
* Class which runs game client and provides tools that client needs
*/

class GameClient : public NetworkGame{
public:
    GameClient(std::string);
    virtual void InitMode();
    virtual void GameLoop();
protected:
    virtual void UpdateHUD() const;
    virtual void InterpretMapFormat(int, int, int);
    Player * serverPlayer;
};



#endif //BOMBERMAN_SEMESTRALKA_NETWORK_H
