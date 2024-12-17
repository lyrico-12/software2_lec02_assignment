#include <stdio.h>

typedef struct student {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} Student;

typedef struct tagged_student1 {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
} TStudent1;

typedef struct tagged_student2 {
    char tag;
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} TStudent2;

int main (int argc, char**argv){
    Student s_begin;
    Student s1;
    TStudent1 s2;
    TStudent2 s3;
    TStudent2 s_end;
  

    /*課題1*/
    printf("Student's members\n");
    printf("s1.id: %p\n", &s1.id);
    printf("s1.name: %p\n", &s1.name);
    printf("s1.age: %p\n", &s1.age);
    printf("s1.height: %p\n", &s1.height);
    printf("s1.weight: %p\n", &s1.weight);
    
    printf("%zu\n", (uintptr_t)&s1.name - (uintptr_t)&s1.id);
    printf("%zu\n", (uintptr_t)&s1.age - (uintptr_t)&s1.name);
    printf("%zu\n", (uintptr_t)&s1.height - (uintptr_t)&s1.age);
    printf("%zu\n", (uintptr_t)&s1.weight - (uintptr_t)&s1.height);
    printf("\n");


    printf("TStudent1's members\n");
    printf("s2.id: %p\n", &s2.id);
    printf("s2.name: %p\n", &s2.name);
    printf("s2.age: %p\n", &s2.age);
    printf("s2.height: %p\n", &s2.height);
    printf("s2.weight: %p\n", &s2.weight);
    printf("s2.tag: %p\n", &s2.tag);

    printf("%zu\n", (uintptr_t)&s2.name - (uintptr_t)&s2.id);
    printf("%zu\n", (uintptr_t)&s2.age - (uintptr_t)&s2.name);
    printf("%zu\n", (uintptr_t)&s2.height - (uintptr_t)&s2.age);
    printf("%zu\n", (uintptr_t)&s2.weight - (uintptr_t)&s2.height);
    printf("%zu\n", (uintptr_t)&s2.tag - (uintptr_t)&s2.weight);
    printf("\n");

    
    printf("TStudent2's members\n");
    printf("s3.tag: %p\n", &s3.tag);
    printf("s3.id: %p\n", &s3.id);
    printf("s3.name: %p\n", &s3.name);
    printf("s3.age: %p\n", &s3.age);
    printf("s3.height: %p\n", &s3.height);
    printf("s3.weight: %p\n", &s3.weight);
    

    printf("%zu\n", (uintptr_t)&s3.id - (uintptr_t)&s3.tag);
    printf("%zu\n", (uintptr_t)&s3.name - (uintptr_t)&s3.id);
    printf("%zu\n", (uintptr_t)&s3.age - (uintptr_t)&s3.name);
    printf("%zu\n", (uintptr_t)&s3.height - (uintptr_t)&s3.age);
    printf("%zu\n", (uintptr_t)&s3.weight - (uintptr_t)&s3.height);
    printf("\n");

    return 0;
}
