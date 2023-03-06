#include <stdio.h>

int main(){
    int a = 10;
#ifdef DEBUG
    printf("woshiyigechengxuyuan\n");
#endif
    for(int i = 0; i<3; i++){
        printf("hello, GCC!!!\n");
    }
    int b, c, d, f;
    b = 10;
    c = b;
    d = c;
    f = d;
    return 0;
}