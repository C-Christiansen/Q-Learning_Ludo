#ifndef AI_PLAYER_H
#define AI_PLAYER_H
#include <iostream>
#include "random"
#include "iplayer.h"
#include <sstream>
#include <fstream>
#include <vector>


class AI_player : public iplayer
{
private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;


public:
    AI_player()
    {
        std::random_device rd;
        generator = std::mt19937(rd());
    }
    virtual void you_won(bool has_won);
private:
    std::vector<std::vector<double>> q_table{8, std::vector<double>(232, 0)};
    std::vector<int> pos_end_of_turn = {-1, -1, -1, -1};
    int test = 0;
    int iterations = 1000000;

    int state =0;
    int action = 0;
    int previous_state = 0;
    int previous_action = 0;
    int games_played = 0;
    double reward = 0;
    int game_won = 0;

    //bool training = false;
    //double EXPLORE_RATE = 0.0;
    bool training = true;
    double EXPLORE_RATE = 0.9;
    double EXPLORE_RATE_DECAY;
    double DISCOUNT_FACTOR = 0.4;
    double LEARNING_RATE = 0.2;
    std::string filename;


    int make_decision();
    double update_explore_rate_decay();
    std::vector<int> input_to_state();
    std::vector<int> get_actions();
    int select_action(std::vector<int> states, std::vector<int> possible_actions);

    void get_reward(int action, int state, int decision);

    void load_qtable();
    void save_qtable(std::vector<std::vector<double>> &q_table, std::string filename);

};

// Actions
// No action 0
// Move out of home 1
// Move 2
// Kill 3
// Suicide 4
// Form Blockade 5
// Protect token 6
// Move into goal 7



#endif // AI_PLAYER_H
