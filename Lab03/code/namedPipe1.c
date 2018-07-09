#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h> 

#define SIZE 4096

int main(int argc,char *argv[]){
    if(argc < 2)  
    {  
        printf("./namedPipe1 sour\n");  
        return -1;  
    }
    int fd;
    char sendline[SIZE];
    mknod("fifo",S_IFIFO|0666,0); //create named pipe fifo
    fd = open("fifo",O_WRONLY); //open fifo in mode write-only

    FILE * fileSourceHandler = NULL;
    fileSourceHandler = fopen(argv[1],"r");
    if(fileSourceHandler == NULL)  
    {  
        printf("Open %s failed:%s\n",argv[1],strerror(errno));  
        return -2;  
    } 

    // get file length
    int flen;
    fseek(fileSourceHandler,0L,SEEK_END);
    flen=ftell(fileSourceHandler);
    fseek(fileSourceHandler,0L,SEEK_SET);
    
    // read txt from fileSourceHandler and then write into fifo
    fread(sendline,flen,1,fileSourceHandler);
    write(fd,sendline,flen+1);
    
    close(fd);
    fclose(fileSourceHandler); 

} 