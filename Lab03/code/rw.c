#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <errno.h> 
#include <sys/time.h>    
#include <unistd.h>  
  
#define SIZE 1024  
  
int main(int argc,char *argv[])  
{  
    struct timeval timeStart, timeEnd; //create timestamps timeStart & timeEnd
    double runTime=0;   //runTime = timeEnd - timeStart
    gettimeofday( &timeStart, NULL );   //get timestamp timeStart
    int from_fd;  //read from file num = from_fd
    int to_fd;  //write to file num = to_fd
  
    //used for counting
    int nread;
    int nwrite;  
  
    char buff[SIZE];  
    char *ptr;  
  
    if(argc != 3)  
    {  
        printf("Usage: %s inputfilename outputfilename\n",argv[0]);
        exit(-1);  
    }  
  
    if((from_fd = open(argv[1],O_RDONLY)) == -1)  
    {  
        printf("Open %s failed!\n",argv[1]);  
        exit(-1);  
    }  
      
    if((to_fd = open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR)) == -1)  
    {  
        printf("Open %s failed!\n",argv[2]);  
        exit(-1);  
    }  
  
    while(nread = read(from_fd,buff,SIZE))  
    {  
        if((nread == -1) && (errno != EINTR))  
        {  
            break;  
        }  
        else  
            if(nread > 0)  
            {  
                ptr = buff;  
  
                while(nwrite = write(to_fd,ptr,nread))  
                {  
                    if((nwrite == -1) && (errno != EINTR))  
                    {  
                        break;  
                    }  
                    else  
                        if(nwrite == nread)  
                        {  
                            break;  
                        }  
                        else  
                            if(nwrite > 0)  
                            {  
                                ptr += nwrite;  
                                nread -= nwrite;  
                            }  
                }  
  
                if(nwrite == -1)  
                {  
                    break;  
                }  
  
            }  
    }  
  
    close(from_fd);  
    close(to_fd);

    printf("Copy successfully!\n");
    gettimeofday( &timeEnd, NULL ); //get timestamp timeEnd
    //calculate runTime
    //tv_sec -> s ; tv_usec -> us
    runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
    printf("runTime is %lf\n", runTime);

    exit(0);  
    return 0;  
} 