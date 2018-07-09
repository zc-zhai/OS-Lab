#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 2048
#define MSGKEY 5678
#define MAX_TEXT 512

struct msg_struct   //message = message.key + message.value
{
    long int msg_type;
    char msg_text[BUFFER_SIZE];
};

int main(int argc, const char * argv[])
{
    pid_t pid;
    int running = 1;
    int msgid;
    
    struct msg_struct message;
    
    char buffer[BUFFER_SIZE];
    
    long int msg_to_receive = 1;
    
    msgid = msgget(MSGKEY,0666|IPC_CREAT);  //create message queue
    
    if(msgid == -1)
    {
        printf("msgget failed！\n");
        return 1;
    }
    
    if((pid = fork())== -1)  //create child process
    {
        printf("fork failed!\n");
        return 1;
    }
    else if (pid == 0)    //child process 
    {
        while(running)       //receive message from the queue
        {
            if(msgrcv(msgid,(void *)&message,BUFFER_SIZE,msg_to_receive,0) == -1)
            {
                printf("msgrcv failed!\n");
                return 1;
            }
            
            printf("%s",message.msg_text);
            
            if(strncmp(message.msg_text,"end",3) == 0)
            {
                running = 0;    //input "end" to exit
            }
        }
    }
    
    else  //parent process
    {
        while(running)
        {
            fgets(buffer,BUFFER_SIZE,stdin);    //copy input to parent process buffer
            message.msg_type = 5;
            strcpy(message.msg_text,buffer);    //copy parent process buffer to message
            
            if(msgsnd(msgid,(void *)&message,MAX_TEXT,0) == -1) //send message to messageQueue
            {
                printf("msgsed failed!\n");
                return 1;
            }
            
            if(strncmp(message.msg_text,"end",3) == 0)
            {
                running = 0;    //input "end" to exit
            }
        }
    }
    
    //delete message queue
    if(msgctl(msgid,IPC_RMID,0) == -1)
    {
        printf("msgctl(IPC_RMID) failed!\n");
        return 1;
    }
    
    return 0;
}
