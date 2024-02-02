#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 100000
#define SYMBOL '*'
#define ITERATIONS 3

struct symbol {
    int x;
    int y;
    char c;
    struct scene *scene;
};

struct symbol *symbol_create(int x, int y, char c, struct scene *scene)
{
    struct symbol *s = malloc(sizeof(struct symbol));
    s->x = x;
    s->y = y;
    s->c = c;
    s->scene = scene;

    return s;
}

void symbol_move(struct symbol *s, int x, int y)
{
    s->x = x;
    s->y = y;
}

void symbol_destroy(struct symbol *s)
{
    free(s);
}

struct scene {
    int max_x;
    int max_y;
    struct symbol *symbol;
};

struct scene *scene_create(int max_x, int max_y)
{
    struct scene *scene;

    scene = malloc(sizeof(struct scene));
    scene->max_x = max_x;
    scene->max_y = max_y;
    scene->symbol = symbol_create(0, scene->max_y / 2, SYMBOL, scene);

    return scene;
}

void render(struct scene *s)
{
    struct symbol *symbol;
    symbol = s->symbol;
    clear();
    move(symbol->y, symbol->x);
    addch(symbol->c);
    refresh();
}

void scene_destroy(struct scene *s)
{
    symbol_destroy(s->symbol);
    free(s);
}

void run(struct scene *s)
{
    int i;
    int step;
    int new_x, new_y;
    struct symbol *symbol;

    symbol = s->symbol;
    step = 1;

    render(s);

    for (i = 0; i < ITERATIONS; i++) {
        do {
            usleep(DELAY);
            
            new_x = symbol->x + step;
            new_y = symbol->y;

            if (new_x == s->max_x - 1 || new_x == 0) {
                step *= -1;
            }
            
            symbol_move(s->symbol, new_x, new_y);
            render(s);
        } while (symbol->x != 0);
    }
}

int main(int argc, char **argv)
{
    struct scene *scene;
    int max_x, max_y;
    
    initscr();
    getmaxyx(stdscr, max_y, max_x);
    curs_set(0);
    
    scene = scene_create(max_x, max_y);
    run(scene);
    scene_destroy(scene);

    endwin();
    
    return 0;
}
