#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * Worker 1
 * Ia din canalul MyFifo creat anterior si pune intr-un nou canal (MyOtherFifo) ccu textul corectat (Litera mare dupa un semnele de punctuatie aferente)
 * */


int main()
{
    //Open the fifo.
    int fd = open("MyFifo", O_RDONLY ); 
    if(fd == -1)
        perror("open myfifo");
    //Create another fifo
    mkfifo("MyOtherFifo", 0600);
    //Open the fifo for writing
    int fd_fifo = open("MyOtherFifo", O_WRONLY ); 
    //Write to another fifo
    //Capitalize if needed
    char buf[32];

    bool seen_punct = false;

    while(1){
        int red = read(fd, buf, sizeof(buf));

        if( red <= 0 ){
            break;
        }
        int  i = 0;
        for(i = 0 ; i < red ; ++i ){
            if( seen_punct )
                buf[i] = toupper(buf[i]); 
            if( strchr(".!?", buf[i]) != NULL )
                seen_punct = true;
            else if ( buf[i] != ' ' )
                seen_punct = false;

            //scrie in noul canal fifo textul corectat
            write(fd_fifo, (buf + i), sizeof(char));                
        }

    }

    close(fd);
    close(fd_fifo);
}
