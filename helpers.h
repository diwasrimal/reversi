#define ROWS 8
#define COLS ROWS
#define EMPTY ' '
#define RED_BOLD "\033[1;31m"
#define GREEN_BOLD "\033[1;36m"
#define RESET "\033[0m"
#define A '+'
#define B '*'

#define DEBUG 0
#if DEBUG
#define DBPRINT(...) printf(__VA_ARGS__)
#else
#define DBPRINT(...)
#endif

typedef struct {
    char board[ROWS][COLS];
    bool complete;
} Board;

typedef struct {
    int row;
    int col;
    char turn;
} Move;

Board board_init(void);
void board_print(Board b) ;
bool board_complete(Board b) ;
Move get_move(Board b, char player);
Board board_update(Board old, Move m);
int **valid_moves_for(char player, Board b);
void print_valid_moves(int **valid);
bool search_line(char arr[ROWS][COLS], char c, int start[], int inc[]);
