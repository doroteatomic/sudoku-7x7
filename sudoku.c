#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 7

int grid[SIZE][SIZE];
int initial[SIZE][SIZE];

int blockMap[SIZE][SIZE] = {
    {0, 0, 1, 1, 1, 1, 2},
    {0, 0, 0, 1, 1, 1, 2},
    {3, 0, 0, 4, 4, 2, 2},
    {3, 3, 4, 4, 4, 2, 2},
    {3, 3, 4, 4, 5, 5, 2},
    {3, 6, 6, 6, 5, 5, 5},
    {3, 6, 6, 6, 6, 5, 5}
};

int rowMask[SIZE], colMask[SIZE], blkMask[SIZE];

static inline int numToBit(int n) { return 1 << (n - 1); }
static inline int getBlock(int r, int c) { return blockMap[r][c]; }

int candidatesMask(int r, int c) {
    int used = rowMask[r] | colMask[c] | blkMask[getBlock(r, c)];
    int all = (1 << SIZE) - 1;
    return all & ~used;
}

void place(int r, int c, int num) {
    int b = numToBit(num);
    grid[r][c] = num;
    rowMask[r] |= b;
    colMask[c] |= b;
    blkMask[getBlock(r, c)] |= b;
}

void unplace(int r, int c, int num) {
    int b = numToBit(num);
    grid[r][c] = 0;
    rowMask[r] &= ~b;
    colMask[c] &= ~b;
    blkMask[getBlock(r, c)] &= ~b;
}

void shuffleArray(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}




int solveBB(int r, int c, int randomize) {
    if (r == SIZE) return 1;
    if (c == SIZE) return solveBB(r + 1, 0, randomize);
    if (grid[r][c] != 0) return solveBB(r, c + 1, randomize);

    int mask = candidatesMask(r, c);
    if (mask == 0) return 0;

    int nums[SIZE];
    int count = 0;
    for (int num = 1; num <= SIZE; num++)
        if (mask & numToBit(num)) nums[count++] = num;

    if (randomize) shuffleArray(nums, count);

    for (int i = 0; i < count; i++) {
        place(r, c, nums[i]);
        if (solveBB(r, c + 1, randomize)) return 1;
        unplace(r, c, nums[i]);
    }

    return 0;
}

void generateRandomInitial(int cellsToRemove) {
    memset(grid, 0, sizeof(grid));
    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(blkMask, 0, sizeof(blkMask));

    solveBB(0, 0, 1);
    memcpy(initial, grid, sizeof(grid));

    while (cellsToRemove > 0) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (initial[r][c] != 0) {
            initial[r][c] = 0;
            cellsToRemove--;
        }
    }
}

void printGrid(int g[SIZE][SIZE], int showDots) {
    const char* colors[] = {
        "\033[91m", "\033[92m", "\033[93m", "\033[94m",
        "\033[95m", "\033[96m", "\033[90m", "\033[38;5;208m"
    };
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int block = blockMap[r][c] % 8;
            if (showDots && g[r][c] == 0)
                printf("%s. \033[0m", colors[block]);
            else
                printf("%s%d \033[0m", colors[block], g[r][c]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    generateRandomInitial(35);
    memcpy(grid, initial, sizeof(grid));

    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(blkMask, 0, sizeof(blkMask));

    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (grid[r][c] != 0) {
                int b = numToBit(grid[r][c]);
                rowMask[r] |= b;
                colMask[c] |= b;
                blkMask[getBlock(r, c)] |= b;
            }

    printf("Original Sudoku:\n");
    printGrid(initial, 1);
    printf("\n");

    if (solveBB(0, 0, 0)) {
        printf("Solved Sudoku:\n");
        printGrid(grid, 0);
    }
    else {
        printf("Sudoku couldn't be solved.\n");
    }

    return 0;
}


