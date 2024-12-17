# software2_lec02_assignment

## 課題４
内/外シングル、ダブル、トリプル、ブルの各得点領域の重心座標などを保存する枠組みが欲しかったので、新たにSectionという構造体を作った。
```c
// 各得点領域の情報を持つ(Sectionの種類は内シングル、外シングル、ダブル、トリプル)
typedef struct section{
    double smallR; // 内半径
    double largeR; // 外半径
    double center[20][2]; // 重心の座標。_.center[i - 1]には得点がiの場所の重心座標がある。
} Section;
```

Sectionのメンバcenterの初期化方法については`init_section_center`関数を参照。
この関数内で使用している`center_r`関数は、中心角10/π、R>rとして、半径Rの扇形から半径rの扇形を取り除いた図形の重心の、中心からの距離を求める関数である。

## 課題５
### 追加機能1 <br>ユーザの入力（B, T13, S7みたいなやつ）にかかった時間で標準偏差を変化
`clock_gettime`関数を使って時間処理をした。`CLOCK_MONOTONIC`はシステムの単調な時刻を格納しており、
```c
struct timespec start, end;
for (int i = 1 ; i <= 3 ; i++){
    printf("狙う場所を入力してください。");
    clock_gettime(CLOCK_MONOTONIC, &start);

    fgets(input, sizeof(input), stdin);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // 略

    double elapsed_time = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
    stddev = put_stddev(elapsed_time);
    
    // 略
}
```
上のように使うことで、`start`, `end`に`struct timespec`型の時刻を格納できる。`elasped_time`は秒単位とミリ秒単位で項を分けて計算している。
<br>
`put_stddev`関数で`elasped_time`を標準偏差に変換している。