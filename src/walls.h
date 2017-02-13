//
// Created by badger on 6/9/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_WALLS_H
#define BOMBERMAN_SEMESTRALKA_WALLS_H

#include "brick.h"


class Undestroyable : public GeneralBrick {
public:

    /**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] letter character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */

    Undestroyable(WINDOW *, char, int, int);

};

class Destroyable : public GeneralBrick {
public:
	
    /**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] letter character represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */

    Destroyable(WINDOW *, char, int, int);

    virtual void Burn();
};

#endif //BOMBERMAN_SEMESTRALKA_WALLS_H
