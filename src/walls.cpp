//
// Created by badger on 6/9/16.
//

#include "walls.h"


Destroyable::Destroyable(WINDOW *GameWin, char l, int y, int x) : GeneralBrick(GameWin, l, y, x) {
    destroyable = true;
    blocking = true;
}

Undestroyable::Undestroyable(WINDOW *GameWin, char l, int y, int x) : GeneralBrick(GameWin, l, y, x) {
    destroyable = false;
    blocking = true;
}

void Destroyable::Burn() {
    if (letter == ' ')
        GeneralBrick::Burn();
    else {
        letter = '#';
        blocking = false;
        burning = true;
        wprintw(wblock, "%c", letter);
        wrefresh(wblock);
    }
}
