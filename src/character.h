//
// Created by badger on 5/26/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_CHARACTER_H
#define BOMBERMAN_SEMESTRALKA_CHARACTER_H

#include <ncurses.h>
#include <deque>
#include <map>
#include <cstdlib>

#include "brick.h"
#include "bomb.h"

class Bomb;

/**
 * Base class for all moving characters, both players and AI controlled monsters.
 */

class Moving_Character : public GeneralBrick {
public:
    Moving_Character();

    /**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] letter character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */

    Moving_Character(WINDOW *, char, int, int);

    /**
    * Most important method that remotes the player or the enemy.
    *
    * @param[in] direction represented by int
    * @param[in] map of the whole game world
    *
    * @return true if move is made or false
    */ 

    virtual bool Move(int, std::map<std::pair<int, int>, GeneralBrick *> &);

    /**
    *
    * @return HP value
    */

    virtual int GetHP();

    /**
    *
    * @return Bomb count
    */

    virtual int GetBombCnt();

    virtual void Burn();

    /**
    * Function that updates players and their logic
    *
    * @param[in] time Game time
    * @param[in] map of the whole world
    * @param[in] pointer at player1
    * @param[in] pointer at player2
    */

    virtual bool Action(int, std::map<std::pair<int, int>, GeneralBrick *> &, Moving_Character*, Moving_Character*);

    /**
    * Method drops the bomb at the place of player
    *
    * @param[in] map of the world
    * @return false if player dies, else true
    */

    virtual void DropBomb(std::map<std::pair<int, int>, GeneralBrick *> &);

    /**
    * Method that generates bonus represented by int
    *
    * @return[in] int random generated 
    */

    int GetBonus();

    int GetY();

    int GetX();

protected:
    int HP;
    int BombCount;
    std::deque<Bomb *> inventary;
};

class Player : public Moving_Character {
public:
	/**
    * Constructor.
    *
    * @param[in] id of player if there is a more then one
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */
    Player(int, WINDOW *, char, int, int);

    /**
    * Method that takes the input from user
    *
    * @param[in] key pressed
    * @param[in] time of world
    * @param[in] map of world
    * @param[in] pointer to the player1 
    * @param[in] pointer to the player2
    * @return true if player lives and wants to continue playing
    */
    bool HumanControl(int, int, std::map<std::pair<int, int>, GeneralBrick *> &, Moving_Character*, Moving_Character*);

private:
    int id;
};

class EnemyBomberman : public Moving_Character {
public:
	/**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */
    EnemyBomberman(WINDOW *, char, int, int);

    /**
    * Method that takes the input from user
    *
    * @param[in] bool represents the difficulty 
    * @param[in] time of world
    * @param[in] map of world
    * @param[in] pointer to player1
    * @param[in] pointer to player2
    * @return true if player lives and wants to continue playing
    */

    bool AI(bool, int, std::map<std::pair<int, int>, GeneralBrick *> &, Moving_Character*, Moving_Character*);

private:
    int state;
    int next_state;
};

#endif //BOMBERMAN_SEMESTRALKA_CHARACTER_H
