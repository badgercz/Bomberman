//
// Created by badger on 5/26/16.
//
#include <unistd.h>
#include "character.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STARTPACK 20

Moving_Character::Moving_Character() {

}

Moving_Character::Moving_Character(WINDOW *GameWin, char l, int y, int x) : GeneralBrick(GameWin, l, y, x) {
    destroyable = true;
    blocking = true;
}

bool Moving_Character::Move(int direction, std::map<std::pair<int, int>, GeneralBrick *> &around) {
    if (((y == 1 || around[std::make_pair(y - 1, x)]->Blocking()) && direction == UP) ||
        ((y == LINES - 5 || around[std::make_pair(y + 1, x)]->Blocking()) && direction == DOWN) ||
        ((x == 1 || around[std::make_pair(y, x - 1)]->Blocking()) && direction == LEFT) ||
        ((x == COLS - 3 || around[std::make_pair(y, x + 1)]->Blocking()) && direction == RIGHT)) {
        return false;
    }
    if (((around[std::make_pair(y - 1, x)]->Burning()) && direction == UP) ||
        ((around[std::make_pair(y + 1, x)]->Burning()) && direction == DOWN) ||
        ((around[std::make_pair(y, x - 1)]->Burning()) && direction == LEFT) ||
        ((around[std::make_pair(y, x + 1)]->Burning()) && direction == RIGHT) ||
            ((around[std::make_pair(y, x + 1)]->Burning()) && direction == RIGHT)) {
        HP--;
    }
    if ((around[std::make_pair(y - 1, x)]->GetType()=='g' && direction == UP) ||
        (around[std::make_pair(y + 1, x)]->GetType()=='g' && direction == DOWN) ||
        (around[std::make_pair(y, x - 1)]->GetType()=='g' && direction == LEFT) ||
        (around[std::make_pair(y, x + 1)]->GetType()=='g' && direction == RIGHT)
            ) {
        int receive = GetBonus();
        if(receive == 0)
            HP++;
        else
            BombCount++;
    }
    wclear(wblock);
    wrefresh(wblock);

    switch (direction) {
        case UP:
            mvwin(wblock, --y, x);
            break;
        case DOWN:
            mvwin(wblock, ++y, x);
            break;
        case LEFT:
            mvwin(wblock, y, --x);
            break;
        case RIGHT:
            mvwin(wblock, y, ++x);
            break;
        default:
            break;
    }
    wprintw(wblock, "%c", letter);
    wrefresh(wblock);
    return true;
}

int Moving_Character::GetBonus(){
    return rand()%2;
}

bool Moving_Character::Action(int time, std::map<std::pair<int, int>, GeneralBrick *> &around, Moving_Character* p1, Moving_Character*p2) {
    if (!inventary.empty())
        for (const auto &i : inventary)
            i->Check(time, around,p1,p2);
    if(time%100==0){
        int ry = rand()%(COLS-3) + 3;
        int rx = rand()%(LINES-10)+3;
        if(around.find({ry,rx}) != around.end() && around[std::make_pair(ry,rx)]->GetType() == ' ' ) {
            GeneralBrick * tmp = new GeneralBrick(board, 'g', ry, rx);
            around[std::make_pair(ry, rx)] = tmp;
        }
    }
    if(HP==0)
        return false;
    return true;
}

int Moving_Character::GetY(){
    return y;
}

int Moving_Character::GetX(){
    return x;
}

Player::Player(int nmb, WINDOW *GameWin, char l, int y, int x) : Moving_Character(GameWin, l, y, x) {
    HP = 3;
    BombCount = STARTPACK;
    id = nmb;
}

bool Player::HumanControl(int c, int time, std::map<std::pair<int, int>, GeneralBrick *> &around, Moving_Character* p1, Moving_Character* p2) {
    if(!Action(time, around,p1,p2 )){
        wclear(board);
        wrefresh(board);
        mvwprintw(board, LINES/2 , COLS/2 , "Player %d LOST", id+1);
        wrefresh(board);
        usleep(2000000);
        return false;
    }

    int key = c;
    if (id == 1) {
        switch (key) {
            case '4':
                Move(LEFT, around);
                break;
            case '6':
                Move(RIGHT, around);
                break;
            case '8':
                Move(UP, around);
                break;
            case '2':
                Move(DOWN, around);
                break;
            case '5':
                if (BombCount == 0)
                    break;
                else {
                    DropBomb(around);
                }
                break;
            default:
                Move(0, around);
        }
    }
    if (id == 0) {
        switch (key) {
            case KEY_LEFT:
                Move(LEFT, around);
                break;
            case KEY_RIGHT:
                Move(RIGHT, around);
                break;
            case KEY_UP:
                Move(UP, around);
                break;
            case KEY_DOWN:
                Move(DOWN, around);
                break;
            case 'q':
                return false;
            case ' ':
                if (BombCount == 0)
                    break;
                else {
                    DropBomb(around);
                }
                break;
            default:
                Move(0, around);
        }
    }
    return true;
}

EnemyBomberman::EnemyBomberman(WINDOW *GameWin, char l, int y, int x) : Moving_Character(GameWin, l, y, x) {
    HP = 3;
    BombCount = 8;
    state = (rand() % 4) + 1;
}

bool EnemyBomberman::AI(bool easy, int time, std::map<std::pair<int, int>, GeneralBrick *> &around, Moving_Character * p1, Moving_Character * p2) {
    int move;
    int bomb_frequency;
    if(easy){
    	move = 10;
    	bomb_frequency = 200;
    }
    else{
    	move = 3;
    	bomb_frequency = 80;
    }

    if (time < 20 || time % move != 0)
        return true;
    if (!inventary.empty())
        for (const auto &i : inventary)
            i->Check(time, around, p1, p2);

    if (time % bomb_frequency == 0) {
        DropBomb(around);
    }
    next_state = (rand() % 4 + 1);
    if (!Move(state, around)) {
        state = next_state;
    }
    if(time%rand()==0){
    	state = next_state;
    }
    if(HP==0){
        wclear(board);
        wrefresh(board);
        mvwprintw(board, LINES/2 , COLS/2 , "Enemy LOST");
        wrefresh(board);
        usleep(2000000);
        return false;
    }
    return true;
}

int Moving_Character::GetHP() {
    return HP;
}

int Moving_Character::GetBombCnt() {
    return BombCount;
}

void Moving_Character::DropBomb(std::map<std::pair<int, int>, GeneralBrick *> &around) {
    BombCount--;
    inventary.push_back(new Bomb(board, ' ', y, x));
    around[std::make_pair(y, x)] = inventary.back();

}

void Moving_Character::Burn(){
    HP--;
}


