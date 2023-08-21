#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helpers.h"

Board board_init(void)
{
    Board b;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            b.board[i][j] = EMPTY;
    b.board[3][3] = A;
    b.board[4][4] = A;
    b.board[3][4] = B;
    b.board[4][3] = B;
    b.complete = false;
    return b;
}

void board_print(Board b)
{
    printf("#");
    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf(" %d", i);
        else 
            printf(" %d", i + 1);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf("%d", i);
        else 
            printf("%d", i + 1);

        for (int j = 0; j < COLS; j++) {
            char c = b.board[i][j];
            switch (c) {
                case A: printf(RED_BOLD); break;
                case B: printf(GREEN_BOLD); break;
                default: printf(RESET);
            }
            printf(" %c", c);
        }
        printf("\n");
    }
    printf(RESET);
}

bool board_complete(Board b)
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (b.board[i][j] == EMPTY)
                return false;
    return true;
}

int **valid_moves_for(char player, Board b)
{
    // Allocate memory
    int **valid = calloc(sizeof(int*), ROWS);
    for (size_t i = 0; i < ROWS; i++)
        valid[i] = calloc(sizeof(int), COLS);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            DBPRINT("On [%d, %d]\n", i, j);

            // Loop through all neighboring cells
            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    DBPRINT("\tNeighbor: [%d, %d]\n", x, y);

                    // Skip invalid indices
                    if (x < 0 || y < 0 || x > ROWS - 1 || y > COLS - 1) {
                        DBPRINT("\t\tinvalid indices\n");
                        continue;
                    }

                    // No need to check this piece
                    if (x == i && y == j) {
                        DBPRINT("\t\tno need to check this poiece\n");
                        continue;
                    }

                    // Continue if neighboring piece is not opposing
                    if (b.board[x][y] == player || b.board[x][y] == EMPTY) {
                        DBPRINT("\t\tNot opposing piece\n");
                        continue;
                    }

                    // Check if we have supporting piece on other side
                    //                  \  |  / 
                    //                   . . .
                    // [0,-1]+p[i,j] <-- . p . -->  p[i,j]+[0,1]
                    //                   . . .
                    //                  /  |  \
                    //                     |
                    //               p[i,j]+[1,0]
                    //
                    // Generally p[i,j] + [inc_x, inc_y]
                    // inc_x given by: x - i
                    // inc_y given by: y - j
                    
                    int inc[] = {x - i, y - j};
                    int start[] = {i, j};
                    bool found = search_line(b.board, player, start, inc);
                    if (found) {
                        DBPRINT("\t\t[%d, %d] is valid\n", i, j);
                        valid[i][j]++;
                    } else {
                        DBPRINT("\t\tNot found any matching piece on other side\n");
                    }
                }
            }
        }
    }

    return valid;
}

bool search_line(char arr[ROWS][COLS], char c, int start[], int inc[])
{
    DBPRINT("\t\tSearching from [%d, %d] with matrix [%d, %d]\n", start[0], start[1], inc[0], inc[1]);

    int i = start[0] + inc[0];
    int j = start[1] + inc[1];
    while (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
        DBPRINT("Checking arr[%d][%d]: %c\n", i, j, arr[i][j]);
        if (arr[i][j] == EMPTY) return false;
        if (arr[i][j] == c) return true;
        i += inc[0];
        j += inc[1];
    }
    return false;
}

void print_valid_moves(int **valid)
{
    printf("#");
    for (int i = 0; i < ROWS; i++) {
        if (DEBUG)
            printf(" %d", i);
        else
            printf(" %d", i + 1);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        if (DEBUG)
            printf("%d", i);
        else
            printf("%d", i + 1);

        for (int j = 0; j < COLS; j++) 
            printf(" %d", valid[i][j]);

        printf("\n");
    }
}

Move get_move(Board b, char player)
{
    int row, col;
    int **valid_moves = valid_moves_for(player, b);

    do {
        printf("Move (#row #col): ");
        scanf("%d %d", &row, &col);
        --row;
        --col;
    } while (
        row < 0 || row > ROWS - 1 ||
        col < 0 || col > COLS - 1 ||
        !valid_moves[row][col]
    );

    Move m = {.row = row, .col = col, .turn = player};
    return m;
}

Board board_update(Board old, Move m)
{
    // Copy contents and place move
    Board new;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            new.board[i][j] = old.board[i][j];

    new.board[m.row][m.col] = m.turn;

    // Take over opponents pieces
    for (int i = m.row - 1; i <= m.row + 1; i++) {
        for (int j = m.col - 1; j <= m.col + 1; j++) {

            if (i == m.row && j == m.col)
                continue;

            int inc[] = {i - m.row, j - m.col};
            int start[] = {m.row, m.col};
            bool has_other_end = search_line(new.board, m.turn, start, inc);

            // Fill up with current player's piece if other end is found
            if (has_other_end) {
                int x = start[0] + inc[0];
                int y = start[1] + inc[1];
                while (new.board[x][y] != m.turn) {
                    new.board[x][y] = m.turn;
                    x += inc[0];
                    y += inc[1];
                }
            }
        }
    }

    return new;
}
