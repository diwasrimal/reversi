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
    return b;
}

void board_print(Board b, char next_turn)
{
    printf("\n          %c: %d, %c: %d, Turn: %s%c%s\n", A, piece_count(b, A), 
            B, piece_count(b, B), (next_turn == A) ? RED_BOLD : GREEN_BOLD, next_turn,
            RESET);
    printf("┌───┬───┬───┬───┬───┬───┬───┬───┬───┐\n");
    printf("│ # │");
    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf(" %d │", i);
        else 
            printf(" %d │", i + 1);
    }
    printf("\n├───┼───┼───┼───┼───┼───┼───┼───┼───┤\n");

    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf("│ %d │", i);
        else 
            printf("│ %d │", i + 1);

        for (int j = 0; j < COLS; j++) {
            char c = b.board[i][j];
            printf("%s %c %s", (c == A) ? RED_BOLD : GREEN_BOLD, c, RESET);
            printf("│");
        }
        if (i == ROWS - 1)
            printf("\n└───┴───┴───┴───┴───┴───┴───┴───┴───┘\n");
        else
            printf("\n├───┼───┼───┼───┼───┼───┼───┼───┼───┤\n");
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

            // Cell must be empty to be valid
            if (b.board[i][j] != EMPTY) {
                DBPRINT("Cell is taken\n");
                continue;
            }


            // Loop through all neighboring cells
            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    DBPRINT("\tNeighbor: [%d, %d]\n", x, y);

                    // Skip invalid indices
                    if (OUT_OF_BOUNDS(x, y)) {
                        DBPRINT("\t\tinvalid indices\n");
                        continue;
                    }

                    // No need to check this piece
                    if (x == i && y == j) {
                        DBPRINT("\t\tno need to check this piece\n");
                        continue;
                    }

                    // Continue if neighboring piece is not opposing
                    if (b.board[x][y] == player || b.board[x][y] == EMPTY) {
                        DBPRINT("\t\tNot opposing piece\n");
                        continue;
                    }
                    
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


int valid_move(Board b, Move m)
{

    if (b.board[m.row][m.col] != EMPTY) {
        DBPRINT("Cell not empty!\n");
        return 0;
    }

    // Count from how many sides move is valid
    int valid = 0;

    // Loop through all neighboring pieces
    // Ensure that neighboring piece is opposing and
    // has another supporting piece on other side
    for (int i = m.row - 1; i <= m.row + 1; i++) {
        for (int j = m.col - 1; j <= m.col + 1; j++) {

            DBPRINT("\tNeighbor: [%d, %d]\n", i, j);

            // Skip invalid indices
            if (OUT_OF_BOUNDS(i, j)) {
                DBPRINT("\t\tinvalid indices\n");
                continue;
            }

            // No need to check this piece
            if (i == m.row && j == m.col) {
                DBPRINT("\t\tno need to check this poiece\n");
                continue;
            }

            // Continue if neighboring piece is not opposing
            if (b.board[i][j] == m.turn || b.board[i][j] == EMPTY) {
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

            int increment_matrix[] = {i - m.row, j - m.col};
            int start[] = {m.row, m.col};
            bool end_found = search_line(b.board, m.turn, start, increment_matrix);
            if (end_found)
                valid++;
        }
    }
    return valid;
}

bool search_line(char arr[ROWS][COLS], char c, int start[], int inc[])
{
    DBPRINT("\t\tSearching from [%d, %d] with matrix [%d, %d]\n", start[0], start[1], inc[0], inc[1]);

    int i = start[0] + inc[0];
    int j = start[1] + inc[1];
    while (!OUT_OF_BOUNDS(i, j)) {
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
    printf("\n┌───┬───┬───┬───┬───┬───┬───┬───┬───┐\n");
    printf("│ # │");
    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf(" %d │", i);
        else 
            printf(" %d │", i + 1);
    }
    printf("\n├───┼───┼───┼───┼───┼───┼───┼───┼───┤\n");

    for (int i = 0; i < ROWS; i++) {
        if (DEBUG) 
            printf("│ %d │", i);
        else 
            printf("│ %d │", i + 1);

        for (int j = 0; j < COLS; j++) {
            int num = valid[i][j];
            printf("%s %d %s", (num > 0) ? GREEN_BOLD : "", num, RESET);
            printf("│");
        }
        if (i == ROWS - 1)
            printf("\n└───┴───┴───┴───┴───┴───┴───┴───┴───┘\n");
        else
            printf("\n├───┼───┼───┼───┼───┼───┼───┼───┼───┤\n");
    }
    printf(RESET);
}

Move get_move(Board b, char player)
{
    Move m;
    m.turn = player;

    do {
        printf("Move (#row #col): ");
        scanf("%d %d", &m.row, &m.col);
        if (!DEBUG) {
            --m.row;
            --m.col;
        }
    } while (OUT_OF_BOUNDS(m.row, m.col) || !valid_move(b, m));

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

int piece_count(Board b, char player)
{
    int count = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (b.board[i][j] == player)
                count++;
    return count;
}

void print_winner(Board b)
{
    int count_a = piece_count(b, A);
    int count_b = piece_count(b, B);

    if (count_a > count_b)
        printf("Winner: %c\n", A);
    else if (count_b > count_a)
        printf("Winner: %c\n", B);
    else
        puts("TIE");
}
