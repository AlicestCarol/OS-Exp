#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
    int fd1;
    int fd2;
    mode_t mode = 0777;
    /*
       参数个数不为3，报错
     */
    if (argc != 3) {
        printf("Invalid number of arguments\n");
        exit(0);
    }

    fd1 = open(argv[1], O_RDONLY, mode);
    if (fd1 < 0) {
        printf("No such file or directory:");
        printf("%s\n", argv[1]);
        exit(0);
    }

    fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fd2 < 0) {
        printf("Create File Error:");
        printf("%s\n", argv[2]);
        exit(0);
    }

    char buf[50];               //缓冲区
    int count1;                 //记录读取个数
    int count2;                 //记录写入个数
    while (1) {
        count1 = read(fd1, &buf, sizeof(buf));
        if (count1 == -1) {
            printf("READ ERROR\n");     //读取出错
            exit(0);
        } else if (count1 == 0) {
            break;
        } else {
            count2 = write(fd2, &buf, count1);
            if (count2 == -1) {
                printf("WRITE ERROR\n");        //写入出错
                exit(0);
            }

        }


    }

    close(fd1);
    close(fd1);
    return 0;
}
