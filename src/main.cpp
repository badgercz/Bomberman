#include "enviroment.h"
#include "network.h"


using namespace std;

/**
* @mainpage Bomberman
* This final work of the subject BI-PA2 implements simplified classic game Bomberman.
*/

/**
* Main function serves as initial function.
*
* Input is taken from shell.
*
* @return 0 for success. 2 for wrong arguments, 3 for initial problem.
*/

int main(int argc, char const *argv[])
{
    if(argc < 2 || argc > 4){
        cout << "Wrong arguments count. Type -h or --help for more information." << endl;
        return 2;
    }
    else{
        if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h") ){
            cout << "I guess you need an advice." << endl <<
                    " -se or -sh 'name of map' : just you against computer AI, Easy or Hard" << endl <<
                    " -t 'name of map'         : you against friend in one terminal" << endl <<
                    " -mps 'name of map' 'ip'  : host server and play against friend" << endl <<
                    " -mpc 'name of map' 'ip ' : join server" << endl;
            return 0;
        }
        else if(!strcmp(argv[1], "-se") || !strcmp(argv[1], "-sh")){
           bool easy;
           if(!strcmp(argv[1], "-se"))
           	easy = true;
           else
           	easy = false;
           SinglePlayer game(argv[2],easy);
            try {
                game.TerminalInit();
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            game.GameLoop();
            return 0;
        }
        else if(!strcmp(argv[1], "-t")){
            SplitScreen game(argv[2]);
            try {
                game.TerminalInit();
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            game.GameLoop();
            return 0;
        }
        else if(!strcmp(argv[1], "-mps")){
            GameServer game(argv[2]);
            try {
                game.InitNetwork(argv[3], 7415);
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            try {
                game.TerminalInit();
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            game.GameLoop();
            return 0;
        }
        else if(!strcmp(argv[1], "-mpc")){
            GameClient game(argv[2]);
            try {
                game.InitNetwork(argv[3], 7415);
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            try {
                game.TerminalInit();
            }
            catch (const InitException & e){
                std::cout << e.what() << std::endl;
                return 3;
            }
            game.GameLoop();
            return 0;
        }
        else{
            cout << "Wrong argument. Please, type -h or --help for help." << endl;
            return 2;
        }
    }
}