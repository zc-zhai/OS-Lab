 #include <stdio.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <stdlib.h>
 #include <netinet/in.h>
 #include <errno.h>
 #include <string.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #define MAXLINE 4096
 int main(int argc,char **argv)
 {
 char *servInetAddr = "127.0.0.1";      //set server ipaddress
 int socketfd;      //socket keynum
 struct sockaddr_in sockaddr;       //socket address ; it is a struct containing domain & port & ipaddress
 char recvline[MAXLINE], sendline[MAXLINE];     //message sent stored in sendline ; message received stored in recvline
 int flen;

 if(argc != 2)
 {
 printf("Please input <client ipaddress> as the second value! \n");
 exit(0);
 }
 
 socketfd = socket(AF_INET,SOCK_STREAM,0);      //create socket
 memset(&sockaddr,0,sizeof(sockaddr));      //clear sockaddr
 
 //set sockaddr values
 sockaddr.sin_family = AF_INET;     //domain
 sockaddr.sin_port = htons(10004);      //port  //htons = host to network short
 inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);        //ipaddress
 
 //create socket connection
 if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
 {
 printf("connect error %s errno: %d\n",strerror(errno),errno);
 exit(0);
 }

 printf("send message to server\n");

 //fgets(sendline,MAXLINE,stdin);        //get message from input
 
 FILE *fp = fopen("./inputtext","r");	//source of message is file "task"
 if(fp == NULL){
    printf("Message:%s\n",strerror(errno));
    return -1;
 }
 fseek(fp,0L,SEEK_END);
 flen=ftell(fp);
 fseek(fp,0L,SEEK_SET);
 fread(sendline, flen, 1, (FILE*)fp);
 fclose(fp);
 sendline[flen]='\0';
 
 printf("%s\n",sendline);
 if((send(socketfd,sendline,strlen(sendline),0)) < 0)       //send message
 {
    printf("send mes error: %s errno : %d",strerror(errno),errno);
    exit(0);
 }
 
 //disconnect socket
 close(socketfd);
 printf("Transportation completed!\n");
 exit(0);
 }