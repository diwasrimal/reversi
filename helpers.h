#define ROWS 8
#define COLS ROWS
#define EMPTY '.'
#define RED_BOLD "\033[1;31m"
#define GREEN_BOLD "\033[1;36m"
#define RESET "\033[0m"
#define A '+'
#define B '*'

typedef struct {
    char board[ROWS][COLS];
    bool complete;
} Board;

typedef struct {
    int row;
    int col;
} Move;

Board board_init(void);
void board_print(Board b) ;
bool board_complete(Board b) ;
Move get_move(Board b);
Board board_update(Board old, Move m, char turn);

