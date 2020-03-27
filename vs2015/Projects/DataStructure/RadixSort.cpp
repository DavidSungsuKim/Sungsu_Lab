
#include "Common.h"

#include "ListBasedQueue.h"
#include "RadixSort.h"

#define BUCKET_NUM	10

void RadixSortCore(int arr[], int num, int maxLen)
{
	Queue	buckets[BUCKET_NUM];						// the reason we have Queue is because there may be the same digits in arr.
	int		bi;
	int		pos;
	int		di;
	int		divfac	= 1;
	int		radix;

	for (bi = 0; bi < BUCKET_NUM; bi++)					// initialize all queues.
		InitQueue(&buckets[bi]);

	for (pos = 0; pos < maxLen; pos++)
	{
		for (di = 0; di < num; di++)
		{
			radix = (arr[di] / divfac) % 10;			// start from LSD, and get the room number to be stored in.

			Enqueue(&buckets[radix], arr[di]);			// insert the number in that room.
		}

		for (bi = 0, di = 0; bi < BUCKET_NUM; bi++)
		{
			while(!IsQEmpty(&buckets[bi]))
				arr[di++] = Dequeue(&buckets[bi]);
		}

		divfac *= 10;
	}
}