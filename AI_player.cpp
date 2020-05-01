#include "AI_player.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

double AI_player::update_explore_rate_decay() {
    EXPLORE_RATE_DECAY = (EXPLORE_RATE / iterations) / 3;
    return EXPLORE_RATE_DECAY;
}

void AI_player::save_qtable(std::vector<std::vector<double>> &q_table, std::string filename) {
    std::ofstream myFile(filename);
    // Send data to the stream
    for(unsigned int i = 0; i < q_table.at(0).size(); ++i)
    {
        for(unsigned int j = 0; j < q_table.size(); ++j)
        {
            myFile << q_table.at(j).at(i);
            if(j != q_table.size() - 1) myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }

    // Close the file
    myFile.close();



}
//std::vector<std::vector<double>> AI_player::load_qtable() {
void AI_player::load_qtable() {

    std::vector<std::vector<double>> q_table_load;
     // Read file
    ifstream file("../q_tabletest");
    if (file.is_open()) { // If file has correctly opened...
         // Output debug message
         cout << "File correctly opened" << endl;
            string line;
            while (getline(file, line)) {
                stringstream sep(line);
                string field;
                q_table_load.push_back(vector<double>());
                while (getline(sep, field, ',')) {
                    q_table_load.back().push_back(stod(field));
                }
            }
            q_table = q_table_load;
     }     
     else cout << "Unable to open file" << endl;

}


std::vector<int> AI_player::input_to_state()
{
   std::vector<int> states(4);
    for (int i = 0; i < 4; i++) {
        int state = position[i] + 1;
        // If token moves past goal position
        if (state > 57) {
            state = 58;
        }
        state += 58 * i;
        states[i] = state;
    }
    return(states);
}


std::vector<int> AI_player::get_actions()
{
    std::vector<int> actions;
    for (int i = 0; i < 4; i++) {
        action = 0;
        int start_pos = position[i];
        int new_pos = start_pos + dice;
        // Move out of home
        if (start_pos == -1 && dice == 6) {
            action = 1;
         // Check if token is stuck in home or is in goal
        } else if (start_pos != -1 && start_pos != 99) {
            // Move into goal
            if (new_pos == 56 || new_pos == 50)
                action = 7;

            // Form blockade
            for (int j = 0; j < 4; j++) {
                if (new_pos == position[j] && i != j && position[j] != 99) {
                    action = 5;
                    break;
                }
            }

            // Move onto same field as an enemy
            bool kill = false;
            bool suicide = false;
            for (int j = 4; j < 16; j++) {
                if (new_pos == position[j] && start_pos != -1 && start_pos != 99) {
                    kill = true;
                    // Check if enemy token is on globe (Suicide)
                    if ((position[j] - 8) % 13 == 0 || (position[j] - 8) % 18 == 0) {
                        suicide = true;
                        break;
                    }
                    // Check if enemy has formed a blockade (Suicide)
                    for (int k = 0; k < 4; k++) {
                        int index = int(j/4) * k;
                        if (new_pos == position[index] && index != j) {
                            suicide = true;
                            break;
                        }
                    }
                }
            }
            // Protect token (goal stretch or globe)
            if ((new_pos > 50 && start_pos < 50) ||
                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)) {
                action = 6;
            }
            // Suicide
            if (suicide) {
                action = 4;
            // Kill
            } else if (kill && !suicide) {
                action = 3;
            }
            // Move token
            if (action == 0 && new_pos > start_pos && action != 1) {
                action = 2;
            }
        }
        actions.push_back(action);
    }
    return actions;
}



int AI_player::select_action(std::vector<int> states, std::vector<int> possible_actions)
{
    int best_action = 0;
    if (EXPLORE_RATE == 0 || (double)(rand() % 1000) / 1000.0 > EXPLORE_RATE) {
        double max_q = -10000;
        for (int i = 0; i < 4; i++) {
            if (position[i] > 55 || (position[i] == -1 && dice != 6)) {
                continue;
            }
            if (q_table[possible_actions[i]][states[i]] > max_q && possible_actions[i] != 0) {
                max_q = q_table[possible_actions[i]][states[i]];
                best_action = i;
            }
        }
    // Random action
    } else {
        bool token_out_of_home = false;
        for (int i = 0; i < 4; i++) {
            if (position[i] != -1 && position[i] != 99) {
                token_out_of_home = true;
                break;
            }
        }
        while (true) {
            best_action = rand() % 4;
            if (position[best_action] < 56) {
                if (position[best_action] != -1 && token_out_of_home) {
                    break;
                } else if (!token_out_of_home) {
                    break;
                }
            }
        }
    }
    // Make sure that best_action is not moving a token in goal
    while(position[best_action] == 99) {
        best_action++;
        best_action = best_action % 4;
    }
    return best_action;

}

