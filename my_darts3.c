#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define RADIUS 20
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"

// 座標を表す. [-20,20] が描画範囲
typedef struct point{
    double x;
    double y;
    double score; // 角度に応じた1倍のスコア
} Point;


// 横方向については改行と'\0' も考慮する
typedef struct board{
    char space[2*RADIUS + 1][4 * RADIUS + 3];
    double radius;
} Board;

// 各得点領域の情報を持つ(Sectionの種類は内シングル、外シングル、ダブル、トリプル)
typedef struct section{
    double smallR; // 内半径
    double largeR; // 外半径
    double center[20][2]; // 重心の座標。_.center[i - 1]には得点がiの場所の重心座標がある。
} Section;

// x軸正方向から時計回りに並べたダーツの一倍のスコア
const int dart_scores[20] = {6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20, 1, 18, 4, 13};

// 分散が等方向一定の正規分布の乱数を生成する
Point my_iso_gauss_rand(double xmean, double ymean, double stddev);


// ドーナツ型の扇形の重心について、扇形の中心からの距離を返す。
// 中心角:π/10
double center_r(double large_r, double small_r);

// 構造体Sectionのメンバcenterに値を入れる関数
void init_section_center(Section *s);


// ユーザが狙った場所を中心とした乱数を生成。
// p.xとp.yに値を代入
// p.scoreにも値を代入
void init_point_aim(char input[10], Point *p, double stddev);

size_t my_get_board_height(Board *b);

size_t my_get_board_width(Board *b);

// 初期化して円を表示する
void my_init_board(Board *b);

// 盤面を表示
void my_print_board(Board *b);

// 座標が描画領域ならtrueを返す
bool my_is_in_board(Board *b, Point p);

// 座標が有効（得点圏内）ならtrueを返す
bool my_is_valid_point(Board *b, Point p);

// i回目 (1-3) が盤面内なら数字でプロット
void my_plot_throw(Board *b, Point p);

// ユーザの入力が有効な入力かチェックする
bool check_user_input(char input[10]);

// 座標を (? ?) で表示（改行なし）
void my_print_point(Point p);

// 中心からの距離に応じてスコア計算
int calc_score(Point p);

// 入力の経過時間によって標準偏差の値を変える
double put_stddev(double elasped_time);


int main(int argc, char **argv){
    Board board;

    double stddev;// 標準偏差をユーザ入力で受け付けるようにする。

    srand((unsigned int)time(NULL));

    my_init_board(&board);

    int score; // 最初の持ち点をユーザ入力で受け付ける
    int opt;
    opt = getopt(argc, argv, "a:");
    
    if(opt == 'a') {
        if (!isdigit(optarg[0])) {
            printf("-a [3, 5, 7, 9, 11, 15のいづれかの数字] と入力してください。\n");
            return 1;
        } else {
            score = atoi(optarg);
            if (!(score == 3 || score == 5 || score == 7 || score == 9 || score == 11 || score == 15)) {
                printf("-a [3, 5, 7, 9, 11, 15のいづれかの数字] と入力してください。\n");
                return 1;
            }
            score = 100 * score + 1;// 01ルール
            printf(RED"Rule : %d\n"RESET, score);
        }

    } else if (opt == '?' || opt == -1) {
        printf("-a [3, 5, 7, 9, 11, 15のいづれかの数字] と入力してください。\n");
        return 1;
    }

    char input[10];

    struct timespec start, end;
    
    int i = 1; // 何投目なのか
    // 301ルール
    while (score != 0){
        printf("狙う場所を入力してください。");

        // 入力にかかる時間を計測する
        clock_gettime(CLOCK_MONOTONIC, &start);

        fgets(input, sizeof(input), stdin);
        clock_gettime(CLOCK_MONOTONIC, &end);
        // 改行文字を取り除く
        input[strcspn(input, "\n")] = '\0';

        if (check_user_input(input) != true) {
            printf("入力形式が間違っています。\n");
            i--;
            continue;
        }

        // 入力時間(秒単位)
        double elapsed_time = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        stddev = put_stddev(elapsed_time);
        printf("elasped_time: %f, stddev: %f\n", elapsed_time, stddev);

        Point p;
        init_point_aim(input, &p, stddev);
        score -= calc_score(p);

        if (score < 0) { // もしスコアが負になったら一つ前のスコアに戻す。
            score += calc_score(p);
        }

        my_plot_throw(&board,p);
        my_print_board(&board);
        printf("-------\n");
        my_print_point(p);
        if (!my_is_valid_point(&board, p)) printf(" miss!");
        printf("\n");
        printf("Your score is %d\n", score);
        if (score == 0) {
            printf("End! Your total num of throw: %d.\n", i);
        }
        printf("\n");
        sleep(1);

        i++;
    }
    return 0;
}


