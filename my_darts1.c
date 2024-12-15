#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#define RADIUS 20

// 座標を表す. [-20,20] が描画範囲
typedef struct point{
    double x;
    double y;
} Point;


// 横方向については改行と'\0' も考慮する
typedef struct board{
    char space[2*RADIUS + 1][4 * RADIUS + 3];
    double radius;
} Board;


size_t my_get_board_height(Board *b) {
    return sizeof(b->space) /sizeof(b->space[0]);
}

size_t my_get_board_width(Board *b) {
    return sizeof(b->space[0]) / sizeof(char);
}


// 初期化して円を表示する
void my_init_board(Board *b) {
    int height = my_get_board_height(b);
    int width = my_get_board_width(b);
    b->radius = RADIUS;

    // 円の中心はh = radius, w = radius
    for (int h = 0; h < height ; h++) {
        for (int w = 0; w < width - 2; w++) { /*後ろ二つは改行と\0*/
            b->space[h][w] = ' ';

            // 以下で円を描く(+で)

            //distは円の中心との距離
            double dist = (double)(h - b->radius) * (h - b->radius) + (double)(w - 2 * b->radius) * (w - 2 * b->radius) / 4;
            if ( dist < b->radius * b->radius + 8 && dist > b->radius * b->radius - 8) {
                b->space[h][w] = '+';
            }
        }
        b->space[h][width - 2] = '\n';
        b->space[h][width - 1] = '\0';

        
    }
}

// 盤面を表示
void my_print_board(Board *b) {
    int height = my_get_board_height(b);

    for (int h = 0; h < height; h++) {
        printf("%s", b->space[h]);
    }
}

// 座標が描画領域ならtrueを返す
bool my_is_in_board(Board *b, Point p) {
    if (p.x >= -b->radius && p.x <= b->radius && p.y >= -b->radius && p.y <= b->radius) {
        return true;
    } else {
        return false;
    }
}

// 座標が有効（得点圏内）ならtrueを返す
bool my_is_valid_point(Board *b, Point p) {
    double dist = p.x * p.x + p.y * p.y;
    if (dist < b->radius * b->radius) {
        return true;
    } else {
        return false;
    }
}

// i回目 (1-3) が盤面内なら数字でプロット
void my_plot_throw(Board *b, Point p, int i) {
    char n = (char) i + 48;
    if (my_is_in_board(b, p)) {
        b->space[(int)p.y + (int)b->radius][(int)p.x * 2 + (int)b->radius * 2] = n;
    }
}

// 分散が等方向一定の正規分布の乱数を生成する
Point my_iso_gauss_rand(Point mu, double stddev){
    double u1;
    double u2;
    
    u1 = (double) rand() / RAND_MAX;
    u2 = (double) rand() / RAND_MAX;

    mu.x = stddev * sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    mu.y = stddev * sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);

    return mu;
}

// 座標を (? ?) で表示（改行なし）
void my_print_point(Point p) {
    printf("(%f, %f)", p.x, p.y);
}





int main(int argc, char **argv){
    Board board;

    srand((unsigned int)time(NULL));

    my_init_board(&board);

    // 3回投げる
    for (int i = 1 ; i <= 3 ; i++){
        Point p = my_iso_gauss_rand((Point){.x = 0, .y = 0}, 15.0);

        my_plot_throw(&board,p,i);
        my_print_board(&board);
        printf("-------\n");
        my_print_point(p);
        if (!my_is_valid_point(&board, p)) printf(" miss!");
        printf("\n");
        sleep(1);
    }
    return 0;
}

