#include "inc/master.h"
/////////////////////////////////////////////////////////
/**
 * @antuor   OneSleepWorm(一只瞌睡虫)
 * @brief    贪吃蛇游戏
 */
/////////////////////////////////////////////////////////
#define CELL_SIZE       10
#define GRID_COLS       24
#define GRID_ROWS       14
#define MAX_SNAKE_LEN   200
#define TICK_MS         300

#define darkgreen 0x0004

typedef enum { DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_UP } direction_t;

typedef struct snake_node {
    struct snake_node* next;
    struct snake_node* prev;
    ksc_obj_t obj;
} snake_node_t;

static snake_node_t node_pool[MAX_SNAKE_LEN];
static snake_node_t* free_head;
static snake_node_t* snake_head;
static snake_node_t* snake_tail;
static uint16_t snake_len;

static direction_t snake_dir;
static direction_t snake_next_dir;
static uint8_t food_x;
static uint8_t food_y;
static uint8_t game_over;
static uint16_t score;
static uint8_t init_render;

static volatile uint16_t timer_tick_count;

static uint32_t rng_state;
static void rng_seed(uint32_t s) { rng_state = s; }
static uint32_t rng_rand(void) {
    rng_state = rng_state * 1103515245 + 12345;
    return (rng_state >> 16) & 0x7FFF;
}

static char* u16toa(uint16_t n, char* buf) {
    char tmp[6], *p = buf;
    uint8_t i = 0;
    do { tmp[i++] = '0' + n % 10; n /= 10; } while (n);
    while (i) *p++ = tmp[--i];
    *p = '\0';
    return buf;
}

static uint8_t tail_dirty;
static uint8_t tail_px;
static uint8_t tail_py;
static uint8_t food_dirty;
static uint8_t old_food_px;
static uint8_t old_food_py;

static ksc_obj_t food_obj;
static ksc_obj_t gameover_obj;
static char score_buf[20];

static void update_score_str(void) {
    char* d = score_buf;
    const char* s = "Score: ";
    while (*s) *d++ = *s++;
    u16toa(score, d);
}

static void pool_init(void) {
    free_head = &node_pool[0];
    for (int i = 0; i < MAX_SNAKE_LEN - 1; i++)
        node_pool[i].next = &node_pool[i + 1];
    node_pool[MAX_SNAKE_LEN - 1].next = NULL;
    snake_head = NULL;
    snake_tail = NULL;
    snake_len = 0;
}

