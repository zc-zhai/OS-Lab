#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 2048

void space_mark ()
{
    printf ("Child process space begins...\n");
}

int main(int argc, const char * argv[]) {
    pid_t pid;
    int shmid;
    char *shm_addr;
    char flag[]="parent";
    char buff[BUFFER_SIZE];
    
    signal (SIGUSR1,space_mark);
    
    //create shared memory space through shmget
    if((shmid = shmget(IPC_PRIVATE,BUFFER_SIZE,0666)) < 0) //0666 = 110 110 110 authority management
    {
        printf("shmget failed!\n");
        return 1;
    }
    else
        printf("Successfully create shared memory!\n");
    
    //create child process through fork
    if((pid = fork()) == -1 )
    {
        printf("fork failed!\n");
        return 1;
    }
    else if(pid == 0)     //child process
    {
        if((shm_addr = shmat(shmid,0,0)) == (void *)-1)  //get shared memory space address
        {
            printf("child shmat failed!\n");
            return 1;
        }
        else
            printf("Child process successfully attached shared memory:%p\n",shm_addr);
        
        while(strncmp(shm_addr,flag,strlen(flag)))
        {
            printf("Child process: waiting for data...\n");
            sleep(10);
        }
        
        strcpy(buff,shm_addr + strlen(flag));  //copy text in shared memory to child process buffer
        
        printf("Child process: shared memory:%s\n",buff);
        
        if(shmdt(shm_addr) < 0)   //disconnect with shared memory through shmdt
        {
            printf("child shmdt failed!\n");
            return 1;
        }
        else
            printf("Child process successfully detached shared memory!\n");
    }
    else
    {
        sleep(1);
        if((shm_addr = shmat(shmid,(void *)0,0)) == (void *)-1)
        {
            printf("parent shmat failed!\n");
            return 1;
        }
        else
        {
            printf("Parent process successfully attached share memory:%p\n",shm_addr);
        }
        
        sleep(1);
        printf("input string:\n");
        
        fgets(buff,BUFFER_SIZE-strlen(flag),stdin);  //copy input to parent process buffer
        
        strncpy(shm_addr+strlen(flag),buff,strlen(buff));
        strncpy(shm_addr,flag,strlen(flag));  //copy parent process buffer to shared memory
        
        if(shmdt(shm_addr) < 0)  //diconnect with shared memory
        {
            printf("parent shmdt failed!\n");
            return 1;
        }
        else
            printf("Parent process successfully detached shared memory!\n");
        
        kill (pid, SIGUSR1);  //signal processing
        
        waitpid(pid,NULL,0);  //waiting for child process to complete
        
        if(shmctl(shmid,IPC_RMID,NULL) == -1)  //delete shared memory through shmctl
        {
            printf("shmctl failed!\n");
            return 1;
        }
        else
            printf("Successfully delete shared memory!\n");
        
        printf("Test completed!\n");
    }
    
    return 0;
}
