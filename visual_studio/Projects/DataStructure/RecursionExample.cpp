
#include "Common.h"

#include "RecursionExample.h"

void Recursive(int num)
{
	if(num <= 0)
		return;

	printf("Recursive call! n=%d\n", num);

	Recursive(num-1);
}

int	Factorial(int n)
{
	if(n <= 0)
		return 1;

	return n*Factorial(n-1);
}

int	FibonacciSeq(int n)
{
	int sum;

	if(n == 1)
	{
		sum = 0;
	}
	else if (n == 2)
	{
		sum = 1;
	}
	else
	{	
		sum = FibonacciSeq(n - 1) + FibonacciSeq(n - 2);
	}

//	printf("%d ", sum);			//	printing here will almost make you confused, 
								//	so you'd better not to do that in understanding the aspect of how a recursion function works.

	return (sum);
}

int	BinarySearchCore(int ar[], int first, int last, int target)
{
	int mid;
	if (first > last)
		return -1;

	mid = (first + last) / 2;

	if (ar[mid] == target)
		return mid;
	else if (target < ar[mid])
		return BinarySearchCore(ar, first, (mid - 1), target);
	else
		return BinarySearchCore(ar, (mid + 1), last, target);
}

int	BinarySearch(int ar[], int first, int last, int target)
{
	return BinarySearchCore(ar, first, last, target);
}

int	InterpolationSearchCore(int ar[], int first, int last, int target)
{
	int mid;
	if (ar[first] > target || ar[last] < target)		// For Interpolation Search, we should rather use this. Otherwise, this may cause stack overflow.
//	if (first > last)
		return -1;

	mid = ((double)(target-ar[first]) / (ar[last]-ar[first]) * (last-first)) + first;

	if (ar[mid] == target)
		return mid;
	else if (target < ar[mid])
		return InterpolationSearchCore(ar, first, (mid - 1), target);
	else
		return InterpolationSearchCore(ar, (mid + 1), last, target);
}

int	InterpolationSearch(int ar[], int first, int last, int target)
{
	return InterpolationSearchCore(ar, first, last, target);
}

void HanoiTowerCore(int num, char from, char by, char to)
{
	if (num == 1)
	{
		printf("Move 1	--> %c\n", to);
	}
	else
	{
		HanoiTowerCore(num-1, from, to, by);			// move 'n-1' to the second point.
		printf("Move %d	--> %c\n", num, to);
		HanoiTowerCore(num-1, by, from, to);			// move 'n-1' to the third point (finish).
	}
}

void HanoiTower(int height)
{
	HanoiTowerCore(height, 'A', 'B', 'C');
}