
//
// Created by badger on 6/6/16.
//

#include "brick.h"


GeneralBrick::GeneralBrick() { }

GeneralBrick::GeneralBrick(WINDOW *GameBoard, char name, int i_y, int i_x) {
    board = GameBoard;
    y = i_y;
    x = i_x;
    blocking = false;
    destroyable = true;
    burning = false;
    letter = name;
    wblock = derwin(GameBoard, 1, 1, y, x);
    mvwprintw(wblock, 0, 0, "%c", letter);
}

GeneralBrick::~GeneralBrick() {
    delwin(wblock);
}

void GeneralBrick::Render() const {
    wrefresh(wblock);
}

bool GeneralBrick::Blocking() const {
    return blocking;
}


bool GeneralBrick::Destroyable() const {
    return destroyable;
}


bool GeneralBrick::Burning() const {
    return burning;
}

void GeneralBrick::Burn() {
    letter = '+';
    blocking = false;
    burning = true;
    wprintw(wblock, "%c", letter);
    wrefresh(wblock);
}

void GeneralBrick::UnBurn() {
    letter = ' ';
    blocking = false;
    burning = false;
    mvwprintw(wblock, 0, 0, "%c", letter);
    wrefresh(wblock);

}

WINDOW *GeneralBrick::GetBoard() {
    return board;
}

char GeneralBrick::GetType() {
    return letter;
}
