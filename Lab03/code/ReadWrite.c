#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h> 
#include <sys/time.h>    
#include <unistd.h> 
#define READ_BUFF 1024 
int main(int argc,char *argv[])  
{  
    if(argc != 3)  
    {  
        printf("Usage: %s inputfilename outputfilename\n",argv[0]);
        exit(-1);  
    }  

    char buf[READ_BUFF];
    int i;  
    
    // fpSrc -> file to read
    // fpDst -> file to write
    FILE *fpSrc = NULL;  
    FILE *fpDst = NULL;

    if ((fpSrc=fopen(argv[1],"r"))==NULL)  
    {  
        printf("Open %s failed!\n",argv[1]);  
        exit(0);  
    }  
    if ((fpDst=fopen(argv[2],"w"))==NULL)  
    {  
        printf("Open %s failed!\n",argv[2]);  
        exit(0);  
    }
    
    // get file length
    int flen;
    fseek(fpSrc,0L,SEEK_END);
    flen=ftell(fpSrc);
    fseek(fpSrc,0L,SEEK_SET);

    char op;
    printf("Choose the operation (1.fscanf&fprintf / 2.fgetc&fputc / 3.fgets&fputs) >>> ");
    
    do op = getchar();
	while (op == '\n' || op == '\t' || op == ' ');
    
    // use fscanf & fprintf
    if (op == '1'){
        while(fscanf(fpSrc,"%s",buf)!=EOF)  {    
            fprintf(fpDst,"%s ",buf);  
        }
    } 

    // use fgetc & fputc
    else if(op == '2'){
        for(i=0;i<flen;i++){
            fputc(fgetc(fpSrc),fpDst);        
        }
    }

    // use fgets & fputs
    else if(op == '3'){
        while(fgets(buf,READ_BUFF,fpSrc)){
            fputs(buf,fpDst);
        }
    } 
    else{
        printf("Please input valid operation number!\n");
        exit(-1);
    }
    fclose(fpSrc);  
    fclose(fpDst);  
    exit(0);  
}  