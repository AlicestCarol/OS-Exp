

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
    memset(argv,0,sizeof(argv));
    if (command == NULL)        //判空
    {
        exit(0);
    }

    int len = strlen(command);  //获取命令长度，将最后一位即'\n'改为'\0'即可
    if(len==1)
        return ;
    command[len - 1] = '\0';

    char *p = (char *) malloc(100);
    strcpy(p, command);

    // split command into argv   根据结束条件  argv[count] == NULL 所以之后不需要再进行写NULL操作

    argv[count] = strtok(p, " ");
    while (argv[count] != NULL) {
        argv[++count] = strtok(NULL, " ");
    }



    if (strcmp(argv[0], "exit") == 0)   //exit操作
    {
        exit(0);
    } else if (strcmp(argv[0], "pwd") == 0)     //pwd命令     
    {
        char buf[100];
        getcwd(buf, 100);
        printf("%s\n", buf);
        return;
    } else if (strcmp(argv[0], "cd") == 0)      //cd命令,错误返回值为-1
    {
        if (chdir(argv[1]) == -1) {
            printf("cd:%s:", argv[1]);
            printf("No such file or directory\n");
        }
        return;
    } else {
        pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            
            execvp(argv[0], argv);
           
        }
        wait(NULL);
    }


}



int main()
{
    while (1) {
        printf("> ");
        char line[100];
        fgets(line, 100, stdin);        //获取命令

        mysys(line);
    }
    return 0;
}
