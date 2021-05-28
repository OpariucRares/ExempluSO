#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



/*
 *  worker2 
 *
 *  open the fifo to read
 *  write to two files stat.txt and fixed.txt
 *
 * */


int main()
{
    const char tab[] = {'\t'};
    //Need to read from the fifo
    int fd = open("MyOtherFifo", O_RDONLY );
    int chars_cnt = 0;
    int lines_cnt = 0;

    //Need file descriptors for the stat and fixed files
    int fd_fixed = open("fixed.txt", O_WRONLY | O_CREAT , 0600);
    int fd_stat = open("stat.txt", O_WRONLY | O_CREAT, 0600);

    char buf[32];
    while(1){
        
        int red = read(fd, buf, sizeof(buf));

        if(red <= 0)
            break;
        
        int i = 0;
        for(i=0; i < red ; ++i){
            //Just write the char forward to the file
            write(fd_fixed, (buf + i), sizeof(char));

            //If we find a newline we need to add a tab
            if(buf[i] == '\n')
                write(fd_fixed, tab, sizeof(char));

            if(buf[i] == '\n')
                lines_cnt++;
        }
        chars_cnt += red;
    }
    close(fd);
    sprintf(buf, "chars: %d\n", chars_cnt);
    write(fd_stat, buf, strlen(buf) );
    sprintf(buf, "lines: %d\n", lines_cnt);
    write(fd_stat, buf, strlen(buf) );

    close(fd_fixed);
    close(fd_stat);
}