static snake_node_t* alloc_node(void) {
    if (!free_head) return NULL;
    snake_node_t* n = free_head;
    free_head = free_head->next;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

static void free_node(snake_node_t* n) {
    n->next = free_head;
    free_head = n;
}

static uint8_t is_occupied(uint8_t gx, uint8_t gy) {
    for (snake_node_t* n = snake_head; n; n = n->next)
        if (n->obj.sdx / CELL_SIZE == gx && n->obj.sdy / CELL_SIZE == gy)
            return 1;
    return 0;
}

static void spawn_food(void) {
    do {
        food_x = rng_rand() % GRID_COLS;
        food_y = rng_rand() % GRID_ROWS;
    } while (is_occupied(food_x, food_y));
}

static void snake_init(void) {
    pool_init();
    snake_dir = DIR_RIGHT;
    snake_next_dir = DIR_RIGHT;
    game_over = 0;
    score = 0;
    tail_dirty = 0;
    food_dirty = 0;
    init_render = 1;
    timer_tick_count = 0;

    for (int i = 2; i >= 0; i--) {
        snake_node_t* n = alloc_node();
        n->obj._type = _fillbox;
        n->obj.width = CELL_SIZE;
        n->obj.height = CELL_SIZE;
        n->obj.colorck = ggreen;
        n->obj.sdx = (GRID_COLS / 2 - i) * CELL_SIZE;
        n->obj.sdy = (GRID_ROWS / 2) * CELL_SIZE;
        n->next = snake_head;
        if (snake_head) snake_head->prev = n;
        snake_head = n;
        if (!snake_tail) snake_tail = n;
        snake_len++;
    }
    snake_head->obj.colorck = darkgreen;

    rng_seed(1);
    spawn_food();

    food_obj._type = _fillbox;
    food_obj.width = CELL_SIZE;
    food_obj.height = CELL_SIZE;
    food_obj.colorck = rred;

    gameover_obj._type = _string;
    gameover_obj.colorck = rred;
    gameover_obj.sdx = 76;
    gameover_obj.sdy = 64;
    gameover_obj.data = "GAME OVER";

        update_score_str();
    }

static void snake_tick(void) {
    snake_dir = snake_next_dir;

    uint8_t head_gx = snake_head->obj.sdx / CELL_SIZE;
    uint8_t head_gy = snake_head->obj.sdy / CELL_SIZE;
    uint8_t new_gx = head_gx;
    uint8_t new_gy = head_gy;
    switch (snake_dir) {
        case DIR_RIGHT: new_gx++; break;
        case DIR_DOWN:  new_gy++; break;
        case DIR_LEFT:  new_gx--; break;
        case DIR_UP:    new_gy--; break;
    }

    if (new_gx >= GRID_COLS || new_gy >= GRID_ROWS) {
        game_over = 1;
        init_render = 1;
        return;
    }

    for (snake_node_t* n = snake_head; n; n = n->next)
        if (n->obj.sdx / CELL_SIZE == new_gx && n->obj.sdy / CELL_SIZE == new_gy) {
            game_over = 1;
            init_render = 1;
            return;
        }

    uint8_t ate = (new_gx == food_x && new_gy == food_y);

    food_dirty = ate;
    if (ate) {
        old_food_px = food_x * CELL_SIZE;
        old_food_py = food_y * CELL_SIZE;
    }

    if (!ate) {
        tail_dirty = 1;
        tail_px = snake_tail->obj.sdx;
        tail_py = snake_tail->obj.sdy;
    } else {
        tail_dirty = 0;
    }

    snake_node_t* new_head = alloc_node();
    if (!new_head) { game_over = 1; init_render = 1; return; }

    new_head->obj._type = _fillbox;
    new_head->obj.width = CELL_SIZE;
    new_head->obj.height = CELL_SIZE;
    new_head->obj.colorck = darkgreen;
    new_head->obj.sdx = new_gx * CELL_SIZE;
    new_head->obj.sdy = new_gy * CELL_SIZE;

    new_head->next = snake_head;
    if (snake_head) snake_head->prev = new_head;
    snake_head = new_head;
    snake_len++;

    if (snake_head->next)
        snake_head->next->obj.colorck = ggreen;

    if (!ate) {
        snake_node_t* old_tail = snake_tail;
        snake_tail = snake_tail->prev;
        if (snake_tail) snake_tail->next = NULL;
        snake_len--;
        free_node(old_tail);
    } else {
        score++;
        spawn_food();
    update_score_str();
    }
}

static void render_full(k_draw_device* devp, KSC_window* win, KSC_window* swin) {
    kfull(devp, win, wwhite, 0, 0, 240, 144);
    kfull(devp, swin, bblack, 0, 0, 240, 16);

    food_obj.sdx = food_x * CELL_SIZE;
    food_obj.sdy = food_y * CELL_SIZE;
    kobjdraw(devp, win, &food_obj);

    for (snake_node_t* n = snake_head; n; n = n->next)
        kobjdraw(devp, win, &n->obj);

    kstring(devp, swin, score_buf, 4, 2, wwhite, bblack);
    kbox(devp, win, bblack, 0, 0, 240, 140);

    if (game_over)
        kobjdraw(devp, win, &gameover_obj);
}

static void render_tick(k_draw_device* devp, KSC_window* win, KSC_window* swin) {
    if (food_dirty) {
        kfillbox(devp, win, wwhite, old_food_px, old_food_py, CELL_SIZE, CELL_SIZE);
        food_dirty = 0;
    }

    if (tail_dirty) {
        kfillbox(devp, win, wwhite, tail_px, tail_py, CELL_SIZE, CELL_SIZE);
        tail_dirty = 0;
    }

    if (snake_head && snake_head->next)
        kfillbox(devp, win, ggreen,
            snake_head->next->obj.sdx, snake_head->next->obj.sdy,
            CELL_SIZE, CELL_SIZE);

    kfillbox(devp, win, darkgreen,
        snake_head->obj.sdx, snake_head->obj.sdy,
        CELL_SIZE, CELL_SIZE);

    food_obj.sdx = food_x * CELL_SIZE;
    food_obj.sdy = food_y * CELL_SIZE;
    kfillbox(devp, win, rred, food_obj.sdx, food_obj.sdy, CELL_SIZE, CELL_SIZE);

    kfillbox(devp, swin, bblack, 4, 2, 120, 8);
    kstring(devp, swin, score_buf, 4, 2, wwhite, bblack);
    kbox(devp, win, bblack, 0, 0, 240, 140);
}

static ki8 timer_callback(void* user_data) {
    ((input_device_t*)user_data)->input_create();
    timer_tick_count++;
    return 0;
}

static void handle_key(uint32_t keydata) {
    if (game_over) {
        snake_init();
        return;
    }
    switch (keydata) {
        case KEY_A5:  if (snake_dir != DIR_DOWN)  snake_next_dir = DIR_UP;    break;
        case KEY_A9:  if (snake_dir != DIR_UP)    snake_next_dir = DIR_DOWN;  break;
        case KEY_A8:  if (snake_dir != DIR_RIGHT) snake_next_dir = DIR_LEFT;  break;
        case KEY_A10: if (snake_dir != DIR_LEFT)  snake_next_dir = DIR_RIGHT; break;
    }
}

int main(void) {
    KSCOSSystem_Init();
    KSCOSSystemClock_Init(KSCOS_NORMAL_CLOCK);

    k_draw_device* devp = kscreenmount();

    KSC_window score_win = {0};
    score_win.ssx = 0;
    score_win.ssy = 0;
    score_win.width = 240;
    score_win.height = 16;
    score_win.bk = bblack;

    KSC_window game_win = {0};
    game_win.ssx = 0;
    game_win.ssy = 16;
    game_win.width = 240;
    game_win.height = 144;
    game_win.bk = wwhite;

    snake_init();
    render_full(devp, &game_win, &score_win);
    init_render = 0;

    input_device_t keydevice = key_default_device;
    keydevice.input_init();

    clock_task_t clocktask = clock_default_task;
    clocktask.task_cycle = 30;
    clocktask.callback = timer_callback;
    clocktask.user_data = (void*)&keydevice;
    clocktask.init(&clocktask);
    clocktask.run(&clocktask);

    while (1) {
        input_t keyinput = input_get(KEY_DEVICE_ID);
        if (keyinput.input_id == KEY_DEVICE_ID && keyinput.data)
            handle_key(keyinput.data);

        if (timer_tick_count >= TICK_MS / 30) {
            timer_tick_count = 0;
            if (!game_over) {
                snake_tick();
                render_tick(devp, &game_win, &score_win);
            }
        }

        if (init_render) {
            render_full(devp, &game_win, &score_win);
            init_render = 0;
        }

        sysdelay(10);
    }
}
