#define ROWS 8
#define COLS ROWS
#define EMPTY ' '
#define A '+'
#define B '*'
#define OUT_OF_BOUNDS(i, j) ((i) < 0 || (i) >= ROWS || (j) < 0 || (j) >= COLS)

#define DEBUG 0
#if DEBUG
#define DBPRINT(...) printf(__VA_ARGS__)
#else
#define DBPRINT(...)
#endif

/* Colors for Windows */
#ifdef _WIN32
#include <windows.h>
#define COLOR_RED		SetConsoleTextAttribute(hConsole, FOREGROUND_RED)
#define COLOR_GREEN		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN)
#define COLOR_RESET		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
HANDLE hConsole;

/* For *nix */
#else
#define COLOR_RED printf("\033[31m")
#define COLOR_GREEN printf("\033[36m")
#define COLOR_RESET printf("\033[0m")
#endif

typedef struct {
    char board[ROWS][COLS];
} Board;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int row;
    int col;
    char turn;
    bool valid;
    int consumed_pieces;
    Point consumed_points[ROWS * COLS];
} Move;

Board board_init(void);
void  board_print(Board b, char next_turn);
bool  board_complete(Board b) ;
Board board_update(Board old, Move m);
Move  get_move(Board b, char player);
int   piece_count(Board b, char player);
void  print_winner(Board b);
void  check_move(Board b, Move *m);
