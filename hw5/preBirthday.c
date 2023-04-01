#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dirent.h> // to use DT_BLK and others
#include <sys/types.h> // uid_t
#include <stddef.h>
#include <sys/stat.h> // to use struct stat
#include <unistd.h> // geteuid()
#include <linux/limits.h> // In this header, it define the longest route length

int filetype[100];
char fileSymbol[8];
int count[100] = {0};

void initFileType();
int readSize(char *pathname);
long Dir_total_size(char *pathname);
void print_type();


int main(int argc, char **argv){
    if(argc == 1){
        printf("No directory name.\n");
        exit(EXIT_FAILURE);
    }
    printf("授課老師（羅習五）的生日是：1990/04/10\n");
    uid_t euid = geteuid();
    setuid(euid);
    initFileType();

    char *dir_name = argv[1];
    long file_size = Dir_total_size(dir_name);

    printf("總共大小：%ld bytes\n", file_size);
    print_type();
}

void initFileType(){
    for(int i = 0; i < 100; i++){
        filetype[i] = 0;
    }
    // There are some symbol defined in dirent.h
    // DT_UNKNOWN : type is unknown
    // DT_REG : regular file
    // DT_DIR : director
    // DT_FIFO : pipe or FIFO
    // DT_SOCK : local-domain socket
    // DT_CHR : character device
    // DT_BLK : block device
    // DT_LNK : symbolic link
    // DT_WHT : dummy whiteout inode, but this should not be seen in normal user applications, so I don't put it in this program
    fileSymbol[DT_UNKNOWN] = 'U';
    fileSymbol[DT_REG] = '-';
    fileSymbol[DT_DIR] = 'd';
    fileSymbol[DT_FIFO] = 'f';
    fileSymbol[DT_SOCK] = 's';
    fileSymbol[DT_CHR] = 'c';
    fileSymbol[DT_BLK] = 'b';
    fileSymbol[DT_LNK] = 'l';
    //fileSymbol[DT_WHT] = 'w';
}

int readSize(char *pathname){
    struct stat buf;
    assert(stat(pathname, &buf) == 0);
    return buf.st_size;
}

long Dir_total_size(char *pathname){
    long size = 0;
    // 打開目錄
    DIR *dirp = opendir(pathname);
    struct dirent* ent;
    if(dirp != NULL){
        ent = readdir(dirp);
    }
    
    // solve problem that some file cannot access
    if(access(pathname, W_OK) != 0){
        return -1;
    }

    // 讀取第一個物件（不一定是檔案）
    while(ent != NULL){
        //『這個目錄』及『上一層目錄』跳過不處理
        if(strcmp(ent -> d_name, ".") == 0 || strcmp(ent -> d_name, "..") == 0){
            ent = readdir(dirp);
            continue;
        }

        // 設定型別並計算數量
        filetype[ent -> d_type] += 1;

        // 製作 路徑/名
        char file_path_name[PATH_MAX] = "";
        strcat(file_path_name, pathname);
        strcat(file_path_name, "/");
        strcat(file_path_name, ent -> d_name);

        // 如果是regular檔案
        if(ent -> d_type == DT_REG){
            size += readSize(file_path_name);
        }
        // 如果是目錄
        else if(ent -> d_type == DT_DIR){
            size += Dir_total_size(file_path_name);
        }
        
        ent = readdir(dirp);
    }
    closedir(dirp);
    return size;
}

void print_type(){
    printf("檔案總類：");
    for(int i = 0; i < 100; i++){
        if(filetype[i] > 0) printf("%c", fileSymbol[i]);
    }
    printf("\n");

    if(filetype[DT_UNKNOWN] > 0) printf("UNKNOWN : %d個\n", filetype[DT_UNKNOWN]);
    if(filetype[DT_FIFO] > 0) printf("FIFO : %d個\n", filetype[DT_FIFO]);
    if(filetype[DT_CHR] > 0) printf("CHR : %d個\n", filetype[DT_CHR]);
    if(filetype[DT_DIR] > 0) printf("DIR : %d個\n", filetype[DT_DIR]);
    if(filetype[DT_BLK] > 0) printf("BLK : %d個\n", filetype[DT_BLK]);
    if(filetype[DT_REG] > 0) printf("REG : %d個\n", filetype[DT_REG]);
    if(filetype[DT_LNK] > 0) printf("LNK : %d個\n", filetype[DT_LNK]);
    if(filetype[DT_SOCK] > 0) printf("SOCK : %d個\n", filetype[DT_SOCK]);
}