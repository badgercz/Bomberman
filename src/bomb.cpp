//
// Created by badger on 6/11/16.
//

#include <unistd.h>
#include "bomb.h"

Bomb::Bomb(WINDOW *GameWin, char l, int y, int x) : Destroyable(GameWin, l, y, x) {
    timer = 9;
    countdown = 5;
    range = 3;
}

bool Bomb::Check(int time, std::map<std::pair<int, int>, GeneralBrick *> &around, Moving_Character*p1,Moving_Character*p2 ) {
    wrefresh(wblock);
    if (timer == 0 && time % countdown == 0) {
        wprintw(wblock, " ");
        wrefresh(wblock);
        blocking = false;
        fire.Remove(around);
        return true;
    }
    if (timer == 1) {
        usleep(100000);
        wprintw(wblock, "%d", timer);
        timer--;
        fire.Detonate(y, x, range, around, p1, p2);
    }
    if (timer > 0 && time % countdown == 0) {
        timer--;
        wprintw(wblock, "%d", timer);
        return false;
    }
    else
        return false;
}

Flame::Flame() {
}

void Flame::Detonate(int y, int x, int Brange, std::map<std::pair<int, int>, GeneralBrick *> &around, Moving_Character* p1, Moving_Character* p2) {
    range = Brange;
    bx = x;
    by = y;
    for (int i = 1; i < range + 1; ++i) {
        if (p1->GetY() == y - i && p1->GetX() == x){
            p1->Burn();
        }
        if (p2->GetY() == y - i && p2->GetX() == x){
            p2->Burn();
        }
        if (around[std::make_pair(y - i, x)]->Destroyable())
            around[std::make_pair(y - i, x)]->Burn();
        else
            break;
    }

    for (int i = 1; i < range + 1; ++i) {
        if(p1->GetY() == y + i && p1->GetX() == x){
            p1->Burn();
        }
        if(p2->GetY() == y + i && p2->GetX() == x){
            p2->Burn();
        }
        if (around[std::make_pair(y + i, x)]->Destroyable())
            around[std::make_pair(y + i, x)]->Burn();
        else
            break;
    }

    for (int i = 1; i < range + 1; ++i) {
        if(p1->GetY() == y && p1->GetX() == x+i){
            p1->Burn();
        }
        if(p2->GetY() == y - i && p2->GetX() == x+i){
            p2->Burn();
        }
        if (around[std::make_pair(y, x + i)]->Destroyable())
            around[std::make_pair(y, x + i)]->Burn();
        else
            break;
    }

    for (int i = 1; i < range + 1; ++i) {
        if(p2->GetY() == y && p2->GetX() == x-i){
            p2->Burn();
        }
        if(p1->GetY() == y && p1->GetX() == x-i){
            p1->Burn();
        }
        if (around[std::make_pair(y, x - i)]->Destroyable())
            around[std::make_pair(y, x - i)]->Burn();
        else
            break;
    }
}

void Flame::Remove(std::map<std::pair<int, int>, GeneralBrick *> &around) {
    for (int i = 1; i < range + 1; ++i) {
        if (around.find(std::make_pair(by - i, bx)) == around.end())
            break;
        if (around[std::make_pair(by - i, bx)]->Burning())
            around[std::make_pair(by - i, bx)]->UnBurn();
    }

    for (int i = 1; i < range + 1; ++i) {
        if (around.find(std::make_pair(by + i, bx)) == around.end())
            break;
        if (around[std::make_pair(by + i, bx)]->Burning())
            around[std::make_pair(by + i, bx)]->UnBurn();
    }

    for (int i = 1; i < range + 1; ++i) {
        if (around.find(std::make_pair(by, bx + i)) == around.end())
            break;
        if (around[std::make_pair(by, bx + i)]->Burning())
            around[std::make_pair(by, bx + i)]->UnBurn();
    }

    for (int i = 1; i < range + 1; ++i) {
        if (around.find(std::make_pair(by, bx - i)) == around.end())
            break;
        if (around[std::make_pair(by, bx - i)]->Burning())
            around[std::make_pair(by, bx - i)]->UnBurn();
    }

}
