

/*
 * From test2 file tes2bb
 *
 *
 * given that we want to delete from rnum down to (rnum + num) I setup the
 * file token to start, then move the token down past the last record i want to
 * delete. I then read in from that point to the eof into the buffer. I reset my token
 * and plase it to the first record i want to delete at rnum. I rite till the buffer is empty.
 * Then knowing the size of the file, and how many records I want to delete: size - (num *rsz)
 * I truncate the file and close fd and i am done
 */

int delRecs(char *fdb, int rnum, int num, int rsz)
{
    int fdin;
    char buf[4096];
    int i = 0;
    int index = 2;
    int howManyToDelete = 3;
    int fd1 = open(fdb, O_RDWR);  //open file that was passed into us
    int x = 0, n = 0 ;



    //check if file opened successfully
    if(fd1 == -1)
    {
        perror("open");
        exit -1;
    }

    //get size of file we opened.
    int sizeOfFile = lseek(fdin, 0, SEEK_SET);

    //set our file token to index past the last rec we want to delete
    lseek(fdin, (index+howManyToDelete)*rsz, SEEK_CUR);

    //read in the contents of the file past where we want to delete and put in a buffer
    while((n = read(fdin, buf, rsz)) == rsz){
    }

    //check for read error
    if (n == -1) return -1;

    //set out file token to index of first record we want to delete
    lseek(fdin, index*rsz, SEEK_CUR);

    //write to file records in our buf. we overwrite files to delete
    while( (x = write(fdin, buf, rsz)) == rsz ){
    }

    //check if write errors
    if (n == -1) return -1;

    //math used to find out size of our new file with out the records we wanted and truncate
    ftruncate(fdin, sizeOfFile - ((howManyToDelete +1) * rsz));

    //close the fd we opened
    close(fdin);


    return 0;
}
