
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    int fd;
    int i;

    /*
       参数个数不为2，报错
     */
    if (argc != 2) {
        printf("Invalid number of arguments\n");
        exit(0);
    }

    fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        printf("No such file or directory:");
        printf("%s\n", argv[1]);
        exit(0);
    }

    char buf[50];               //缓冲区
    int count;                  //记录读取个数
    while (1) {
        count = read(fd, &buf, sizeof(buf));
        if (count == -1) {
            printf("READ ERROR\n");     //读取出错
            exit(0);
        } else if (count == 0) {
            break;
        } else {
            for (i = 0; i < count; i++)
                printf("%c", buf[i]);
        }


    }
    printf("\n");
    close(fd);
    return 0;
}
