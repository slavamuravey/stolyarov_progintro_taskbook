#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define SYMBOL_STAR '*'
#define AREA_INITIAL_SIZE 5
#define MIN_SCREEN_SIZE AREA_INITIAL_SIZE
#define KEY_ESC 27
#define KEY_SPACE 32
#define COLOR_PAIR_ID 1
#define COLORS_SIZE 8

const int colors[COLORS_SIZE] = {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
};

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
    char textcolor;
    char bgcolor;
};

struct area *area_create(char c, int width, int height, struct scene *scene)
{
    struct area *a;

    a = malloc(sizeof(struct area));
    a->width = width;
    a->height = height;
    a->c = c;
    a->scene = scene;
    a->textcolor = colors[0];
    a->bgcolor = colors[0];

    return a;
}

void area_set_pair(struct area *a, char textcolor, char bgcolor) 
{
    /* It is strange, but if text color and backgound color are black, we can not switch colors on the first try. */
    if (a->textcolor == COLOR_BLACK && a->bgcolor == COLOR_BLACK) {
        init_pair(COLOR_PAIR_ID, -(textcolor - COLORS_SIZE), -(bgcolor - COLORS_SIZE));
    }
    a->textcolor = textcolor;
    a->bgcolor = bgcolor;
    init_pair(COLOR_PAIR_ID, textcolor, bgcolor);
    refresh();
}

void area_fill(struct area *a, char c)
{
    int i, j, start_x, start_y;
    start_x = (a->scene->max_x - a->width) / 2;
    start_y = (a->scene->max_y - a->height) / 2;
    
    area_set_pair(a, a->textcolor, a->bgcolor);
    attrset(COLOR_PAIR(COLOR_PAIR_ID));
    for (i = 0; i < a->height; i++) {
        for (j = 0; j < a->width; j++) {
            move(start_y + i, start_x + j);
            addch(c);
        }
    }

    refresh();
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
    int key, textcolor_id = 0, bgcolor_id = 0;
    scene_render(s);

    while (1) {
        key = getch();
        switch (key) {
        case KEY_LEFT:
            bgcolor_id = bgcolor_id == 0 ? COLORS_SIZE - 1 : bgcolor_id - 1;
            area_set_pair(s->area, colors[textcolor_id], colors[bgcolor_id]);

            break;
        case KEY_RIGHT:
            bgcolor_id = bgcolor_id == COLORS_SIZE - 1 ? 0 : bgcolor_id + 1;
            area_set_pair(s->area, colors[textcolor_id], colors[bgcolor_id]);
            
            break;
        case KEY_DOWN:
            textcolor_id = textcolor_id == 0 ? COLORS_SIZE - 1 : textcolor_id - 1;
            area_set_pair(s->area, colors[textcolor_id], colors[bgcolor_id]);
            
            break;
        case KEY_UP:
            textcolor_id = textcolor_id == COLORS_SIZE - 1 ? 0 : textcolor_id + 1;
            area_set_pair(s->area, colors[textcolor_id], colors[bgcolor_id]);
            
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

    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Colors are not supported.\n");
        exit(1);
    }

    cbreak();
    timeout(0);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    start_color();
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
