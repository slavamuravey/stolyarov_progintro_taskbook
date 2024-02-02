#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define SYMBOL_STAR '*'
#define AREA_INITIAL_SIZE 3
#define MIN_SCREEN_SIZE AREA_INITIAL_SIZE + 2
#define KEY_ESC 27
#define KEY_SPACE 32

struct scene {
    int max_x;
    int max_y;
    struct area *area;
};

struct area {
    char c;
    int width;
    int height;
    struct scene *scene;
};


struct area *area_create(char c, int width, int height, struct scene *scene)
{
    struct area *a;

    a = malloc(sizeof(struct area));
    a->width = width;
    a->height = height;
    a->c = c;
    a->scene = scene;

    return a;
}

void area_fill(struct area *a, char c)
{
    int i, j, start_x, start_y;
    start_x = (a->scene->max_x - a->width) / 2;
    start_y = (a->scene->max_y - a->height) / 2;
    
    for (i = 0; i < a->height; i++) {
        for (j = 0; j < a->width; j++) {
            move(start_y + i, start_x + j);
            addch(c);
        }
    }

    refresh();
}

void area_render(struct area *a, int width, int height) 
{
    area_fill(a, ' ');
    a->width = width;
    a->height = height;
    area_fill(a, a->c);
}

void area_destroy(struct area *a)
{
    free(a);
}

struct scene *scene_create(int max_x, int max_y)
{
    struct scene *scene;

    scene = malloc(sizeof(struct scene));
    scene->max_x = max_x;
    scene->max_y = max_y;
    scene->area = area_create(SYMBOL_STAR, AREA_INITIAL_SIZE, AREA_INITIAL_SIZE, scene);

    return scene;
}

void scene_render(struct scene *s)
{
    clear();
    area_fill(s->area, s->area->c);
    refresh();
}

void scene_destroy(struct scene *s)
{
    area_destroy(s->area);
    free(s);
}

void run(struct scene *s)
{
    int key;
    scene_render(s);

    while (key = getch()) {
        switch (key) {
        case KEY_LEFT:
            if (s->area->width > 1) {
                area_render(s->area, s->area->width - 1, s->area->height);
            }

            break;
        case KEY_RIGHT:
            if (s->area->width < s->max_x - 2) {
                area_render(s->area, s->area->width + 1, s->area->height);
            }
            
            break;
        case KEY_DOWN:
            if (s->area->height > 1) {
                area_render(s->area, s->area->width, s->area->height - 1);
            }
            
            break;
        case KEY_UP:
            if (s->area->height < s->max_y - 2) {
                area_render(s->area, s->area->width, s->area->height + 1);
            }
            
            break;
        case KEY_SPACE:
        case KEY_ESC:
            return;
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
