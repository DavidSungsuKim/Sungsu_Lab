
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
	printf("안녕하세요. 김다엘군!\n");

	if (argc < 3 )
	{
		printf("Example : ./bin [option] [number]\n");
		printf("[option]\n");
		printf("	0 : down counting\n");
		printf("	1 : up counting\n");
		printf("[number]\n");
		printf("	number to start from or reach to\n");
		return -1;
	}

	int		option		= atoi(argv[1]);
	int 		intervalms 	= 1000;//atoi(argv[2]);
 	unsigned long 	num 		= atol(argv[2]);

	int intervalus = intervalms * 1000;

	printf("*********************\n");
	printf("Number : %ld\n", num);
	printf("Option : %s\n", option == 0 ? "counting down" : "counting up");
	printf("Time Expected : %.3f[sec]\n", (double)num / (double)intervalms * 1000. );
	printf("*********************\n");


	// counting down
	if ( option == 0 )
	{
		num++;
		while(num--)
		{
			usleep(intervalus);
			printf("%ld\n", num);
		}
	}
	// counting up
	else
	{
		unsigned long i = 0;
		while(++i <= num)
		{
			usleep(intervalus);
			printf("%ld\n", i);
		}
	}

	return 0;
}
