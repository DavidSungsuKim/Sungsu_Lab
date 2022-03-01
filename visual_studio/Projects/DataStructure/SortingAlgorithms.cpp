
#include "Common.h"

#include "SortingAlgorithms.h"
#include "QuickSort.h"
#include "RadixSort.h"

void BubbleSort(int arArray[], int	aSize)
{
	int		i, j;
	int		temp;

	int		iEnd;
	int		jEnd;
	int*	pData;

	pData	= arArray;

	iEnd	= aSize - 1;
	jEnd	= iEnd	- 1;

	for (i = 0; i < iEnd; i++)
	{
		for (j = 0; j < jEnd; j++)
		{
			if (pData[j] > pData[j + 1])	// O(n^2) for the comparing operation
			{
				temp		= pData[j];		// O(n^2) for the moving operation	
				pData[j]	= pData[j+1];
				pData[j+1]	= temp;
			}
		}
	}
}

void SelectionSort(int arArray[], int aSize)
{
	int		i, j, iEnd, jEnd;
	int		maxIdx;
	int		temp;

	int*	pData;

	pData	= arArray;

	iEnd	= aSize - 1;
	jEnd	= iEnd	- 1;

	for (i = 0; i < iEnd; i++)
	{
		maxIdx	= i;
		for (j = i+1; j < aSize; j++)		// searching for the minimum value in the array.
		{
			if (pData[j] < pData[maxIdx])	// O(n^2) for the comparing operation
			{
				maxIdx	= j;
			}
		}

		temp			= pData[i];			// O(n) for the moving operation
		pData[i]		= pData[maxIdx];
		pData[maxIdx]	= temp;
	}
}

void InsertionSort(int arArray[], int aSize)
{
/*	Though total length of this function is not long, 
	it's not that simple for me to imagine how it works.
	So, make a note on a paper to better understand this.	*/
	int		i, j;
	int		insData;

	int*	pData;

	pData	= arArray;

	for (i = 1; i < aSize; i++)
	{
		insData = pData[i];					// get the data to be sorted.

		for (j = i-1; j >= 0; j--)			// search the minimum value prior to the data.
		{			
			if (pData[j] > insData)			// O(n^2) for the comparing operation at the worst case.
			{
				pData[j+1] = pData[j];		// it's a shifting operation.
			}
			else
				break;						// it's already in order.
		}

		pData[j+1] = insData;				// j+1 is the final destination. 
	}
}

void HeapSort(int arArray[], int aSize, PriorityComp pc)
{
/*	Notice) Before UHeapXX functions are used, we need to apply this:
	
	before:	typedef char	HData;
	after:	typedef int		HData;

	*/
	Heap	heap;
	int		i;
	int*	pData;
	HData	data = {0, NULL, NULL};		// we are going to use only v1 of HData(Edge).

	pData	= arArray;

	UHeapInit(&heap, pc);				// initialize the heap.

	for (i = 0; i < aSize; i++)
	{
		data.v1	= pData[i];

		UHInsert(&heap, data);			// configure heap adding data at the same time.
	}

	for (i = 0; i < aSize; i++)
	{
		data = UHDelete(&heap);			// now we can make sure that data is ordered by the fuction 'pc'.
		pData[i] = data.v1;
	}
}

void MergeTwoArea(int arr[], int left, int mid, int right)
{
	int fIdx	= left;
	int sIdx	= left;
	int rIdx	= mid + 1;	
	int	i;

	int*	sortArr	= (int*)malloc(sizeof(int)*(right+1));	// why should the size be like this? To apply just the buffer size.

	while (fIdx <= mid && rIdx <= right)
	{
		if (arr[fIdx] <= arr[rIdx])							// compare two areas by each of data
		{
			sortArr[sIdx] = arr[fIdx++];
		}
		else
		{
			sortArr[sIdx] = arr[rIdx++];
		}

		sIdx++;												// With increasing the index by one, 
	}

	if (fIdx > mid)											// filling out tails!! ok.
	{
		for (i = rIdx; i <= right; i++, sIdx++)
		{
			sortArr[sIdx] = arr[i];
		}
	}
	else
	{
		for (i = fIdx; i <= mid; i++, sIdx++)
		{
			sortArr[sIdx] = arr[i];
		}
	}

	for (i = left; i <= right; i++)
	{
		arr[i] = sortArr[i];
	}

	free(sortArr);
}

int g_dbgMergeSort;

void MergeSortCore(int arArray[], int left, int right)
{
	int mid;

	g_dbgMergeSort++;
//	printf("recursive merge=%d,	left=%d, right=%d\n", g_dbgMergeSort, left, right);

	if (left < right)
	{
		mid = (left + right) / 2;

		MergeSortCore(arArray, left, mid);
		MergeSortCore(arArray, mid+1, right);

		MergeTwoArea(arArray, left, mid, right);
	}
	// if it cannot split the array anymore, returns. (left >= right)

//	printf("recursive merge=%d,	left=%d, right=%d\n", g_dbgMergeSort, left, right);
}

void MergeSort(int arArray[], int aSize)
{
	int left;
	int right;

	left	= 0;
	right	= aSize - 1;
	
	g_dbgMergeSort = 0;

	MergeSortCore(arArray, left, right);
	
	return;
}

void QuickSort(int arArray[], int aSize)
{	
	int left	= 0;
	int right	= aSize - 1;

	QuickSortCore(arArray, left, right);
}

void RadixSort(int arArray[], int aSize)
{
	int i, max, maxLen;

	max = 0;
	for (i = 0; i < aSize; i++)
	{
		if ( max < abs(arArray[i]) )
			max = arArray[i];
	}

	maxLen = 0;
	while (max)
	{
		max = max / 10;
		maxLen++;
	}

	RadixSortCore(arArray, aSize, maxLen);
}