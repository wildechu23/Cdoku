#include <stdio.h>

#include "sudoku.h"

void print_sudoku(char* solution) {
    printf("Solution:\n");
    for(int row = 0; row < 9; row++) {
        char line[9];
        for(int col = 0; col < 9; col++) {
            line[col] = solution[row * 9 + col];
        }
        printf("%s\n", line);
    }
}

int main() {
    FILE* file = fopen("input.txt", "r");
    // char input[82];
    // char solution[81];
    // while(fgets(input, sizeof(input)+1, file)) {
    //     // printf("%s\n", input);
    //     // if(solve(input, solution) == 1) print_sudoku(solution);
    //     solve(input,solution);
    // }
    solve_file(file);
}