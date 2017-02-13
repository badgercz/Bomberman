//
// Created by badger on 5/26/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_ENVIROMENT_H
#define BOMBERMAN_SEMESTRALKA_ENVIROMENT_H

#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "character.h"
#include "brick.h"
#include "walls.h"
#include "exceptions.h"

/**
 * Class provides the enviroment for game seassion - window with game objects.
 *
 * The main part of the enviroment class is game loop method. Class also generates the map from the file.
 */

class AbstractGame {
public:
    AbstractGame();

    /**
    * Constructor
    * 
    * @param[in] path with the map file
    */

    AbstractGame(std::string);

    virtual ~AbstractGame();

    /**
    * Initial function that inits ncurses and makes ready terminal for the game
    */
    virtual void TerminalInit();

    /**
    * Game Loop method starts the loop where the game takes inputs from player/s.
    */
    virtual void GameLoop() = 0;

protected:

	/**
	* Method that draws the game scene and makes it updated.
	*/
    virtual void RenderScene() const;

    /**
    * Method that updates HUD in the bottom of screen. 
    */

    virtual void UpdateHUD() const = 0;

    /**
    * Significant member function that transforms file represented level to the game.
    *
    * @throw InitException if something gets wrong.
    */
    virtual void LoadMap();

    /**
    * There are the maps that game has to interpret.
    *
    * There is a lot of game modes but it uses the same map files. The game has to interpret correctely all of them
    */
    virtual void InterpretMapFormat(int, int, int) = 0;

    std::string mapName;
    WINDOW *GameWin;
    WINDOW *HUD;
    WINDOW *HUDplayer1;
    WINDOW *HUDplayer2;
    std::vector<WINDOW *> AllWindows;
    std::map<std::pair<int, int>, GeneralBrick *> BricksByCoord;
    Player *player1;
    int LIN;
    int CO;
    bool run;
};

/**
* Class is base for non-network-based games.
*/

class LocalGame : public AbstractGame {
public:
    LocalGame(std::string);
};

/**
* Class provides tools for single player games. Easy and hard both.
*/

class SinglePlayer : public LocalGame {
public:
	/**
	* Constructor with bool that represents AI level
	*/
    SinglePlayer(std::string,bool);

    virtual void GameLoop();

protected:
    virtual void UpdateHUD() const;

    virtual void InterpretMapFormat(int, int, int);

    EnemyBomberman *enemy;

    bool easy;
};

/**
* Class provides tools for one-computer multiplayer.
*/

class SplitScreen : public LocalGame {
public:
    SplitScreen(std::string);

    virtual void GameLoop();

protected:
    virtual void UpdateHUD() const;

    virtual void InterpretMapFormat(int, int, int);

    Player *player2;

};

#endif //BOMBERMAN_SEMESTRALKA_ENVIROMENT_H
