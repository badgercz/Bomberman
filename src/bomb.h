//
// Created by badger on 6/11/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_BOMB_H
#define BOMBERMAN_SEMESTRALKA_BOMB_H

#include <map>

#include "character.h"
#include "brick.h"
#include "walls.h"

class Moving_Character;

/**
* Functor making the fire.
*/

class Flame {
public:
    Flame();

    /**
    * Method detonates the bomb and makes the world burn
    *
    * @param[in] y coord of epicentrum
    * @param[in] x coord of epicentrum
    * @param[in] range of bomb
    * @param[in] map world map
    * @param[in] pointer to player1
    * @param[in] pointer to player2
    */
    void Detonate(int, int, int, std::map<std::pair<int, int>, GeneralBrick *> &, Moving_Character*, Moving_Character*);

    /**
    * Method remove the fire
    *
    * @param[in] map world map
    */    

    void Remove(std::map<std::pair<int, int>, GeneralBrick *> &);

private:
    int range;
    int bx;
    int by;
};

/**
* Class represents the dangerous bomb
*/

class Bomb : public Destroyable {
public:	
    /**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] letter character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */
    Bomb(WINDOW *, char, int, int);

    /**
    *
    * Method check the bomb if still ticking
    * @param[in] time world time
    * @param[in] map world map
    * @param[in] pointer to player1
    * @param[in] pointer to player2
    * 
    */

    bool Check(int, std::map<std::pair<int, int>, GeneralBrick *> &, Moving_Character*, Moving_Character*);

private:
    int countdown;
    int range;
    int timer;
    Flame fire;

};


#endif //BOMBERMAN_SEMESTRALKA_BOMB_H
