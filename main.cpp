#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "AI_player.h"
#include "player_fast.h"
#include "player_aggro_fast.h"
#include <string>
#include <vector>

using namespace std;

int main()
{
    //std::string filename = "../winners" + std::to_string(play_games);
    std::string filename = "../winners_game1";
    std::ofstream myFile(filename);

    //Run the unit tests
    //test_game tester;
    //tester.run_all_tests();

    //Create players
    AI_player player_0;
    player_aggro_fast player_1;
    player_fast player_2;
    player_random player_3;


    //player_random player_1;
    //player_random player_2;
    //player_random player_3;

    //Play a game of Ludo
    game g(&player_0, &player_1, &player_2, &player_3);
    //g.play_game();
    //cout << "Player " << g.get_winner() << " won the game!" << endl << endl;

    int play_games = 10000;
    //Play many games of Ludo
    int wins[] = {0, 0, 0, 0};
    for(int i = 0; i < play_games; i++)
    {
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        //cout << "new game " << endl;
        wins[g.get_winner()]++;

        if (i == play_games - 1){
        myFile << wins[0] << ","<< wins[1] << ","<< wins[2] << ","<< wins[3]<< "\n";
        }
        int j = play_games - i;
        cout << "missing games: " << j<< endl;

    }
    for(int i = 0; i < 4; i++){
        cout << "Player " << i << " won " << wins[i] << " games." << endl;
    }

    myFile.close();

    cout << "End of main" << endl;
    return 0;
}


