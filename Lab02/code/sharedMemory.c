#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define SHMKEY1 18001 	//shared memory key = 18001
#define SIZE 1024 		//shared memory size = 1024
#define SEMKEY1 19001 	//sem1 key = 19001
#define SEMKEY2 19002 	//sem2 key = 19002

//sem operating function
static void semcall(sid, op)	
int sid, op;	//sid = sem key ; op = 1 / -1
{
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = op;
	sb.sem_flg = 0;
	if (semop(sid, &sb, 1) == -1)
		system("semop");
};

//create and initialize sem
int creatsem(key)
key_t key;
{
	int sid;
	union semun {   /* if not defined in sem.h */
		int val;
		struct semid_ds *buf;
		ushort *array;
	} arg;
	if ((sid = semget(key,1,0666 | IPC_CREAT)) == -1)	//create a sem ; authority management = 110 110 110
		system("semget");

	arg.val = 1;	//initial value = 1

	if (semctl(sid,0,SETVAL,arg) == -1)
		system("semctl");
	return(sid);	//return sem key
}
void P(sid)
int sid;
{
	semcall(sid, - 1); //sem_Wait
}

void V(sid)
int sid;
{
	semcall(sid,1);	//sem_Post
}

int main()
{
	char *segaddr;
	int segid,sid1,sid2;
	//create shared memory
	if ((segid = shmget(SHMKEY1,SIZE,IPC_CREAT | 0666)) == -1)
        system("shmget");
	//attach to shared memory
	segaddr = shmat(segid,0,0);
	sid1 = creatsem(SEMKEY1); //create sem1 & sem2 ; initial values = 1
	sid2 = creatsem(SEMKEY2);
	P(sid2);                //set sem2 value = 0
	/* message waiting for adding into shared memory = sem1
	   message in shared memory waiting for extracting = sem2
	*/
	if (!fork()){       //child process : receive message from shared memory
	while(1){
        P(sid2);
        printf("Received from Parent : %s\n",segaddr);

        FILE *p = fopen("./out","w");	//output message into file "out"
        if(p == NULL){
            printf("Message:%s\n",strerror(errno));
            return -1;
        }
        fputs(segaddr, p);
        fclose(p);
        char cmd;
        char argu1[8] = {""};
        char argu2[3] = {""};
        char argu3[3] = {""};
        strncpy(argu1,segaddr,7);
        strncpy(argu2,segaddr+8,2);
        strncpy(argu3,segaddr+11,2);
		execl(argu1, argu2, argu3, (char*)0);	//end current process , start other processes according to message context
		printf("Execl error.Enter 'r' to try again.\n");	//if message does not contain a valid order , error
		cmd = getchar();
		if (cmd != 'r'&&cmd != 'R'){
            exit(2);
		}
        V(sid1);}
    }else{             //parent process : add message into shared memory
        while(1){
        P(sid1);
       char buff[20];
        FILE *fp = fopen("./task","r");	//source of message is file "task"
        if(fp == NULL){
            printf("Message:%s\n",strerror(errno));
            return -1;
        }
        fgets(buff, 20, (FILE*)fp);
        fclose(fp);
        strcpy (segaddr,buff);
		V(sid2);
	}
	}
	return 0;
}
