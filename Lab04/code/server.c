 #include <stdio.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <string.h>
 #include <netinet/in.h>
 #include <stdlib.h>
 #include <errno.h>
 #include <unistd.h>
 #include <arpa/inet.h>
 
 #define MAXLINE 4096
 int main(int argc,char **argv)
 {
 int listenfd,connfd;
 struct sockaddr_in sockaddr;
 char buff[MAXLINE];
 int n;
 
 memset(&sockaddr,0,sizeof(sockaddr));      //clear sockaddr
 
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //htonl = host to network long
 sockaddr.sin_port = htons(10004);
 
 listenfd = socket(AF_INET,SOCK_STREAM,0);      //create socket
 
 bind(listenfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr));     //bind function
 
 listen(listenfd,MAXLINE);     //listen function ; waiting for accepting connect request from client

 
 printf("Please wait for the client information...\n");
 
 for(;;)
 {
 if((connfd = accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)        //accept connect request
 {
 printf("accpet socket error: %s errno :%d\n",strerror(errno),errno);
 continue;
 }
 
 n = recv(connfd,buff,MAXLINE,0);
 buff[n] = '\0';
 //printf("recv msg from client:%s",buff);
 FILE *p = fopen("./outputtext","w");	//output message into file "out"
 if(p == NULL){
    printf("Message:%s\n",strerror(errno));
    return -1;
 }
 fputs(buff, p);
 fclose(p);
 close(connfd);
 }
 close(listenfd);
 }