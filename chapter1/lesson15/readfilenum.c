/*
    1. opendir:打开目录
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
    DIR *fdopendir(int fd);
        dir* :目录流结构体指针

    2. readdir:读取目录中的数据
    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
        dirp 为open得到的指针
        返回值 struct dirent 为文件读取到的信息，
        若读到目录末尾或失败，返回值为 NULL

    3. closedir：关闭目录
    #include <sys/types.h>
    #include <dirent.h>
    int closedir(DIR *dirp);
*/

//读取某个目录下所有普通文件的个数
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
int main(int argc, char* argv[]){
    
    if(argc < 2){
        printf("%s path\n", argv[0]);
        return -1;
    }


    return 0;
}

//获取目录下所有普通文件的个数
int getFileNum(const char* filePath){

    DIR* dir = opendir(filePath);
    if(dir == NULL){
        perror("opendir");
        return -1;
    }

    struct dirent* ptr = readdir(dir);
    while(ptr != NULL){
        
        if(ptr->d_name == "." || ptr->d_name == ".."){
            continue;
        }

        if(ptr->d_type == DT_DIR)
        
    }
    
}