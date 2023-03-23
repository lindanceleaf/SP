#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>
#include <unistd.h>

#define BUFFER_SIZE 8

int main(int argc, char* argv[]){
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 1e7;

    int fd = open("./lockf.db", O_RDWR);
    char buffer[BUFFER_SIZE];
    int offset;
    for(int i = 0; i < 1000; i++){
        // sleep
        nanosleep(&delay, &delay);
        // lock
        lockf(fd, F_LOCK, 0);

        lseek(fd, -5, SEEK_END);
        read(fd, buffer, 5);
        sscanf(buffer, "%d", &offset);
        lseek(fd, offset, SEEK_CUR);
        sprintf(buffer, "%d", offset + 1);
        write(fd, buffer, 5);
        
        // unlock
        lockf(fd, F_ULOCK, 0);
    }
}