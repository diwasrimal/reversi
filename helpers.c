#include <stdio.h>
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
    for (int i = 0; i < ROWS; i++)
        printf(" %d", i + 1);
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
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

Move get_move(Board b)
{
    int row, col;
    do {
        printf("Move (#row #col): ");
        scanf("%d %d", &row, &col);
        --row;
        --col;
    } while (
        b.board[row][col] != EMPTY
        || row < 0 || row > 7
        || col < 0 || col > 7
    );

    Move m = { .row = row, .col = col };
    return m;
}

Board board_update(Board old, Move m, char turn)
{
    Board new;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            new.board[i][j] = old.board[i][j];
    new.board[m.row][m.col] = turn;
    return new;
}
