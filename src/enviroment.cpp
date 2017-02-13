//
// Created by badger on 5/26/16.
//
#include "enviroment.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

AbstractGame::AbstractGame() {}

AbstractGame::AbstractGame(std::string inputPath) : mapName(inputPath) { }

AbstractGame::~AbstractGame() {
    for (const auto &i : BricksByCoord) {
        delete i.second;
    }
    for (const auto &i : AllWindows) {
        delwin(i);
    }
    endwin();
}

void AbstractGame::RenderScene() const {
    for (const auto &i : AllWindows) {
        wrefresh(i);
    }
    for (const auto &i : BricksByCoord) {
        i.second->Render();
    }
}


void AbstractGame::TerminalInit() {
    initscr();
    LoadMap();
    keypad(GameWin, TRUE);
    resizeterm(LIN, CO);
    AllWindows.push_back(HUD = newwin(3, CO, LIN - 3, 0));
    box(GameWin, ACS_VLINE, ACS_HLINE);
    box(HUD, ACS_VLINE, ACS_HLINE);
    keypad(stdscr, TRUE);
    noecho();
    raw();
    curs_set(0);
    RenderScene();
}

void AbstractGame::LoadMap() {
    std::ifstream MapFile;
    MapFile.open(mapName, std::ifstream::in);
    if(MapFile.bad())
        throw InitException("Bad map file.");
    MapFile >> CO;
    MapFile >> LIN;
    if(CO < 10 || CO > 500 || LIN < 10 || LIN > 500)
        throw InitException("Bad map file.");
    AllWindows.push_back(GameWin = newwin(LIN - 3, CO, 0, 0));
    for (int i = 1; i < LIN - 3; i++)
        for (int j = 1; j < CO - 1; j++) {
            char c;
            while ((c = MapFile.get()) == '\n'); //destroy \n char
            static int bombermen = 0;
            GeneralBrick *tmp;
            switch (c) {
                case 'x':
                    tmp = new Undestroyable(GameWin, 'x', i, j);
                    BricksByCoord[std::make_pair(i, j)] = tmp;
                    break;
                case ' ':
                    tmp = new GeneralBrick(GameWin, ' ', i, j);
                    BricksByCoord[std::make_pair(i, j)] = tmp;
                    break;
                case 'o':
                    tmp = new Destroyable(GameWin, 'o', i, j);
                    BricksByCoord[std::make_pair(i, j)] = tmp;
                    break;
                case 'B':
                    Player *PlayerBrick;
                    if (bombermen == 0) {
                        PlayerBrick = new Player(bombermen, GameWin, 'B', i, j);
                        player1 = PlayerBrick;
                    }
                    else if (bombermen == 1) {
                        InterpretMapFormat(i, j, bombermen);
                    }
                    else {
                        throw InitException("Too many bombermen.");
                    }
                    tmp = new GeneralBrick(GameWin, ' ', i, j);
                    BricksByCoord[std::make_pair(i, j)] = tmp;
                    bombermen++;
                    break;
                default:
                    break;
            }
        }
    MapFile.close();
    return;

}

LocalGame::LocalGame(std::string inputPath) : AbstractGame(inputPath) { }

SinglePlayer::SinglePlayer(std::string inputPath, bool mode) : LocalGame(inputPath) {
	easy = mode;
}

void SinglePlayer::InterpretMapFormat(int i, int j, int bombermen) {
    EnemyBomberman *EnemyBrick;
    EnemyBrick = new EnemyBomberman(GameWin, 'R', i, j);
    enemy = EnemyBrick;
}

void SinglePlayer::UpdateHUD() const {
    mvwprintw(HUDplayer1, 0, 0, "You: HP = %d BombCount = %d ", player1->GetHP(), player1->GetBombCnt());
    mvwprintw(HUDplayer2, 0, 0, "Enemy: HP = %d BombCount = infinite ", enemy->GetHP());
}

void SinglePlayer::GameLoop() {
    AllWindows.push_back(HUDplayer1 = derwin(HUD, 1, (CO - 2) / 2, 1, 1));
    AllWindows.push_back(HUDplayer2 = derwin(HUD, 1, (CO - 2) / 2, 1, (CO - 2) / 2));
    nodelay(GameWin, TRUE);
    int time = 0;
    bool airun = true;
    do {
        run = player1->HumanControl(wgetch(GameWin), time, BricksByCoord, player1, enemy);
        airun = enemy->AI(easy,time, BricksByCoord,player1,enemy);
        UpdateHUD();
        RenderScene();
        time++;
        usleep(10000);

    } while (run && airun);
    delete player1;
    delete enemy;
}


SplitScreen::SplitScreen(std::string inputPath) : LocalGame(inputPath) { }

void SplitScreen::UpdateHUD() const {
    mvwprintw(HUDplayer1, 0, 0, "Player 1: HP = %d BombCount = %d ", player1->GetHP(), player1->GetBombCnt());
    mvwprintw(HUDplayer2, 0, 0, "Player 2: HP = %d BombCount = %d ", player2->GetHP(), player2->GetBombCnt());
}

void SplitScreen::InterpretMapFormat(int i, int j, int bombermen) {
    Player *pl;
    pl = new Player(bombermen, GameWin, 'R', i, j);
    player2 = pl;
}

void SplitScreen::GameLoop() {
    AllWindows.push_back(HUDplayer1 = derwin(HUD, 1, (CO - 2) / 2, 1, 1));
    AllWindows.push_back(HUDplayer2 = derwin(HUD, 1, (CO - 2) / 2, 1, (CO - 2) / 2));
    nodelay(GameWin, TRUE);
    int time = 0;
    int key;
    std::deque<int> buffer1;
    std::deque<int> buffer2;
    bool sp = true;
    do {
        run = true;
        key = wgetch(GameWin);
        if (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN || key == ' ' || key == '\n')
            buffer1.push_back(key);
        else if (key == '4' || key == '6' || key == '8' || key == '2' || key == '5')
            buffer2.push_back(key);

        if (!buffer1.empty())
            key = buffer1.front();
        run = player1->HumanControl(key, time, BricksByCoord, player1, player2);
        if (!buffer2.empty())
            key = buffer2.front();
        sp = player2->HumanControl(key, time, BricksByCoord, player1, player2);
        if (!buffer1.empty())
            buffer1.pop_front();
        if (!buffer2.empty())
            buffer2.pop_front();
        UpdateHUD();
        RenderScene();
        time++;
        usleep(20000);
    } while (run && sp);
    delete player1;
    delete player2;
}

