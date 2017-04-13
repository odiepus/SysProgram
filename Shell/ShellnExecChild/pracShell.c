#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>



int main(void)
{
    char buf[1024];
    char *sargv[16];
    int fd[2];
    char printThis[4096];

    int i = 0;
    int forc = 0;

    if(pipe(fd) == -1)
    {
        perror("Failed to create pipe\n");
    }


    printf("\n==>> ");
    gets(buf);
    printf("BEFORE WHILE LOOP FOR SHELL%s", buf);
    while((strcmp(buf, "exit")) !=0)
    {
        forc = fork();
        if (forc == -1)
        {
            perror("Fork failed\n");
            exit(-1);
        }
        switch(forc)
        {
            case -1:
                perror("Fork failed\n");
                exit(-1);
            case 0:
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);

                printf("FROM CHILD 1111%s", buf);
                sargv[0] = strtok(buf, " ");
                printf("sargv[0] : %s\n", sargv[0]);

                while(sargv[i++] != NULL)
                {
                    sargv[i] = strtok(NULL, " ");
                }

                printf("FROM CHILD 2222%s", sargv);
                execvp(sargv[0], sargv);
                perror("Fucked up\n");
                exit(-1);
            default:
                read(fd[0], &printThis, sizeof(printThis));
                printf("FROM PARENT AFTER READ %s", printThis);
                //while(buf[x++] != NULL)
                //{
                //    buf[x] = strtok(printThis, " ");
                //}
                //
                printThis[100] = NULL;
                write(STDOUT_FILENO, &printThis, sizeof(printThis));
                //printf("%s",buf);
                //write(STDOUT_FILENO, &buf, sizeof(buf));
                wait(NULL);
                close(fd[1]);
                close(fd[0]);

                break;
        }
        printf("\n==>> ");
        gets(buf);
    }

    return 0;
}
