#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

typedef struct {
    int row;
    int col;
    int box;
} RCB;

// set 9 bits to 1
const uint32_t ALL_BITS = 0x1ff;

uint32_t rows[9];
uint32_t cols[9];
uint32_t boxes[9];

int num_to_solve;
RCB* cells_to_solve;

// initialize variables
void solve_init(const char* input, char* solution) {
    // fill rows
    for(int i = 0; i < 9; ++i) {
        rows[i] = ALL_BITS;
    }
    // fill cols
    for(int i = 0; i < 9; ++i) {
        cols[i] = ALL_BITS;
    }
    // fill boxes
    for(int i = 0; i < 9; ++i) {
        boxes[i] = ALL_BITS;
    }

    num_to_solve = 0;
    for(int i = 0; i < 81; ++i) {
        if(input[i] == '.') num_to_solve++;
    }

    // allocate
    memcpy(solution, input, 81);
    cells_to_solve = (RCB*)malloc(sizeof(RCB)*num_to_solve);

    // for each input, check if blank or clue
    int i = 0;
    for(int row = 0; row < 9; ++row) {
        for(int col = 0; col < 9; ++col) {
            // current box
            int box = 3 * (row/3) + (col / 3);
            if(input[row * 9 + col] == '.') {
                // blank, create RCB and put in solve list
                RCB rcb;
                rcb.row = row;
                rcb.col = col;
                rcb.box = box;
                cells_to_solve[i++] = rcb;
            } else {
                // clue, remove possibilities
                // read (input[row * 9 + col] - '1') as equivalent 
                // to (input[row * 9 + col] - '0') - 1)
                // i.e. convert to int, - 1
                uint32_t value = 1u << (uint32_t)(input[row * 9 + col] - '1');
                rows[row] ^= value;
                cols[col] ^= value;
                boxes[box] ^= value;
            }
        }
    }
    num_to_solve--;
}

// count number of candidates
int num_candidates(const RCB* rcb) {
    int candidates = rows[rcb->row] & cols[rcb->col] & boxes[rcb->box];
    return __builtin_popcount(candidates); // number of bits set
}

// swaps best cell to solve to the front
void sort_best_cell(int first) {
    int best = first;
    int best_count = num_candidates(&cells_to_solve[best]);
    // find best cell in all remaining
    for (int next = first + 1; best_count > 1 && next < num_to_solve; ++next) {
        int next_count = num_candidates(&cells_to_solve[next]);
        if (next_count < best_count) {
            best_count = next_count;
            best = next;
        }
    }
    // swap best to front
    RCB temp = cells_to_solve[best];
    cells_to_solve[best] = cells_to_solve[first];
    cells_to_solve[first] = temp;
}

// solve the sudoku recursively
int solve(int index, char* solution) {
    sort_best_cell(index);

    // current cell and candidates
    RCB cell = cells_to_solve[index];
    uint32_t candidates = rows[cell.row] & cols[cell.col] & boxes[cell.box];

    // for each candidate
    while(candidates != 0) {
        // get lowest set bit
        uint32_t candidate = candidates & -candidates;

        // guess
        // if (candidates ^ candidate)

        // remove the candidate
        rows[cell.row] ^= candidate;
        cols[cell.col] ^= candidate;
        boxes[cell.box] ^= candidate;
        if (index == num_to_solve || solve(index + 1, solution)) {
            solution[cell.row * 9 + cell.col] = (char)('0'+ __builtin_ffs(candidate));
            return 1;
        }

        // if subsequent solves fail, add candidate back
        rows[cell.row] |= candidate;
        cols[cell.col] |= candidate;
        boxes[cell.box] |= candidate;

        // and clear lowest set bit(remove candidate) before moving to next
        candidates = candidates & (candidates - 1);
    }
    return 0;
}

// free memory
void solve_cleanup() {
    free(cells_to_solve);
}
