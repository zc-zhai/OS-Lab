#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h> 
#include <sys/time.h>    
#include <unistd.h>   
  
#define READ_BUFF 1024  
int main(int argc,char *argv[])  
{  
    struct timeval timeStart, timeEnd; //create timestamps timeStart & timeEnd
    double runTime=0; //runTime = timeEnd - timeStart
    gettimeofday( &timeStart, NULL );   //get timestamp timeStart
    if(argc < 3)  
    {  
        printf("./frw sour dest\n");  
        return -1;  
    }

    // read from file argv[1]
    // write to file argv[2]
    // e.g. : ./frw ./inputtext ./outputtext 
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

    int rest = flen % READ_BUFF;

    char buf[READ_BUFF];  
    int nread;
      
    while( nread = fread(buf,READ_BUFF,1,fileSourceHandler) )  
    {  
        fwrite(buf,READ_BUFF,1,fileDestHandler);
        //printf("%d",nread);  
    }

    fread(buf,rest,1,fileSourceHandler);
    fwrite(buf,rest,1,fileDestHandler);
  
    fclose(fileDestHandler);  
    fclose(fileSourceHandler);  
  
    printf("Copy successfully!\n");
    gettimeofday( &timeEnd, NULL ); //get timestamp timeEnd
    // calculate runTime
    // tv_sec -> s ; tv_usec -> us
    runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
    printf("runTime is %lf\n", runTime);  
    return 0;  
} 