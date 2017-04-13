#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int childLoop(int n)
{
    int i = 0;

    while(i < n)
    {
        switch(fork())
        {
            case -1:
                perror("fork fail\n");
                exit (-1);
            case 0:
                sleep(10);
                i++;
                printf("%d\t%d\n", i, getpid());
                break;
            default:
                if(i == 0)return 0;
                if(i != 0)exit(0);
        }
    }
    exit (0);
}