// 分散が等方向一定の正規分布の乱数を生成する
Point my_iso_gauss_rand(double xmean, double ymean, double stddev){
    Point mu;
    double u1;
    double u2;
    
    u1 = (double) rand() / RAND_MAX;
    u2 = (double) rand() / RAND_MAX;

    mu.x = xmean + stddev * sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    mu.y = ymean + stddev * sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
    mu.score = 0;
    return mu;
}


// ドーナツ型の扇形の重心について、扇形の中心からの距離を返す。
// 中心角:π/10
double center_r(double large_r, double small_r) {
    return 40.0 * sin(M_PI / 20) * (pow(large_r, 3.0) - pow(small_r, 3.0)) / (3.0 * M_PI * (pow(large_r, 2.0) - pow(small_r, 2.0)));
}

// 構造体Sectionのメンバcenterを作る関数
void init_section_center(Section *s){
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 2; j++) {
            s->center[i][j] = 0.0;// 一応初期化しておく
        }
    }

    double dummy[20][2];// x軸正方向から得点領域の重心座標を並べたものを格納する。

    double cen_r = center_r(s->largeR, s->smallR);// 重心の、中心からの距離
    double x;// 重心のx座標
    double y;// 重心のy座標
    double ang = 0;
    for (int i = 0; i < 20; i++) {
        x = cen_r * cos(ang);
        y = cen_r * sin(ang);
        dummy[i][0] = x;
        dummy[i][1] = y;
        ang += M_PI / 10;
    }

    // s->center[i] が、１倍のスコアがi点の得点領域の重心にしたい。
    for (int i = 0; i < 20; i ++) {
        s->center[dart_scores[i] - 1][0] = dummy[i][0];
        s->center[dart_scores[i] - 1][1] = dummy[i][1];        
    }

}


