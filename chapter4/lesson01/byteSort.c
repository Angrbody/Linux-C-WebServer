/*
    字节序：字节在内存中存放的顺序


*/

// 通过代码检查主机当前的字节序

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    // C中的 union(联合体)所有成员具有共享内存
    // 联合体的 size 为所有成员数据中的 size最大值
    // 修改其中一个成员会对其他成员产生影响
    union{
        short value;    //2bytes
        char bytes[sizeof(short)]; //2bytes
    } test;

    test.value = 0x0102;
    
    if(test.bytes[0] == 1 && test.bytes[1] == 2){
        printf("--- 大端序 --- \n");
    }else if(test.bytes[0] == 2 && test.bytes[1] == 1){
        printf("--- 小端序 --- \n");    
    }else{
        printf("unknown\n");
    }

    return 0;
}