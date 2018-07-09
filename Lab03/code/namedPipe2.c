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
        printf("./namedPipe2 dest\n");  
        return -1;  
    }
    int fd,n;
    char buf[SIZE];
    fd = open("fifo",O_RDONLY); //open fifo in mode read-only

    FILE * fileDestHandler = NULL;
    fileDestHandler = fopen(argv[1],"w+");
    if(fileDestHandler == NULL)  
    {  
        printf("Open %s failed:%s\n",argv[1],strerror(errno));  
        return -2;  
    } 

    // read txt from fifo and then write into fileDestHandler
    n = read(fd,buf,SIZE);
    buf[n]='\0';
    fputs(buf,fileDestHandler);

    fclose(fileDestHandler);
    close(fd);
}