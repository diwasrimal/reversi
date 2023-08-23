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
    // Show count of pieces and turn
    printf("          %c: %d, %c: %d", A, piece_count(b, A), B, piece_count(b, B));
    printf(", Turn: ");
    if (next_turn == A) COLOR_RED;
    if (next_turn == B) COLOR_GREEN;
    printf("%c\n", next_turn);
    COLOR_RESET;

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
            if (c == A) COLOR_RED;
            if (c == B) COLOR_GREEN;
            printf(" %c ", c);
            COLOR_RESET;
            printf("│");
        }
        if (i == ROWS - 1)
            printf("\n└───┴───┴───┴───┴───┴───┴───┴───┴───┘\n");
        else
            printf("\n├───┼───┼───┼───┼───┼───┼───┼───┼───┤\n");
    }
    COLOR_RESET;
}

bool board_complete(Board b)
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (b.board[i][j] == EMPTY)
                return false;
    return true;
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
        check_move(b, &m);
    } while (!m.valid);

    return m;
}

Board board_update(Board old, Move m)
{
    // Copy contents and place move
    Board new;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            new.board[i][j] = old.board[i][j];

    // Place move
    new.board[m.row][m.col] = m.turn;

    // Take over opponents pieces
    for (int i = 0; i < m.consumed_pieces; i++)
        new.board[m.consumed_points[i].x][m.consumed_points[i].y] = m.turn;

    return new;
}

void check_move(Board b, Move *m)
{
    char supporting_piece = m->turn;
    m->consumed_pieces = 0;

    // Move will be invalid if out of bounds or not empty
    if (OUT_OF_BOUNDS(m->row, m->col) || b.board[m->row][m->col] != EMPTY) {
        m->valid = false;
        return;
    }

    // Loop through neighboring points
    for (int i = m->row - 1; i <= m->row + 1; i++) {
        for (int j = m->col - 1; j <= m->col + 1; j++) {

            if (OUT_OF_BOUNDS(i, j))
                continue;
            if (i == m->row && j == m->col)
                continue;

            // Adjacent cells should be opposing and not empty
            if (b.board[i][j] == supporting_piece || b.board[i][j] == EMPTY)
                continue;


            // Coordinates of given move and one neighboring
            // point forms form a straight line
            // Check if supporting piece exists along that line
            //    \  |  /
            //     . . .
            // --- . m . - x -->
            //     . . .   ^ supporting piece
            //    /  |  \

            // Movement vector to shift the point towards a direction
            int m_vec[] = {i - m->row, j - m->col};
            int x = m->row + m_vec[0];
            int y = m->col + m_vec[1]; 
            int in_between = 0;

            while (!OUT_OF_BOUNDS(x, y) && b.board[x][y] != EMPTY) {
                if (b.board[x][y] == supporting_piece) {
                    m->consumed_pieces += in_between;
                    break;
                }
                Point p = {.x = x, .y = y};
                m->consumed_points[m->consumed_pieces + (in_between++)] = p;
                x += m_vec[0];
                y += m_vec[1];
            }
         }
    }
    m->valid = (m->consumed_pieces > 0);
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