// ユーザが狙った場所を中心とした乱数を生成。
// p.xとp.yに値を代入
// p.scoreにも値を代入
void init_point_aim(char input[10], Point *p, double stddev){
    Section single1;// 内シングル
    single1.smallR = 3;
    single1.largeR = 11;
    init_section_center(&single1);

    Section single2;// 外シングル
    single2.smallR = 13;
    single2.largeR = 18;
    init_section_center(&single2);

    Section dble;
    dble.smallR = 18;
    dble.largeR = 20;
    init_section_center(&dble);

    Section triple;
    triple.smallR = 11;
    triple.largeR = 13;
    init_section_center(&triple);

    // 例えばユーザの入力がT13(実際に狙う得点は39)だったら、aim_scoreは13
    int aim_score = atoi(input + 1);
    double xmean;
    double ymean;
    if (input[0] == 'B') {
        *p = my_iso_gauss_rand(0.0, 0.0, stddev);
    } else if (input[0] == 'T') {
        xmean = triple.center[aim_score - 1][0];
        ymean = triple.center[aim_score - 1][1];
        *p = my_iso_gauss_rand(xmean, ymean, stddev);
    } else if (input[0] == 'D') {
        xmean = dble.center[aim_score - 1][0];
        ymean = dble.center[aim_score - 1][1];
        *p = my_iso_gauss_rand(xmean, ymean, stddev);
    } else if (input[0] == 'S') {
        // 内シングルか外シングルかはランダムで決める
        double random = (double)rand() / RAND_MAX;// 0-1の乱数を生成
        if (random < 0.5) {// 0.5以下だったら内シングル
            xmean = single1.center[aim_score - 1][0];
            ymean = single1.center[aim_score - 1][1];
            *p = my_iso_gauss_rand(xmean, ymean, stddev);
        } else {// 0.5以上だったら外シングル
            xmean = single2.center[aim_score - 1][0];
            ymean = single2.center[aim_score - 1][1];
            *p = my_iso_gauss_rand(xmean, ymean, stddev);
        }
    }

    // 実際に投げた点の１倍のスコアを計算
    double ang = atan2(p->y, p->x); // 点pの角度(-pi ~ pi)
    if (ang < 0) {
        ang += 2 * M_PI;// x軸正方向が0となり、0-2πに
    }

    // i * π/10 - π/20 ~ i * π/10 + π/20
    // ダーツの盤面通りに角度から１倍のスコアを計算
    int sector = (int) ((ang + M_PI / 20) / (M_PI / 10)) % 20;
    p->score = dart_scores[sector];
}

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
            if ( dist < b->radius * b->radius + 8 && dist > b->radius * b->radius - 8) { // 盤面の一番外側
                b->space[h][w] = '+';
            } else if (dist < 11.0 * 11.0 + 6 && dist > 11.0 * 11.0 - 6) {// トリプルの内円
                b->space[h][w] = '+';
            } else if (dist < 13.0 * 13.0 + 6 && dist > 13.0 * 13.0 - 6) {// トリプルの外円
                b->space[h][w] = '+';
            } else if (dist < 18.0 * 18.0 + 8 && dist > 18.0 * 18.0 - 8) {// ダブルの内円
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
    int width = my_get_board_width(b);

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            double dist = (double)(h - b->radius) * (h - b->radius) + (double)(w - 2 * b->radius) * (w - 2 * b->radius) / 4;
            
            if ((dist < 11.0 * 11.0 + 6 && dist > 11.0 * 11.0 - 6) || (dist < 13.0 * 13.0 + 6 && dist > 13.0 * 13.0 - 6)) {// トリプルは赤
                if (!isdigit((int)b->space[h][w])) {
                    printf(RED"%c"RESET, b->space[h][w]);
                } else {
                    printf("%c", b->space[h][w]);
                }
                
            } else if ((dist < 18.0 * 18.0 + 8 && dist > 18.0 * 18.0 - 8) || (dist < b->radius * b->radius + 8 && dist > b->radius * b->radius - 8)) {// ダブルは緑
                if (!isdigit((int)b->space[h][w])) {
                    printf(GREEN"%c"RESET, b->space[h][w]);
                } else {
                    printf("%c", b->space[h][w]);
                }
            } else {// それ以外なら色つけない
                printf("%c", b->space[h][w]);
            }
        }
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
void my_plot_throw(Board *b, Point p) {
    char n = '*';
    if (my_is_in_board(b, p)) {
        b->space[(int)p.y + (int)b->radius][(int)p.x * 2 + (int)b->radius * 2] = n;
    }
}

// ユーザの入力が有効な入力かチェックする
bool check_user_input(char input[10]) {
    char c;
    int n;
    int pos;
    if (input[0] == 'B' && input[1] == '\0') {
        return true;
    }

    if (sscanf(input, "%c%d%n", &c, &n, &pos) != 2 || input[pos] != '\0') {
        return false;
    }

    // 文字と数値の範囲をチェック
    if (!(c == 'S' || c == 'D' || c == 'T')) {
        return false;
    }
    
    if (n < 1 || n > 20) {
        return false;
    }

    return true;
}

// 座標を (? ?) で表示（改行なし）
void my_print_point(Point p) {
    printf("(%f, %f)", p.x, p.y);
}

int calc_score(Point p) {
    double r = sqrt(p.x * p.x + p.y * p.y);

    if (r <= 1) {// インブル
        return 50;
    } else if (1 < r && r <= 3) {// アウターブル
        return 25;
    } else if ((3 < r && r <= 11) || (13 < r && r <= 18)) {// シングル
        return p.score;
    } else if (18 < r && r <= 20) {// ダブル
        return 2 * p.score;
    } else if (11 < r && r <= 13) {// トリプル
        return 3 * p.score;
    } else {
        return 0;
    }
}

// 入力の経過時間によって標準偏差の値を変える
double put_stddev(double elasped_time) {
    if (elasped_time <= 1.0) {
        return 1.0;
    } else if (elasped_time <= 2.0) {
        return 2.0;
    } else if (elasped_time <= 3.0) {
        return 3.0;
    } else if (elasped_time <= 4.0) {
        return 4.0;
    } else if (elasped_time <= 5.0) {
        return 5.0;
    } else {
        return 6.0;
    }
}