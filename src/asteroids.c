#include <curses.h>
#include <sys/param.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define PREFERED_WINDOW_INNER_WIDTH 25
#define PREFERED_WINDOW_INNER_HEIGHT 9

#define GAME_TICK_MILLISECONDS 25
#define MAX_STONES 64
#define VICTORY_TIME 500
#define STONES_MOVE_DELAY 3
#define STONES_SPAWN_DELAY 5

#define WINDOW_BORDER_CHAR '#'
#define PLAYER_CHAR '>'
#define STONE_CHAR '0'
#define EMPTY_CHAR ' '

const char GAME_HEADER [] = "Asteroids minigame";
const char GAME_DESCRIPTION [] = "Use arrows or WASD to move, Q to exit";
const char VICTORY_MESSAGE [] = "You won! Press space to exit";
const char FAIL_MESSAGE [] = "FAIL! Press space to exit";

struct stone {
    int speed;
    int x;
    int y;
};

int window_inner_width;
int window_inner_height;

int offset_y;
int offset_x;

int player_x = 0;
int player_y = 0;

struct stone stone_arr [MAX_STONES];
int survival_time = 0;

void calc_sizes() {
    window_inner_width = MIN(COLS-2, PREFERED_WINDOW_INNER_WIDTH);
    window_inner_height = MIN(LINES-2, PREFERED_WINDOW_INNER_HEIGHT);
    offset_x = (COLS - window_inner_width) / 2;
    offset_y = (LINES - window_inner_height) / 2;
}

void draw_headers() {
    if (offset_y >= 3) {
        mvaddstr(offset_y-3, (COLS-sizeof(GAME_HEADER))/2, GAME_HEADER);
        mvaddstr(offset_y-2, (COLS-sizeof(GAME_DESCRIPTION))/2, GAME_DESCRIPTION);
    }
}

void draw_window_border() {
    move(offset_y-1, offset_x-1);
    for (int i = 0; i < window_inner_width+2; ++i) {
        addch(WINDOW_BORDER_CHAR);
    }
    move(offset_y+window_inner_height, offset_x-1);
    for (int i = 0; i < window_inner_width+2; ++i) {
        addch(WINDOW_BORDER_CHAR);
    }
    for (int i = 0; i < window_inner_height; ++i) {
        mvaddch(offset_y+i, offset_x-1, WINDOW_BORDER_CHAR);
    }
    for (int i = 0; i < window_inner_height; ++i) {
        mvaddch(offset_y+i, offset_x+window_inner_width, WINDOW_BORDER_CHAR);
    }
}

void set_char(int y, int x, char c) {
    mvaddch(offset_y+y, offset_x+x, c);
}

void redraw_char(int old_y, int old_x, int y, int x, char c) {
    mvaddch(offset_y+old_y, offset_x+old_x, EMPTY_CHAR);
    mvaddch(offset_y+y, offset_x+x, c);
}

void draw_victory() {

}

void move_player(int y, int x) {
    redraw_char(player_y, player_x, y, x, PLAYER_CHAR);
    player_y = y;
    player_x = x;
}

void init_curses() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);
}

void init_stones() {
    for (int i = 0; i < MAX_STONES; ++i) {
        stone_arr[i] = (struct stone) {0};
    }
}

void init_game() {
    calc_sizes();
    draw_window_border();
    draw_headers();
    move_player(window_inner_height/2, 1);
    srand((unsigned int)time(NULL));
}

void abort_game() {
    endwin();
    exit(1);
}

void fail_game() {
    mvaddstr(LINES/2, (COLS-sizeof(FAIL_MESSAGE))/2, FAIL_MESSAGE);
    nodelay(stdscr, false);
    while (1)
    {
        if (getch() == ' ') {
            break;
        }
    }
    endwin();
    exit(2);
}

void win_game() {
    clear();
    mvaddstr(LINES/2, (COLS-sizeof(VICTORY_MESSAGE))/2, VICTORY_MESSAGE);
    nodelay(stdscr, false);
    while (1)
    {
        if (getch() == ' ') {
            break;
        }
    }
    endwin();
    exit(0);
}

void spawn_stone() {
    for (int i = 0; i < MAX_STONES; ++i) {
        if (stone_arr[i].speed == 0) { // means no stone
            stone_arr[i].speed = 1;
            stone_arr[i].x = window_inner_width-1;
            stone_arr[i].y = rand() % window_inner_height;
            set_char(stone_arr[i].y, stone_arr[i].x, STONE_CHAR);
            break;
        }
    }
}

void move_stones() {
    int new_x;
    for (int i = 0; i < MAX_STONES; ++i) {
        if (stone_arr[i].speed) {
            new_x = stone_arr[i].x - stone_arr[i].speed; 
            if (player_y == stone_arr[i].y && player_x >= new_x && player_x <= stone_arr[i].x) {
                fail_game();
            }
            if (new_x < 0) {
                set_char(stone_arr[i].y, stone_arr[i].x, EMPTY_CHAR);
                stone_arr[i] = (struct stone){0};
            } else {
                redraw_char(stone_arr[i].y, stone_arr[i].x, stone_arr[i].y, new_x, STONE_CHAR);
                stone_arr[i].x = new_x;
            }
        }
    }
}

void do_tick() {
    ++survival_time;
    if (survival_time % STONES_MOVE_DELAY == 0) {
        move_stones();
    }
    if (survival_time % STONES_SPAWN_DELAY == 0) {
        spawn_stone();
    }
    move_player(player_y, player_x); // just because
    if (survival_time >= VICTORY_TIME) {
        win_game();
    }
}

int main() {
    init_curses();
    init_game();

    while (1)
    {
        int c = getch();
        switch (c)
        {
        case KEY_LEFT: case 'a':
            if (player_x > 0)
                move_player(player_y, player_x-1);
            break;
        case KEY_RIGHT: case 'd':
            if (player_x < window_inner_width-1)
                move_player(player_y, player_x+1);
            break;
        case KEY_UP: case 'w':
            if (player_y > 0)
                move_player(player_y-1, player_x);
            break;
        case KEY_DOWN: case 's':
            if (player_y < window_inner_height-1)
                move_player(player_y+1, player_x);
            break;
        case 'q':
            abort_game();
        default:
            break;
        }
        usleep(GAME_TICK_MILLISECONDS*1000);
        do_tick();
    }
    return 0;
}
