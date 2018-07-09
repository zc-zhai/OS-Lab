#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#define SIZE 4096  
      
int main(int argc,char *argv[])  
{  
    if(argc < 3)  
    {  
        printf("./pipe sour dest\n");  
        return -1;  
    }
    char buffer[SIZE],sendline[SIZE];   //buffer used for storing writing text ; sendline used for storing reading text
    
    // create pipe
    int fd[2]; 
    int ret=pipe(fd);  
    if(ret!=0)  
    {  
        perror("Create pipe failed!\n");  
        exit(1);  
    }

    // read from file argv[1]
    // write to file argv[2]
    // e.g. : ./pipe ./task ./out
    FILE * fileSourceHandler = NULL;  
    FILE * fileDestHandler = NULL;  
    fileSourceHandler = fopen(argv[1],"r");  
    fileDestHandler = fopen(argv[2],"w+");  
    if(fileSourceHandler == NULL || fileDestHandler == NULL)  
    {  
        printf("Open %s or %s failed:%s\n",argv[1],argv[2],strerror(errno));  
        return -2;  
    }  

    // get file length
    int flen;
    fseek(fileSourceHandler,0L,SEEK_END);
    flen=ftell(fileSourceHandler);
    fseek(fileSourceHandler,0L,SEEK_SET);

    // fork ; create child process
    pid_t pid;  
    pid=fork();

    // child process   
    if(pid==0)  
    {  
        close(fd[1]);
        read(fd[0],buffer,flen);
        //printf("buffer:%s\n",buffer);
        fwrite(buffer,flen,1,fileDestHandler);
        close(fd[0]);    
    }  

    // parent process
    else if(pid!=-1)  
    {     
        close(fd[0]);
        fread(sendline,flen,1,fileSourceHandler);
        //printf("sendline:%s\n",sendline);
        write(fd[1],sendline,flen+1);
        close(fd[1]);
    }  
    else  
    {  
        perror("fork error");  
        exit(1);  
    }
    fclose(fileDestHandler);  
    fclose(fileSourceHandler); 
    exit(0); 
} 