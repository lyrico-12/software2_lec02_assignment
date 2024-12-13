#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"

int main(){
    Obj h1 = { .a = 1.0, .b = 'c', .c = 2.0 };
    Obj h2 = h1;
    
    // before trick
    printf("(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");

    h2 = trick(h2); // h2 をいじる

    // after trick
    printf("(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");
    

    return 0;
}