
#include "Common.h"

#include "QuickSort.h"

void Swap(int arr[], int left, int right)
{
	int temp;
	temp		= arr[left];
	arr[left]	= arr[right];
	arr[right]	= temp;
}

int Partition(int arr[], int left, int right)
{
	int pivot	= arr[left];
	int	low		= left + 1;
	int high	= right;

	while (low <= high)								// until 'low' becomes higher than 'high'.
	{
		while(pivot >= arr[low] && low <= right)
			low++;

		while(pivot <= arr[high] && high >= (left + 1))
			high--;

		if(low <= high)
			Swap(arr, low, high);
	}

	Swap(arr, left, high);

	return high;
}

void QuickSortCore(int arr[], int left, int right)
{
	if (left > right)
		return;

	int pivot = Partition(arr, left, right);
	
	QuickSortCore(arr, left, pivot-1);
	QuickSortCore(arr, pivot+1, right);
}
