#include <stdio.h>

#include "sudoku.h"

void print_sudoku(char* solution) {
    for(int row = 0; row < 9; row++) {
        char line[9];
        for(int col = 0; col < 9; col++) {
            line[col] = solution[row * 9 + col];
        }
        printf("%s\n", line);
    }
}

int main() {
    char input[81] = "5.....37....6..............7..54.....4......2...1..6...6..83........2.4...1......";
    // char input[81] = "6..12.384..8459.72.....6..5...264.3..7..8...694...3...31.....5..897.....5.2...19.";
    char solution[81];

    solve_init(input, solution);
    int found = solve(0, solution);
    if(found == 1) {
        print_sudoku(solution);
    }

    solve_cleanup();
}