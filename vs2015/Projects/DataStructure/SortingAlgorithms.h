#pragma once

#ifndef _SORTING_ALGORITHMS_
#define _SORTING_ALGORITHMS_	1

#include "UsefulHeap.h"

void	BubbleSort		(int arArray[],	int	aSize);
void	SelectionSort	(int arArray[], int aSize);
void	InsertionSort	(int arArray[], int aSize);
void	HeapSort		(int arArray[], int aSize, PriorityComp pc);
void	MergeSort		(int arArray[], int aSize);
void	QuickSort		(int arArray[], int aSize);
void	RadixSort		(int arArray[], int aSize);

#endif
