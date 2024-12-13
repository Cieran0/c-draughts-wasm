#include <stdint.h>

typedef uint32_t size_t;

extern unsigned char __heap_base;

void* get_heap_base(void) {
    return &__heap_base;
}

void js_log(char* string) __attribute__((import_module("env"), import_name("js_log")));

void js_draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t colour)
    __attribute__((import_module("env"), import_name("js_draw_rectangle")));

void js_draw_circle(int32_t x, int32_t y, double radius, int32_t colour)
    __attribute__((import_module("env"), import_name("js_draw_circle")));

uint32_t width = 0;
uint32_t height = 0;

const int32_t BLACK = 0x00;
const int32_t BLACK_PLAYER = 0x00222222;
const int32_t WHITE = 0x00FFFFFF;
const int32_t WHITE_PLAYER = 0x00DDDDDD;
const int32_t BACKGROUND_COLOUR = 0x003b1c07;
const int32_t HIGHLIGHT = 0x003D5cFF;

void draw_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t colour) {
    js_draw_rectangle(x, y, width, height, colour);
}

void draw_circle(int32_t centre_x, int32_t centre_y, double radius, int32_t colour) {
    js_draw_circle(centre_x, centre_y, radius, colour);
}

int32_t board[8][8] = { 0 };

void clear_screen() {
    draw_rectangle(0,0, width, height, BACKGROUND_COLOUR);

    int32_t square_size = width < height ? width / 10 : height / 10;

    int32_t total_size = square_size*8;

    int32_t y_offset = (height - total_size)/2; 
    int32_t x_offset = (width - total_size)/2; 

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int32_t x = col * square_size;
            int32_t y = row * square_size;
            int32_t colour = (row + col) % 2 == 0 ? WHITE : BLACK;



            draw_rectangle(x + x_offset, y + y_offset, square_size, square_size, colour);

            if(board[col][row] != 0) {
                draw_circle(x+x_offset + square_size/2 ,y+y_offset + square_size/2, ((double)square_size)/2.1, HIGHLIGHT);
                draw_circle(x+x_offset + square_size/2 ,y+y_offset + square_size/2, ((double)square_size)/2.2, board[col][row]);
            }
        }
    }
    
}

void on_resize(int32_t new_width, int32_t new_height) {

    width = new_width;
    height = new_height;

    clear_screen();
}

void on_click(int32_t x, int32_t y) {
    // Placeholder for handling clicks
}

void reset() {
    for (size_t x = 0; x < 8; x++)
    {
        for (size_t y = 0; y < 8; y++)
        {
            if ((x+y)%2 == 0) {
                continue;
            }

            if(y<3) {
                board[x][y] = BLACK_PLAYER;
            } else if (y>=5) {
                board[x][y] = WHITE_PLAYER;
            }
        }
    }        
}