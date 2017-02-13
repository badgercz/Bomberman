//
// Created by badger on 6/13/16.
//

#include "network.h"

NetworkGame::NetworkGame() { }

void NetworkGame::InitNetwork(const char * srvName, int srvPort) {
    char srvPortText[10];
    snprintf( srvPortText, sizeof(srvPortText), "%d", srvPort );
    if( getaddrinfo(srvName,srvPortText,NULL,&ai) != 0){
        throw InitException("Bad getaddrinfo.");
    }
    s = socket ( ai -> ai_family, SOCK_STREAM, 0 );
    if ( s == -1 ){
        freeaddrinfo ( ai );
        throw InitException("Bad socket.");
    }
    InitMode();
}

GameServer::GameServer(std::string path){
    mapName = path;
}

void GameServer::InitMode() {
    if ( bind ( s, ai -> ai_addr, ai -> ai_addrlen ) != 0 ){
        close ( s );
        freeaddrinfo ( ai );
        throw InitException("Bad bind.");
    }
    if ( listen ( s, 1 ) != 0 ){
        close ( s );
        freeaddrinfo ( ai );
        throw InitException("Bad listen.");
    }
    freeaddrinfo ( ai );
    struct sockaddr addr;
    socklen_t addrLen = sizeof( addr );
    std::cout << "Waiting for another player." << std::endl;
    clientSock = accept ( s, &addr, &addrLen );
}

void GameServer::UpdateHUD() const {
    mvwprintw(HUDplayer1, 0, 0, "Player 1: HP = %d BombCount = %d ", player1->GetHP(), player1->GetBombCnt());
    mvwprintw(HUDplayer2, 0, 0, "Player 2: HP = %d BombCount = %d ", clientPlayer->GetHP(), clientPlayer->GetBombCnt());

}

void GameServer::InterpretMapFormat(int i, int j, int bombermen) {
    Player *pl;
    pl = new Player(0, GameWin, 'R', i, j);
    clientPlayer = pl;
}

GameClient::GameClient(std::string path){
    mapName = path;
}

void GameClient::InitMode() {
    if ( connect( s, ai->ai_addr, ai->ai_addrlen ) != 0 ) {
        throw InitException("Bad connect");
    }
}

void GameClient::UpdateHUD() const {
    mvwprintw(HUDplayer1, 0, 0, "Player 1: HP = %d BombCount = %d ", player1->GetHP(), player1->GetBombCnt());
    mvwprintw(HUDplayer2, 0, 0, "Player 2: HP = %d BombCount = %d ", serverPlayer->GetHP(), serverPlayer->GetBombCnt());
}

void GameClient::InterpretMapFormat(int i, int j, int bombermen) {
    Player *pl;
    pl = new Player(0, GameWin, 'R', i, j);
    serverPlayer = pl;
}

void GameServer::GameLoop() {
    AllWindows.push_back(HUDplayer1 = derwin(HUD, 1, (CO - 2) / 2, 1, 1));
    AllWindows.push_back(HUDplayer2 = derwin(HUD, 1, (CO - 2) / 2, 1, (CO - 2) / 2));
    nodelay(GameWin, TRUE);
    int time = 0;
    int key;
    run = true;
    bool run2 = true;
    char integer[4];
    do {
        key = wgetch(GameWin);
        *((int *) integer) = key;
        send(clientSock, integer, 4, 0);
        run = player1->HumanControl(key, time, BricksByCoord, player1, clientPlayer);
        recv(clientSock, integer, 4, 0);
        key = *((int *) integer);
        run2 = clientPlayer->HumanControl(key, time, BricksByCoord, player1,clientPlayer);
        UpdateHUD();
        RenderScene();
        time++;
        usleep(10000);
    } while (run&&run2);
    close(s);
    delete player1;
    delete clientPlayer;
}

void GameClient::GameLoop() {
    AllWindows.push_back(HUDplayer1 = derwin(HUD, 1, (CO - 2) / 2, 1, 1));
    AllWindows.push_back(HUDplayer2 = derwin(HUD, 1, (CO - 2) / 2, 1, (CO - 2) / 2));
    nodelay(GameWin, TRUE);
    int time = 0;
    int key;
    run = true;
    bool run2;
    char integer[4];
    do {
        recv(s, integer, 4, 0);
        key = *(int *) integer;
        run = player1->HumanControl(key, time, BricksByCoord,player1, serverPlayer);

        key = wgetch(GameWin);
        *((int *) integer) = key;
        send(s, integer, 4, 0);
        run2 = serverPlayer->HumanControl(key, time, BricksByCoord, player1, serverPlayer);
        UpdateHUD();
        RenderScene();
        time++;
        usleep(10000);
    } while (run && run2);
    close(s);
    delete player1;
    delete serverPlayer;
}
