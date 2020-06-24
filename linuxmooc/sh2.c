
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void mysys(char *command)
{
    pid_t pid;
    char *argv[10];
    int count = 0;
    char *temp[2];
    char *input = NULL;
    char *output = NULL;
    memset(argv, 0, sizeof(argv));
    memset(temp, 0, sizeof(temp));
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

    if (strrchr(p, '>') != NULL)        //判断是否有>
    {
        temp[count] = strtok(p, ">");   //temp[0]为命令
        temp[++count] = strtok(NULL, ">");      //temp[1]存储文件名
        output = strtok(temp[1], " ");  //做一次处理，防止出现"  a.txt"带空格情况
        count = 0;
        argv[count] = strtok(temp[0], " ");
        while (argv[count] != NULL) {
            argv[++count] = strtok(NULL, " ");
        }

    } else if (strrchr(p, '<') != NULL) {
        temp[count] = strtok(p, "<");   //temp[0]为命令
        temp[++count] = strtok(NULL, "<");      //temp[1]存储文件名
        input = strtok(temp[1], " ");   //做一次处理，防止出现"  a.txt"带空格情况
        count = 0;
        argv[count] = strtok(temp[0], " ");
        while (argv[count] != NULL) {
            argv[++count] = strtok(NULL, " ");
        }
    }

    else {
        argv[count] = strtok(p, " ");
        while (argv[count] != NULL) {
            argv[++count] = strtok(NULL, " ");
        }
    }

    if (strcmp(argv[0], "exit") == 0)   //exit操作
    {
        exit(0);
    }
    //  改为内置
    //else if (strcmp(argv[0], "pwd") == 0)     //pwd命令     
    // {
    //     char buf[100];
    //     getcwd(buf, 100);
    //     printf("%s\n", buf);
    //     return;
    // } 
    else if (strcmp(argv[0], "cd") == 0)        //cd命令,错误返回值为-1
    {
        if (chdir(argv[1]) == -1) {
            printf("cd:%s:", argv[1]);
            printf("No such file or directory\n");
        }
        return;
    } else {
        if (temp[0] != NULL)    //重定向
        {
            pid = fork();
            if (pid == 0) {
                int fd;
                if (input != NULL) {
                    fd = open(input, O_RDWR, 0666);
                    dup2(fd, 0);
                }
                if (output != NULL) {
                    fd = open(output, O_CREAT | O_RDWR | O_TRUNC, 0666);
                    dup2(fd, 1);
                }
                close(fd);
                execvp(argv[0], argv);
            }
            wait(NULL);
        } else {
            pid = fork();
            if (pid == 0) {
                int error = execvp(argv[0], argv);
                if (error < 0) {
                    perror("execvp");
                }
            }
            wait(NULL);
        }

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
