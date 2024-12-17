# 課題1

## 実行結果

```
Student's members
s1.id: 0x16fdb3288
s1.name: 0x16fdb328c
s1.age: 0x16fdb32f0
s1.height: 0x16fdb32f8
s1.weight: 0x16fdb3300
4
100
8
8

TStudent1's members
s2.id: 0x16fdb3200
s2.name: 0x16fdb3204
s2.age: 0x16fdb3268
s2.height: 0x16fdb3270
s2.weight: 0x16fdb3278
s2.tag: 0x16fdb3280
4
100
8
8
8

TStudent2's members
s3.tag: 0x16fdb3180
s3.id: 0x16fdb3184
s3.name: 0x16fdb3188
s3.age: 0x16fdb31ec
s3.height: 0x16fdb31f0
s3.weight: 0x16fdb31f8
4
4
100
4
8
```

## 考察
### Student
ageの後に4バイトのパディングが生じている。これは、heightの先頭アドレスを8バイト境界に揃えるため。

### TStudent1
ageの後に4バイトのパディングが生じている。これは、heightの先頭アドレスを8バイト境界に揃えるため。

### TStudent2
tagの後に3バイトのパディングが生じている。これは、idの先頭アドレスを4バイト境界に揃えるため。ageの後にパディングが生じていないのは、heightの先頭アドレスが8バイト境界になっているから。
