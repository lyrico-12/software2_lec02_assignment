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
