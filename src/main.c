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
 *  Open a file
 *  Read bytes from the file
 *  Delete punctuation errors
 *  Send the data trough a fifo channel to another program
 *  The second program capitalizes the right charcters in the text and sends it to a third
 *  The third keeps count of #characters recienved and writes it to statistica.txt
 *  and outputs the text to text_fixed.txt after indentig the paragphs
 *
 * */


int main(){
    //Deschidem fisieru text.txt din care vrem sa citim
    int fd = open( "text.txt", O_RDONLY );
    //Pregatim un buffer in care sa citim caraterele din fisier
    char buf[32];
    //Pregatim fifo-ul pe care il vom folosi sa transmitem datale catre procesul fiu 1
    mkfifo("MyFifo", 0600 );


    if( fork() == 0 )
    {
        execl("bin/worker1", "worker1", NULL );
    }


    //Deschidem fifo-ul pentru a scrie in el 
    int fd_fifo = open( "MyFifo", O_WRONLY );
    //Citim intr-un while in caz ca exista mai mult tex tin fisier decat avem spatiu in buffer
    
    int seen_space = 0;
    while(1){
        //How many bytes were we able to read?
        int red = read(fd, buf, sizeof(buf) );

        //If there are no more bytes to read we can break out of the loop
        if( red <= 0 )
            break;
        int i = 0;
        for(i = 0; i < red ; ++i ){

           if( buf[i] == ' ' && seen_space == true )
                continue;

           if( buf[i] == ' ' ){
                seen_space = true;
           }
           else
                seen_space = false;
            
            //Scrim un singur caracter in fifo
            write(fd_fifo, (buf + i), sizeof(char) );
            if( strchr(".,!?():;",buf[i]) != NULL ){
                buf[i] = ' ';
                write(fd_fifo, (buf + i), sizeof(char) );
                seen_space = true;
            }

        }

    }
    close(fd);
    close(fd_fifo);
    int ret;
    while( wait(&ret) > 0 ); 
    unlink("MyFifo");
    unlink("MyOtherFifo");
}
