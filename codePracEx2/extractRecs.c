#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<error.h>

int main(void)
{
    int num1, i = 0, moveThisMany,i rsz,  number = 3, fd1, fd2, howManyRecs;

    if( (fd1 = open(fdb, O_RDWR) == -1 ){
        perror("open");
        return -1;
        }

    if( (fd2 = open(fdout, O_CREAT | O_WRONLY | O_APPEND, 0600)) == -1 )
    {
        perror("open");
        return -1;
    }

    int fileSize = lseek(fd1, 0, SEEK_END);

    howManyRecs = fileSize / rsz;

    lseek(fd1, num1 * rsz, SEEK_CUR);

    for(; i < moveThisMany; i++)
    {
        if( read(fd1, buf, rsz) == -1 )
        {
            perror("read");
            return -1;
        }
    }

    lseek(fd2, 0, SEEK_SET);
    i = 0;

    for(; i < moveThisMany; i++)
    {
        if(write(fd2, buf, rsz) == -1)
        {
            perror("write");
            return -1;
        }
    }

    truncate(fd2, fileSize - moveThisMany*rsz);


    return 0;
}
