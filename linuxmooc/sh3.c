
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAX_ARGC      16
#define MAX_COMMANDS  16

struct command {
    int argc;
    char *argv[MAX_ARGC];
    char *input;
    char *output;
};

int command_count = 0;
struct command commands[MAX_COMMANDS];

// 字符串分割函数
void parse_commands(char *line) 
{
    char *p = NULL, *q = NULL,*r = NULL;
    command_count = 0;
    commands[command_count].argc = 0;
    int t = 0;
    while ((commands[command_count].argv[commands[command_count].argc] = strtok_r(line, "|", &p)) != NULL) {
        int count = 0;
        line = commands[command_count].argv[commands[command_count].argc];
        char *temp[10];
        memset(temp,0,sizeof(temp));
        while((temp[count]=strtok_r(line, " ", &q))!=NULL)
        {
            count++;
			line=NULL;
        }
        commands[command_count].input = NULL;
        commands[command_count].output = NULL;
        int i;
		for(i=0;i<count;i++)
		{
			if(strcmp(temp[i],"<")==0)
			{
				commands[command_count].input = temp[i+1];
				i++;
			}
			else if(strcmp(temp[i],">")==0)
			{
				commands[command_count].output = temp[i+1];
				i++;
			}
			else if(temp[i][0]=='<')
			{
			   commands[command_count].input = strtok_r(temp[i], "<", &r);	
			}
			else if(temp[i][0]=='>')
			{
				commands[command_count].output = strtok_r(temp[i], ">", &r);
			}
			else
			{
				commands[command_count].argv[commands[command_count].argc++]=temp[i];
			}
			
		}
            
		command_count++;
        commands[command_count].argc = 0;
        line = NULL;
    }
}


void command_dump()
{
    int i = 0, j = 0;
    for (i = 0; i < command_count; i++) {
        for (j = 0; j < commands[i].argc; j++) {
            printf("%d,%s ", j, commands[i].argv[j]);
        }
        printf("\n");
        if (commands[i].input != NULL)
            printf("in:%s ", commands[i].input);
        if (commands[i].output != NULL)
            printf("out:%s ", commands[i].output);
        printf("\n");
    }
}

void exec_simple(int child_count)
{

    // 重定向输入
    if(commands[child_count].input!=NULL)
        {
            int infd;
            infd = open(commands[child_count].input,O_RDWR, 0666);
            dup2(infd,0);
            close(infd);
        }

    //重定向输出
    if (commands[child_count].output != NULL) 
    {
        int outfd;
        outfd =open(commands[child_count].output,O_CREAT | O_RDWR | O_TRUNC,0666);
        dup2(outfd, 1);
        close(outfd);
    }

    execvp(commands[child_count].argv[0], commands[child_count].argv);
}

void exec_pipe(int child_count)
{

    if (child_count == 0)
        {
            exit(0);
        }
    
    // 单命令直接执行exec_simple
    if(command_count==1)
    {
        exec_simple(0);
        exit(0);
    }

    pid_t pid;
    int fd[2];
    pipe(fd);

    // 递归执行多管道命令

    pid = fork();
    if (pid == 0) {
        //使用管道的写端
        dup2(fd[1], 1);   
        close(fd[0]);
        close(fd[1]);
        exec_pipe(child_count - 1);
    }
    //使用管道的读端
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    exec_simple(child_count - 1);
    wait(NULL);
}



void mysys(char *line)
{
    int len = strlen(line);     //获取命令长度，将最后一位即'\n'改为'\0'即可
    if(len==1)                  
        return ;
    line[len - 1] = '\0';       
    parse_commands(line);       //划分命令
    //command_dump();

    if (strcmp(commands[0].argv[0], "exit") == 0) {             
        exit(0);
    } else if (strcmp(commands[0].argv[0], "cd") == 0) {
        if (chdir(commands[command_count - 1].argv[1]) == -1) {
            printf("cd:%s:", commands[command_count - 1].argv[1]);
            printf("No such file or directory\n");
        }
        return ;
    } else {
        pid_t pid;
        pid = fork();
        if (pid == 0)
            exec_pipe(command_count);
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
