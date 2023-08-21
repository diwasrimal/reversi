#include "helpers.c"

int main(void)
{
    char players[] = {A, B};

    Board b = board_init();
    int round = 0;

    while (!board_complete(b)) {
        board_print(b);
        Move m = get_move(b);
        char turn = players[round % 2];
        b = board_update(b, m, turn);
        round++;
    }
}
