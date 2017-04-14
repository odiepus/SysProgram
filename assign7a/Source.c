#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>




int main(void)
{
	struct mesg {
		char string[100];
		int id;
	}sendMesg, receiveMesg;

	int id = 0;
	int i = 0;
	int children = 10;
	const int writeCount = children + 2;
	int fd[10][2];


	int loopThisTimes = children - 1;

	int m = 0;
	for (; m < children; m++)
	{
		if (pipe(fd[m]) == -1)
		{
			perror("Pipe Broke");
			exit(-1);
		}
	}

	int j = 0;
	for (; j < children - 1; j++)
	{
		switch (fork())
		{
		case -1:
			perror("Fork Broke");
			exit(-1);

		case 0:
			sleep(3);
			id = ++i;
			
			j = 100;
			break;
		default:
			if (id == 0 && i == 0)
			{
				
				i++;
				loopThisTimes--;
			}
			else
			{
				i++;
			}
		}
	}

	strcpy(sendMesg.string, "process");
	sendMesg.id = id;

	//rng fd numbers to write to
	srand(5972261 * id);
	int rng = RAND_MAX / children;
	rng *= children;
	int n, e = 0, num = 0;

	int randoNums[12];
	for (n = 0; n < children + 2; n++)
	{
		do {
			num = rand();
			e = num % children;
			
		} while (num >= rng || id == e);
		randoNums[n] = e;
	}



	int k = 0;
	for (; k < n; k++)
	{
		if (write(fd[randoNums[k]][1], &sendMesg, sizeof(sendMesg)) == -1)
		{
			perror("write");
		}
	}

	int b = 0;
	for (; b < children; b++)
	{
		close(fd[b][1]);
	}



	char readBuf[256];
	int readSize = 0;
	while ((readSize = read(fd[id][0], &receiveMesg, sizeof(receiveMesg))) > 0)
	{
		printf("process%d has recieved a message from %s%d\n", id, receiveMesg.string, receiveMesg.id);
	}

	if (readSize == -1)
	{
		perror("read");
	}

	int l = 0;
	for (; l < children; l++)
	{
		close(fd[l][0]);
	}

	close(fd[id][0]);

	return 0;
}

