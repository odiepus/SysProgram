#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



int main(void)
{
	int id = 0;
	int i = 0;
	 int children = 1;
	const int writeCount = children + 2;
	int fd[10][2];

	int loopThisTimes = children;

	int j = 0;
	for (; j < children; j++)
	{
		if (pipe(fd[j]) == -1)
		{
			perror("Pipe Broke");
			exit(-1);
		}
	}

	while (loopThisTimes != 0)
	{
		switch (fork())
		{
		case -1:
			perror("Fork Broke");
			exit(-1);

		case 0:
			id = ++i;
			break;
		default:
			if (id == 0 && i == 0)
			{
				i++;
				loopThisTimes--;
			}
			else if (id != 0)
			{
				loopThisTimes = 0;
			}
			else
			{
				i++;
				loopThisTimes--;
			}
		}
	}

	close(fd[id][1]); //close this process's write fd to itself

					  //close all of this process's read pipes excluding its indexed [id][0] reader
	int h = 0;
	for (; h < writeCount; h++)
	{
		if (id != i)
		{
			close(fd[id][0]);
		}
	}
	
	char mesg[] = { 'p', 'r', 'o', 'c', 'e', 's', 's', '0' };
	int len = strlen(mesg);

	if (id != 0)
	{
		mesg[len - 1] = id;
	}

	//rng fd numbers to write to
	srand(5972261 * i);
	int rng = RAND_MAX / children;
	rng *= children;
	int n, e = 0, num = 0;

	int randoNums[12];
	for (n = 0; n < children + 2; n++)
	{
		do {
			num = rand();
			e = num % 10;
			randoNums[n] = e;
		} while (num >= rng || id == e);
	}

	int k = 0;
	for (; k < writeCount; k++)
	{
		if (k != id)
		{
			if (randoNums[n] == k)
			{
				if (write(fd[k][1], &mesg, sizeof(mesg)) == -1)
				{
					perror("write");
				}
			}
		}
	}

	char readBuf[256];
	int readSize = 1;
	while (readSize != -1)
	{
		readSize = read(fd[id][0], &readBuf, sizeof(readBuf));
		printf("process%d has recieved a message from %s\n", id, readBuf);
	}









	return 0;
}

