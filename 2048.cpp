#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <numeric> // needed for accumulate in compute_score

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    for (const auto &row : board) {
        for (auto val : row) {
            if (val == 0) cout << ".\t";
            else cout << val << "\t";
        }
        cout << "\n";
    }
    cout << endl;
    write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly
void spawn_tile(vector<vector<int>>& board) {
    vector<pair<int, int>> empty_cells;
    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++)
            if (board[r][c]==0) {empty_cells.push_back({r, c});}

    if (empty_cells.empty()) return;  // No empty cells, can't spawn

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> cell_dist(0, empty_cells.size()-1);
    int random_cell = cell_dist(gen);
    auto generate_cell = empty_cells[random_cell];
    int row = generate_cell.first;
    int col = generate_cell.second;

    std::uniform_int_distribution<int> num_dist(1, 10);
    int num_chance = num_dist(gen);

    if(num_chance <= 9){board[row][col] = 2;}
    else{board[row][col] = 4;}
}

// TODO: implement using STL algorithms
bool move_left(vector<vector<int>>& board){
    bool moved = false;
    
    for(int r=0; r<4; r++){
        vector<int> og_row = board[r];
        vector<int> compressed;
        for(int value : og_row){
            if(value != 0){
            compressed.push_back(value);
            }
        }

        if (compressed.size() > 1) {  
            for (int i = 0; i < (int)compressed.size()-1; i++){
                if (compressed[i] == compressed[i+1]){
                    compressed[i] *= 2;
                    compressed[i+1] = 0;
                    i++;
                }
            }
        }
        
        vector<int> new_row_after_merge;
        for (int value : compressed){
            if (value != 0){
                new_row_after_merge.push_back(value);
            }
        }

        while(new_row_after_merge.size() < 4){
            new_row_after_merge.push_back(0);
        }

        board[r] = new_row_after_merge;
        if (og_row != new_row_after_merge){
            moved = true;
        }
    }
    return moved;
}
bool move_right(vector<vector<int>>& board){
    bool moved = false;
    
    for(int r=0; r<4; r++){
        vector<int> og_row = board[r];
        reverse(og_row.begin(), og_row.end());

        vector<int> compressed;
        for(int value : og_row){
            if(value != 0){
            compressed.push_back(value);
            }
        }

        if(compressed.size() > 1){
            for (int i = 0; i < compressed.size()-1; i++){
                if (compressed[i] == compressed[i+1]){
                    compressed[i] *= 2;
                    compressed[i+1] = 0;
                    i++;
                }
            }
        }

        vector<int> new_row_after_merge;
        for (int value : compressed){
            if (value != 0){
                new_row_after_merge.push_back(value);
            }
        }

        while(new_row_after_merge.size() < 4){
            new_row_after_merge.push_back(0);
        }

        reverse(new_row_after_merge.begin(), new_row_after_merge.end());

        board[r] = new_row_after_merge;
        if (og_row != new_row_after_merge) {moved = true;}
    }
    return moved;
}
bool move_up(vector<vector<int>>& board){
    bool moved = false;
    
    for(int c=0; c<4; c++){
        vector<int> og_col = {board[0][c], board[1][c], board[2][c], board[3][c]};
        vector<int> compressed;
        for(int value : og_col){
            if(value != 0){
            compressed.push_back(value);
            }
        }

        if(compressed.size() > 1){
            for (int i = 0; i < compressed.size()-1; i++){
                if (compressed[i] == compressed[i+1]){
                    compressed[i] *= 2;
                    compressed[i+1] = 0;
                    i++;
                }
            }
        }

        vector<int> new_col;
        for (int value : compressed){
            if (value != 0){
                new_col.push_back(value);
            }
        }

        while(new_col.size() < 4){
            new_col.push_back(0);
        }

        board[0][c] = new_col[0];
        board[1][c] = new_col[1];
        board[2][c] = new_col[2];
        board[3][c] = new_col[3];

        if (og_col != new_col){
            moved = true;
        }
    }
    return moved;
}
bool move_down(vector<vector<int>>& board){
    bool moved = false;
    
    for(int c=0; c<4; c++){
        vector<int> og_col = {board[0][c], board[1][c], board[2][c], board[3][c]};
        reverse(og_col.begin(), og_col.end());
        vector<int> compressed;
        for(int value : og_col){
            if(value != 0){
            compressed.push_back(value);
            }
        }

        if(compressed.size() > 1){
            for (int i = 0; i < compressed.size()-1; i++){
                if (compressed[i] == compressed[i+1]){
                    compressed[i] *= 2;
                    compressed[i+1] = 0;
                    i++;
                }
            }
        }

        vector<int> new_col;
        for (int value : compressed){
            if (value != 0){
                new_col.push_back(value);
            }
        }

        while(new_col.size() < 4){
            new_col.push_back(0);
        }

      
        reverse(new_col.begin(), new_col.end());

        board[0][c] = new_col[0];
        board[1][c] = new_col[1];
        board[2][c] = new_col[2];
        board[3][c] = new_col[3];
        
        if (og_col != new_col){
            moved = true;
        }
    }
    return moved;
}

// TODO: compute score by summing board values
int compute_score(const vector<vector<int>>& board) {
    int sum = 0;
    for (int r=0; r<4; r++){
        for (int c=0; c<4; c++){
            sum += board[r][c];
        }
    }
    return sum;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    history.push(board);
    bool first=true;

    while(true){
        print_board(board, first);
        write_board_csv(board, first);
        first=false;
        cout << "Score: " << compute_score(board) << "\n";
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: Implement undo handling here using the history stack
            if(!history.empty()){
                board = history.top();
                history.pop();
            } else{
                cout << "No previous moves found." << endl;
            }
            print_board(board,false);
            continue;
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: push prev to history stack for undo
            history.push(prev);
            spawn_tile(board);
        }
    }
    return 0;
}