void AI_player::get_reward(int action, int state, int decision) {
    // Move out of home
    if (previous_action == 1) {
        reward += 0.25;
    }

    // Move piece closest to home
    if (previous_action != 0 && previous_action != 1 && previous_action != 4) {
        bool closest = true;
        for (int i = 0; i < 4; i++) {
            if (pos_end_of_turn[decision] < pos_end_of_turn[i] && decision != i && pos_end_of_turn[i] != 99) {
                    closest = false;
            }
        }
        if (closest && pos_end_of_turn[decision] < 51) {
            reward += 0.1;
        }
    }
    // Kill
    if (previous_action == 3) {
        reward += 0.15;
    }
    // Form Blockade
    if (previous_action == 5) {
        reward += 0.05;
    }
    // Protect token
    if (previous_action == 6) {
        reward += 0.2;
    }
    // Move into goal
    if (previous_action == 7) {
        reward += 0.25;
    }
    // Suicide
    if (previous_action == 4) {
        reward -= 0.8;
    }
    // Getting a token knocked home
    for (int i = 0; i < 4; i++) {
        if (position[i] == -1 && pos_end_of_turn[i] != -1) {
            reward -= 0.25;
        }
    }
    if (EXPLORE_RATE < 0) {
        EXPLORE_RATE = 0;
    }
    // Update q-table
    if (reward != 0) {
        q_table[previous_action][previous_state] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_table[action][state]
             - q_table[previous_action][previous_state]);
    }
    previous_state = state;
    previous_action = action;
    reward = 0;
}


void AI_player::you_won(bool has_won) {
    game_won = int(has_won);
    // Winning the game
    if (game_won == 1) {
        if (EXPLORE_RATE > 0)
            EXPLORE_RATE -= EXPLORE_RATE_DECAY;
        games_played++;
        reward += 1;
    } else {
        // Losing the game
        if (EXPLORE_RATE > 0) {
            EXPLORE_RATE -= EXPLORE_RATE_DECAY;
        }
        games_played++;
        reward -= 1;
    }


    if (EXPLORE_RATE < 0) {
        EXPLORE_RATE = 0;
    }
    // Update q-table
    if (reward != 0) {
        q_table[previous_action][previous_state] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_table[action][state]
             - q_table[previous_action][previous_state]);
    }
    previous_state = state;
    previous_action = action;
    static bool game_saved = false;

    // save the Q table
    if (games_played == iterations - 1 && !game_saved || games_played == test + 10) {
        test = games_played;
        std::string filename = "../Qtable/q_table_" + std::to_string(games_played) + "_" + std::to_string(DISCOUNT_FACTOR) + "_" + std::to_string(LEARNING_RATE);
        save_qtable(q_table, filename);
        game_saved = true;
    }

}

// Print Q table
/*
for ( std::vector<std::vector<int>>::size_type i = 0; i < q_table.size(); i++ )
{
   for ( std::vector<int>::size_type j = 0; j < q_table[i].size(); j++ )
   {
      std::cout << q_table[i][j] << ' ';
   }
   std::cout << std::endl;
}
*/

int AI_player::make_decision() {
    static bool first_turn = true;
    if (first_turn) {
        for (int i = 0; i < 4; i++){
            pos_end_of_turn[i] = -1;
        }
        if (training == false) {
            load_qtable();
        }
        first_turn = false;

    }

    update_explore_rate_decay();
    std::vector<int> states = input_to_state();
    std::vector<int> possible_actions = get_actions();
    int decision = select_action(states, possible_actions);

    if (training == true)
    {
        get_reward(possible_actions[decision], states[decision], decision);
    }

    for (int i = 0; i < 4; i++){
        pos_end_of_turn[i] = position[i];
    }

    return decision;
}
