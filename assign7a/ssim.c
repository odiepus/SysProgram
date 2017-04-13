#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <error.h>
#include <string.h>



int *rando(int x, int i)
{
    srand(5972261 * i);
    int rng = RAND_MAX / 10;
    rng *= 10;
    int n, j, num;

    int randoNums[x + 2];
    for (n = 0; n < x+2; n++)
    {
        do{
            num = rand();
            j = num % 10;
            randoNums[n] = j;
        }
        while(num >= rng || i == j);
    }

        return randoNums;
}


int main(void)
{
    int id =0;
    int i = 0;
    int children = 3;
    int numOfFd = 0;

    numOfFd = children * 2;
    int loopThisTimes = children;

    int fd[numOfFd];
    pipe(fd);

    int j = 1;
    for(; j < children; j++)
    {
        pipe(fd + 2);
    }

    while (loopThisTimes !=0)
    {
        switch(fork())
        {
            case -1:
                perror("fork failed\n");
                exit(-1);
            case 0:

                sleep(3);
                id = ++i;                                 //give this process an id

                close(fd[(id * 2) + 1]);                //close this process write to its own pipe

                int k = 0;
                for(; k < numOfFd; k+=2)                //close this process reads from other pipes
                {
                    if (k != 2 * id)
                    {
                        close(fd[k]);
                    }
                }

                loopThisTimes = 0;

                break;
            default:
                if (id == 0 && i == 0)          //earlier i made children == to j for this if
                {                       //I did this so i can close parents appropriate
                    close(fd[1]);       //fd's only once while in the loop
                    int k = 2;
                    for(; k < numOfFd; k+=2)    //close reads from other pipes
                    {
                        close(fd[k]);
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

    printf("id : %d ==== i : %d\n", id, i);

    int *k = rando(children, id); int h = 0;
    for(; h < sizeof(k); h++)
    {
        //finsisht this shit asap
        printf("id = %d  == send to : %d", id, k[h]);
    }


    return 0;
}

