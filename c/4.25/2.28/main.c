#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 100000
#define SYMBOL_HASH '#'
#define SYMBOL_STAR '*'
#define SQUARE_SIZE 10
#define MIN_SCREEN_SIZE SQUARE_SIZE + 2

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

struct symbol *symbol_create(int x, int y, char c, struct scene *scene)
{
    struct symbol *s;
    s = malloc(sizeof(struct symbol));
    s->x = x;
    s->y = y;
    s->c = c;
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
        scene
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

void run(struct scene *s)
{
    int step_x, step_y;
    int tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y;
    int new_x, new_y, key;
    struct symbol *symbol;

    tl_x = (s->max_x - s->square->size) / 2 - 1;
    tl_y = (s->max_y - s->square->size) / 2 - 1;

    tr_x = (s->max_x + s->square->size) / 2 + 1;
    tr_y = (s->max_y - s->square->size) / 2 - 1;

    br_x = (s->max_x + s->square->size) / 2 + 1;
    br_y = (s->max_y + s->square->size) / 2;

    bl_x = (s->max_x - s->square->size) / 2 - 1;
    bl_y = (s->max_y + s->square->size) / 2;

    symbol = s->symbol;
    step_x = 1;
    step_y = 0;

    scene_render(s);

    while (1) {
        usleep(DELAY);

        new_x = symbol->x + step_x;
        new_y = symbol->y + step_y;

        if (new_x == tl_x && new_y == tl_y) {
            step_x += 1;
            step_y += 1;
        }

        if (new_x == tr_x && new_y == tr_y) {
            step_x -= 1;
            step_y += 1;
        }

        if (new_x == br_x && new_y == br_y) {
            step_x -= 1;
            step_y -= 1;
        }

        if (new_x == bl_x && new_y == bl_y) {
            step_x += 1;
            step_y -= 1;
        }

        key = getch();

        if (key != ERR) {
            return;
        }

        symbol_move(s->symbol, new_x, new_y);
    }
}

int main(int argc, char **argv)
{
    struct scene *scene;
    int max_x, max_y;
    
    initscr();
    timeout(0);
    getmaxyx(stdscr, max_y, max_x);
    curs_set(0);
    
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
