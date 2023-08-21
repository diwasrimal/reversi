#include "helpers.c"

int main(void)
{
    char players[] = {A, B};

    Board b = board_init();
    int round = 0;

    while (!board_complete(b)) {

        char turn = players[round % 2];
        int **valid_moves = valid_moves_for(turn, b);

        board_print(b);
        puts("");
        print_valid_moves(valid_moves);

        Move m = get_move(b, turn);
        b = board_update(b, m);
        round++;
    }
}
