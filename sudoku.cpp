#include "sudoku.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>


using namespace std;

bool valid_input(string Line, int Puzzle[9][9]){
    if(Line.size() != 5) return false;
    if(!isdigit(Line[0]) || !isdigit(Line[2]) || !isdigit(Line[4])){
        cout << "Invalid Input! Try again! Example: '4 6 7'" << endl;
        return false;
    }

    int num, row, column;
    stringstream ss(Line);
    ss >> num >> row >> column;
    if(num > 9 || num < 1){
        cout << "Invalid number! Number must be between 1 and 9 inclusive! Try again." << endl;
        return false;
    }
    if(row > 9 || row < 1){
        cout << "Invalid row! Row must be between 1 and 9 inclusive! Try again." << endl;
        return false;
    }
    if(column > 9 || column < 1){
        cout << "Invalid column! Column must be between 1 and 9 inclusive! Try again." << endl;
        return false;
    }
    if(Puzzle[row - 1][column - 1] != 0){
        cout << "Invalid Input! The number at row " << row << " and column " << column << " is given, and so, it cannot be changed!" << endl;
        return false;
    }
    return true;
}

bool check(int Grid[9][9]){

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            for(int k = 0; k < 9; k++){
                if ((j != k && Grid[i][j] == Grid[i][k] && Grid[i][j] != 0 && Grid[i][k] != 0) || (i != k && Grid[i][j] == Grid[k][j] && Grid[i][j] != 0 && Grid[k][j] != 0)) return false;
            }
        }
    }
    for(int h = 0; h < 3; h++){
        for(int i = 0; i < 3; i++){
            int num[9], n = 0;
            for(int j = 0; j < 3; j++){
                for(int k = 0; k < 3; k++){
                    num[n] = Grid[j+3*h][k+3*i];
                    n++;
                }
            }
            for(int j = 0; j < 9; j++) for(int k = j; k < 9; k++) if(k != j && num[j] == num[k] && num[j] != 0 && num[k] != 0) return false;
        }
    }
    return true;
}

bool solved(int Game[9][9]){
    bool cond = true;
    for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++) if(Game[i][j] == 0) cond = false;
    
    if(check(Game) && cond) return true;
    return false;
}

bool fill(int Grid[9][9], int i, int j) {
    if (i == 9) return true;

    int ni = (j == 8) ? i + 1 : i;
    int nj = (j == 8) ? 0 : j + 1;

    int nums[9] = {1,2,3,4,5,6,7,8,9};

    for (int k = 0; k < 9; k++) {
        int r = rand() % 9;
        int temp = nums[k];
        nums[k] = nums[r];
        nums[r] = temp;
    }

    for (int k = 0; k < 9; k++) {
        Grid[i][j] = nums[k];

        if (check(Grid) && fill(Grid, ni, nj))
            return true;
    }

    Grid[i][j] = 0;
    return false;
}

void generate_grid(int Grid[9][9]){
    srand(time(0));
    fill(Grid, 0, 0);
}


void generate_puzzle(int Grid[9][9], int Puzzle[9][9]){
    for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++) Puzzle[i][j] = Grid[i][j];
    int missing_prob = (rand() % 9) + 36;
    int count = missing_prob * 0.8, counter = 0;
    while(counter < count){
        for(int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                int miss = (rand() % 81) + 1;
                if(miss > missing_prob && Puzzle[i][j] != 0){
                    Puzzle[i][j] = 0;
                    counter++;
                }
            }
        }
    }
}

void print(int Game[9][9]){
    cout << endl << "     1   2   3    4   5   6    7   8   9   " << endl << endl;
    cout << "  ||===|===|===||===|===|===||===|===|===||" << endl;
    for(int i = 0; i < 9; i++){
        cout << i + 1 << " ||";
        for(int j = 0; j < 9; j++){
            cout << " " << Game[i][j] << (((j + 1) % 3 == 0) ? " ||" : " |");
        }
        cout << endl;
	cout << (((i + 1) % 3 == 0) ? "  ||===|===|===||===|===|===||===|===|===||" : "  ||---|---|---||---|---|---||---|---|---||") << endl;
    }
    cout << "  _________________________________________" << endl;
    cout << endl;
    cout << "Input format: 'number' 'row' 'column'" << endl;
    cout << "Enter 'quit' if you wanna give up." << endl;
}

void type_text(string text, int delay_ms = 40) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

int sudoku(int code2){
    int grid[9][9] = {0}, puzzle[9][9] = {0}, game[9][9] = {0};
    
    type_text("Your challenge is being generated...");
    this_thread::sleep_for(chrono::seconds(2));

    generate_grid(grid);

    type_text("Almost there...");
    this_thread::sleep_for(chrono::seconds(3));

    generate_puzzle(grid, puzzle);

    type_text("It's ready! Enjoy if you dare!!! MUAHAHAHAHAHAHAHA");
    this_thread::sleep_for(chrono::seconds(2));

    for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++) game[i][j] = puzzle[i][j];

    print(game);
    
    while(!solved(game)){
        int number, row, column;
        string line;
        getline(cin, line);
        if(line == "Quit" || line == "quit") break;
        else if(valid_input(line, puzzle)){
            stringstream ss(line);
            ss >> number >> row >> column;
            game[row - 1][column - 1] = number;
            print(game);
            continue;
        }
        else continue;
    }
    
    if(solved(game)){
        print(game);
        type_text("CONGRATULATIONS! You have DONE IT!!! Here's the reward of your unmatched Intellect:); 
        cout << "Code2 = " << code2 << endl;
        type_text("Best of Luck Traveller!");
        return 1;
    }
    return 0;
}
