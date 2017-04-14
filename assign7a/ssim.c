#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <error.h>
#include <string.h>



int main(void)
{
    int id =0;
    int i = 0;
    int children = 1;
    int numOfFd = 0;

    numOfFd = children * 2;
    int loopThisTimes = children;

    int fd[2];
    pipe(fd);

    int j = 1;
    for(; j < children; j++)
   {
       pipe(fd + 2*j);
   }

    while (loopThisTimes !=0)
    {
        switch(fork())
        {
            case -1:
                perror("fork failed\n");
                exit(-1);
            case 0:

                id = ++i;                                 //give this process an id

                int k = 0;
                for(; k < numOfFd; k++)                //close this process reads from other pipes
                {
                    if (2*k != id)
                    {
                        close(fd[2*k]);
                    }
                }
                write(fd[1], "FUKKK", 5);
                loopThisTimes = 0;

                break;
            default:
                if (id == 0 && i == 0)          //earlier i made children == to j for this if
                {                               //I did this so i can close parents appropriate

                    close(STDIN_FILENO);
                    int k = 0;
                    for(; k < numOfFd; k+=2)                //close this process reads from other pipes
                    {
                        if (k != id)
                        {
                            close(fd[k]);
                        }
                    }
                    i++;
                    loopThisTimes--;
                }else if(id == 0)
                {
                    i++;
                    loopThisTimes--;
                }
                break;
        }
    }


    printf("stdout works!!!%d\n", id);

    srand(5972261 * i);
    int rng = RAND_MAX / children;
    rng *= children;
    int n, e = 0, num = 0;

    int randoNums[children + 2];
    for (n = 0; n < children+2; n++)
    {
        do{
            num = rand();
            e = num % 10;
            randoNums[n] = e;
        }
        while(num >= rng || id == e);
    }



    char mesg[] = {'p', 'r', 'o', 'c', 'e', 's', 's', '0'};

    size_t len = strlen(mesg);

    if (id != 0)
    {
        mesg[len-1] = id;
    }
    printf("%s\n", mesg);

    int v = 0;
    for(; v < 12; v++)
    {
        if (randoNums[v] == (2*v) + 1 && randoNums[v] != (2*id) + 1)
        {
                printf("process %d||writing to fd[%d]\n", id, (2*v) + 1);
            if(write(fd[(2*v) + 1], &mesg, len) == -1)
            {
                perror("write to PIPE error");
                exit(-1);
            }
        }
    }

    if(id == 0){
        write(fd[3], &mesg, len);
    }


    char readBuf[256];

    if(id == 1){
        int x =read (fd[2], &readBuf, sizeof(readBuf));
        write(STDOUT_FILENO, "fuck", 4);
    }

    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    close(fd[2*id]);





    return 0;
}

