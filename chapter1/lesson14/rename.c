#include <stdio.h>
#include <errno.h>
int main(){
    int ret = rename("aaa", "bbb");

    if(ret == -1){
        perror("rename");
        return -1;
    }
    
    return 0;
}