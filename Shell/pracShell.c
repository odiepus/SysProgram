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

    int i = 0;
    int forc = 0;

    FILE *stream;

    if(pipe(fd) == -1)
    {
        perror("Failed to create pipe\n");
    }


    printf("\n==>> ");
    gets(buf);
    printf("%s", buf);
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

                sargv[0] = strtok(buf, " ");
                printf("sargv[0]\n");

                while(sargv[i++] != NULL)
                {
                    sargv[i] = strtok(NULL, " ");
                }

                execvp(sargv[0], sargv);
                perror("Fucked up\n");
                exit(-1);
            default:
                wait(NULL);
                char printThis[4096];
                read(fd[0], &printThis, sizeof(printThis));

                write(STDOUT_FILENO, &printThis, sizeof(printThis));
                close(fd[1]);
                close(fd[0]);
                fclose(stream);

                break;
        }
        printf("\n==>> ");
        gets(buf);
    }

    return 0;
}
