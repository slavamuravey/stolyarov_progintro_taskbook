#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 100000
#define DELAY_STEP 10000
#define SYMBOL_HASH '#'
#define SYMBOL_STAR '*'
#define SQUARE_SIZE 10
#define MIN_SCREEN_SIZE SQUARE_SIZE + 2
#define KEY_ESC 27
#define KEY_SPACE 32

int delay = DELAY;

struct scene {
    int max_x;
    int max_y;
    struct symbol *symbol;
    struct square *square;
};

struct square {
    char c;
    int size;
    struct scene *scene;
};

struct symbol {
    int x;
    int y;
    char c;
    struct scene *scene;
    int step_x;
    int step_y;
};

struct square *square_create(int size, char c, struct scene *scene)
{
    struct square *s;

    s = malloc(sizeof(struct square));
    s->size = size;
    s->c = c;
    s->scene = scene;

    return s;
}

void square_render(struct square *s)
{
    int i, j, start_x, start_y;
    start_x = (s->scene->max_x - s->size) / 2;
    start_y = (s->scene->max_y - s->size) / 2;
    
    for (i = 0; i < s->size; i++) {
        for (j = 0; j < s->size; j++) {
            move(start_y + j, start_x + i);
            addch(s->c);
        }
    }
    
    refresh();
}

void square_destroy(struct square *s)
{
    free(s);
}

struct symbol *symbol_create(int x, int y, char c, struct scene *scene, int step_x, int step_y)
{
    struct symbol *s;
    s = malloc(sizeof(struct symbol));
    s->x = x;
    s->y = y;
    s->c = c;
    s->step_x = step_x;
    s->step_y = step_y;
    s->scene = scene;

    return s;
}

void symbol_move(struct symbol *s, int x, int y)
{
    move(s->y, s->x);
    addch(' ');
    s->x = x;
    s->y = y;
    move(s->y, s->x);
    addch(s->c);
    refresh();
}

void symbol_destroy(struct symbol *s)
{
    free(s);
}

struct scene *scene_create(int max_x, int max_y)
{
    struct scene *scene;

    scene = malloc(sizeof(struct scene));
    scene->max_x = max_x;
    scene->max_y = max_y;
    scene->square = square_create(SQUARE_SIZE, SYMBOL_STAR, scene);
    scene->symbol = symbol_create(
        (scene->max_x - scene->square->size) / 2 - 1, 
        (scene->max_y - scene->square->size) / 2 - 1, 
        SYMBOL_HASH, 
        scene,
        1,
        0
    );

    return scene;
}

void scene_render(struct scene *s)
{
    struct symbol *symbol;
    symbol = s->symbol;
    clear();
    square_render(s->square);
    symbol_move(symbol, symbol->x, symbol->y);
    refresh();
}

void scene_destroy(struct scene *s)
{
    symbol_destroy(s->symbol);
    square_destroy(s->square);
    free(s);
}

void check_corner(struct symbol *s, int x, int y)
{
    int tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y;
    struct scene *scene;
    scene = s->scene;
    
    tl_x = (scene->max_x - scene->square->size) / 2 - 1;
    tl_y = (scene->max_y - scene->square->size) / 2 - 1;

    tr_x = (scene->max_x + scene->square->size) / 2 + 1;
    tr_y = (scene->max_y - scene->square->size) / 2 - 1;

    br_x = (scene->max_x + scene->square->size) / 2 + 1;
    br_y = (scene->max_y + scene->square->size) / 2;

    bl_x = (scene->max_x - scene->square->size) / 2 - 1;
    bl_y = (scene->max_y + scene->square->size) / 2;

    if (x == tl_x && y == tl_y) {
        s->step_x += 1;
        s->step_y += 1;
    }

    if (x == tr_x && y == tr_y) {
        s->step_x -= 1;
        s->step_y += 1;
    }

    if (x == br_x && y == br_y) {
        s->step_x -= 1;
        s->step_y -= 1;
    }

    if (x == bl_x && y == bl_y) {
        s->step_x += 1;
        s->step_y -= 1;
    }
}

void run(struct scene *s)
{
    
    int new_x, new_y, key;
    struct symbol *symbol;

    symbol = s->symbol;
    scene_render(s);

    while ((key = getch()) != KEY_ESC) {
        switch (key) {
        case KEY_LEFT:
            delay += DELAY_STEP;
            
            break;
        case KEY_RIGHT:
            delay -= DELAY_STEP;
            if (delay < 0) {
                delay = 0;
            }
            
            break;
        case KEY_SPACE:
            symbol->step_x *= -1;
            symbol->step_y *= -1;

            check_corner(symbol, new_x, new_y);
            
            break;
        case ERR:
            usleep(delay > 0 ? delay : 1);
            new_x = symbol->x + symbol->step_x;
            new_y = symbol->y + symbol->step_y;

            check_corner(symbol, new_x, new_y);

            symbol_move(s->symbol, new_x, new_y);
            
            break;
        }
    }
}

int main(int argc, char **argv)
{
    struct scene *scene;
    int max_x, max_y;
    
    initscr();
    cbreak();
    timeout(0);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, max_y, max_x);
    
    if (max_x < MIN_SCREEN_SIZE || max_y < MIN_SCREEN_SIZE) {
        endwin();
        fprintf(stderr, "Too small screen.\n");
        exit(1);
    }

    scene = scene_create(max_x, max_y);
    run(scene);
    scene_destroy(scene);

    endwin();
    
    return 0;
}
