
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>


void mysys(char *command)
{
    pid_t pid;
    char *argv[10];
    int count = 0;

    if (command == NULL) {
        exit(0);
    }



    /*
       方法一:使用strtok函数
     */
    char *p = (char *) malloc(100);
    strcpy(p, command);
    argv[count] = strtok(p, " ");
    while (argv[count] != NULL) {
        argv[++count] = strtok(NULL, " ");
    }

    /*
       方法二:类似于编译原理的词法分析过程，通过循环将大的字符串进行split

       int  i;
       for(i=0;command[i]!='\0';i++)
       {
       if(command[i]==' ')
       continue;
       else
       {
       char *s;
       s=(char *)malloc(sizeof(char)*100);
       int m=0;
       while(command[i]!=' '&&command[i]!='\0')
       {
       s[m++]=command[i++];
       }
       s[m]='\0';
       i--;
       argv[count]=s;
       count++;
       }
       }
       argv[count]=NULL;

     */

    pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid == 0) {
        int error = execvp(argv[0], argv);
        if (error < 0) {
            perror("execvp");
        }
    }
    wait(NULL);


}



int main()
{
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
