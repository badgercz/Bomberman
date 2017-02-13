//
// Created by badger on 6/6/16.
//

#ifndef BOMBERMAN_SEMESTRALKA_BRICK_H
#define BOMBERMAN_SEMESTRALKA_BRICK_H

#include <ncurses.h>

/**
 * General Brick that is base class for the most entity in the game.
 *
 * In fact, every game entity is object represented by character.
 */

class GeneralBrick {
public:
    GeneralBrick();

    /**
    * Constructor.
    *
    * @param[in] GameWin the game window where entities are spawned
    * @param[in] letter represents the entity
    * @param[in] y coordinate
    * @param[in] x coordinate
    */

    GeneralBrick(WINDOW *, char, int, int);

    virtual ~GeneralBrick();

    /**
    * Method that render the refresh the entity
    */
    void Render() const;

    /**
    * @return true if entity blocks the move of another. Else false.
    */

    bool Blocking() const;

    /**
    * @return true if the entity could be destroyed
    */

    bool Destroyable() const;

    /**
    * @return true if the entity is in the flame
    */

    bool Burning() const;

    /**
    * Method that makes entity burn. 
    */

    virtual void Burn();

    /**
    * Method that colds the entity to stop burning.
    */

    virtual void UnBurn();

    /**
    * return The letter representing entity
    */

    char GetType();

protected:
	/**
	* Method that is useful for making entity in the another entity
	*
	* @return The parent window pointer
	*/
	
	WINDOW *GetBoard();
    WINDOW *board;
    WINDOW *wblock;
    char letter;
    int y;
    int x;
    bool destroyable;
    bool blocking;
    bool burning;
};


#endif //BOMBERMAN_SEMESTRALKA_BRICK_H
