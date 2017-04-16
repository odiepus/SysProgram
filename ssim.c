#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(void)
{
  int i, j, k, l, m, n;           //counters for various loops
  int fd[10][2];                  //for the pipes
  int parent = 1;                 //to ensure children dont run the loop after their creation
  const int totalProcesses = 10;  //total process's to run. can  be changed here instead of in the code
  int randoNums[totalProcesses];  //to hold our randomly generated process Id's

  struct{               //struct to hold our pid's
    int processId;
  }myPid, theirPid;

  for(i = 0; i < totalProcesses; i++)  //loop to create the pipes
  {
    if(pipe(fd[i]) == -1)
    {
      perror("fork line 14");
      exit -1;
    }
  }

  for(j = 0; j < i; j++)              /loop to create the children
  {
    if( (parent = fork()) == -1)
    {
      perror("fork line 26");
      exit -1;
    }
    if (parent) break;
  }

  if(parent == 1)             //if parent then assign 0 to its pid
  {
    myPid.processId = 0;
  }
  else                      //else assign our children creator loop counter
  {
    myPid.processId = ++j;
  }

  srand(5972261*myPid.processId);   //we start the random pid generator

  int rng = RAND_MAX/totalProcesses;
  rng *= totalProcesses;

  for(k = 0; k < totalProcesses + 2; k++)         
  {
    do{
      l = rand();
      m = l%totalProcesses;
    }
    while(l >= rng || j == m);
    randoNums[k] = m;
  }

  for(n = 0; n < totalProcesses; n++)  //close my write and all reads that are not mine
  {
    if(n == myPid.processId)
    {
      close(fd[n][1]);
    }
    else{
      close(fd[n][0]);
    }
  }

  for(n = 0; n < totalProcesses + 2; n++)       //send my pid out to random pids
  {
    if(write(fd[randoNums[n]][1], &myPid, sizeof(myPid)) == -1)
    {
      perror("write line 75");
      exit -1;
    }
  }

  for(n = 0; n < totalProcesses; n++)       //close my writes to others
  {
    close(fd[n][1]);
  }

  while( (n = read(fd[myPid.processId][0], &theirPid, sizeof(theirPid))) == sizeof(theirPid))     //read incoming messages and print for the world to see
  {
    printf("process%d has received a message from process%d\n", myPid.processId, theirPid.processId);
  }

  if(n == -1)         //check if read faild
  {
    perror("write line 94");
    exit -1;
  }

  close(fd[myPid.processId][0]);  //close my last open fd
  return 0;
}
