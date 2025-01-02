#include <iostream>
#include <utility>
#include <string>
using namespace std;

// Yes I COULD make a function for this, but this isn't meant to be good!
#define win_condition (moves&292)==292||(moves&146)==146||(moves&73)==73||(moves&448)==448||(moves&56)==56||(moves&7)==7||(moves&273)==273||(moves&84)==84
#define lose_condition (opp_moves&292)==292||(opp_moves&146)==146||(opp_moves&73)==73||(opp_moves&448)==448||(opp_moves&56)==56||(opp_moves&7)==7||(opp_moves&273)==273||(opp_moves&84)==84
#define draw_condition board==511

constexpr static int bits[9] = {1,2,4,8,16,32,64,128,256};

// Fairly typical minimax algo, just made it constexpr so it saves time!
// Bitwise operations explained below!
constexpr int findminmax(int board, int moves, int depth, bool is_max) {
    int available_moves = 511&(~board);
    int opp_moves = board&(~moves);
    
    if (win_condition)  return 100-depth;
    if (lose_condition) return -100+depth;
    if (draw_condition) return 0;
    
    int best_score = is_max ? -1000 : 1000;
    for (int i = 0; i < 9; ++i) {
        if ((bits[i]&available_moves)==bits[i]) {
            board = board|bits[i];
            moves = moves|bits[i];
            opp_moves = board&(~moves);       
            
            int new_score = findminmax(board,opp_moves,depth+1,!is_max);
            if (is_max && new_score > best_score) best_score = new_score;
            else if(!is_max && new_score < best_score)best_score = new_score;
            
            board = board&(~bits[i]);
            moves = moves&(~bits[i]);
            opp_moves = board&(~moves);
        }
    }
    
    return best_score;
}

// Similarly typical algorithm, bitwise operations explained below!
constexpr int find_next_move(int board, int moves) {
    int available_moves = 511&(~board);
    int opp_moves = board&(~moves);
    if (win_condition || lose_condition || draw_condition) return -1;
    int max_score = -1000;
    int next_move = 0;
    for (int i = 0; i < 9; ++i) {
        // Check available moves
        if ((bits[i]&available_moves)==bits[i]) {
            board = board|bits[i];
            moves = moves|bits[i];
            int minmax = findminmax(board,moves,0,true);
            board = board&(~bits[i]);
            moves = moves&(~bits[i]);
            if (minmax > max_score) {
                max_score = minmax;
                next_move = i;
            }
        }
    }
    return next_move;
}

// lookuptable[ai moves][board state]
// God bless C++17, with constexpr lambdas! Saved me a lot of annoying template metaprogramming
// Finds the optimal move for all possible games
// Could DEFINITELY optimize this, if you force it to play the game properly and thus limit possible AI moves
constexpr auto lookuptable = [] {
    constexpr auto table_size = 0;
    std::array<std::array<int,table_size>,table_size> arr = {};
    
    for (int i = 1; i < table_size; ++i) {
        for (int j = 1; j < table_size; ++j) {
            arr[j][i] = find_next_move(i,j);
        }
    }
    
    return arr;
}();

// Bitwise operations explained here! 

// The board and moves are represented as integers
// It uses its binary value to figure out where Xs and Os are
// for the board variable, 1s represent either an X or an O
// for the moves variable, 1s represent the moves of a particular player
// the moves of the other player are therefore represented as board AND (NOT moves)
// moves are added by flipping a specific bit, hence the bit array at the beginning of the program
// This makes it really easy to generate a table, because the board variable & moves variable represent unique IDs!

// This prints out the board based on above rules!
void print_board(int board, int moves, string player, string ai) {
    int opp_moves = board&(~moves);
    
    for (int i = 0; i < 9; ++i) {
        if ((bits[i]&moves)==bits[i]) {
            cout << ai << " ";
        }
        else if ((bits[i]&opp_moves)==bits[i]) {
            cout << player << " ";
        }
        else {
            cout << "- ";
        }
        if ((i+1)%3==0) cout << endl;
    }
    cout << endl;
}

int main () {
    int moves = 0b000001001;
    int board = 0b000011101;
    
    print_board(board,moves,"x","o");
    
    cout << lookuptable[moves][board] << endl;
}
