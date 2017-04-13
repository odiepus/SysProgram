#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <error.h>
#include <string.h>



void createFamilia(int children)
{
    int id =0;
    int i = 0;

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

    j = children;

    while (loopThisTimes)
    {
        if(id != 0) break;

        switch(fork())
        {
            case -1:
                perror("fork failed\n");
                exit(-1);
            case 0:
                id = i;                                 //give this process an id

                close(fd[(id * 2) + 1]);                //close this process write to its own pipe

                int k = 0;
                for(; k < numOfFd; k+=2)                //close this process reads from other pipes
                {
                    if (k != 2 * id)
                    {
                        close(fd[k]);
                    }
                }

                break;
            default:
                if (j == children)          //earlier i made children == to j for this if
                {                       //I did this so i can close parents appropriate
                    close(fd[1]);       //fd's only once while in the loop
                    int k = 2;
                    for(; k < numOfFd; k+=2)    //close reads from other pipes
                    {
                        close(fd[k]);
                    }
                }
                j = 0;
                i++;
                loopThisTimes--;
                break;
        }
    }

    int rng = RAND_MAX/10;
    rng *=10;
    int n = 0;
    int l = 0;
    int num = 0;

    for(; n < children + 2; n++)
    {
        do{
            num = rand();
            l = num%10;
        }
        while(num >= rng || i == l);
        fprintf(stdout, "%d\n", l);
    }



}


int main(void)
{
    createFamilia(4);


    return 0;
}
