
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
		printf("숫자를 입력해주세요.\n");
		return -1;
	}

	int		option		= atoi(argv[1]);
	int 		intervalms 	= 1000;//atoi(argv[2]);
 	unsigned long 	num 		= atol(argv[2]);

	int intervalus = intervalms * 1000;

	printf("입력한 숫자는: %ld\n", num);

	if ( option == 0 )
	{
		num++;
		while(num--)
		{
			usleep(intervalus);
			printf("%ld\n", num);
		}
	}
	else
	{
		unsigned long i = 0;
		while(++i < num)
		{
			usleep(intervalus);
			printf("%ld\n", i);
		}
	}

	return 0;
}
