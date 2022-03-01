
#include "Common.h"

#include "ALGraph.h"
#include "ArrayBasedStack.h"
#include "LinkedList.h"
#include "CircularQueue.h"

int	VisitVertex(ALGraph* pg, int visitV);

int	WhoIsProceed(LData data1, LData data2)
{
	if(data1.posX > data2.posX)
		return 1;
	else
		return 0;
}

int	PQWeightCom(Edge d1, Edge d2)
{
	return d1.weight - d2.weight;
}

void GraphInit(ALGraph* pg, int nv)
{
	int		i;

	pg->adjList = (List*)malloc(sizeof(List)*nv);
	pg->numV = nv;
	pg->numE = 0;

	for (i = 0; i < nv; i++)
	{
		List*	pList = &(pg->adjList[i]);

		ListInit(pList);
		SetSortRule(pList, WhoIsProceed);
	}

	pg->visitInfo = (int*)malloc(sizeof(int)*(pg->numV));
	memset(pg->visitInfo, 0, sizeof(int)*(pg->numV));

	PQueueInit(&(pg->pqueue), PQWeightCom);
}

void GraphDestroy(ALGraph* pg)
{
	if (pg->adjList != NULL)
		free(pg->adjList);										// don't we need to care about its child nodes in list?

	if (pg->visitInfo != NULL)
		free(pg->visitInfo);
}

void AddEdge(ALGraph* pg, int fromV, int toV, char weight)
{
	Edge	edge = {fromV, toV, weight};
	LData	data = {NULL, NULL};
	List*	pList;

	if ((fromV >= pg->numV) || (toV >= pg->numV))
	{
		exit(-2);
	}

	pList = pg->adjList;

	data.posX = toV;											// we use only posX of data not to change LData's structure.
	SInsert(&(pList[fromV]), data);								// we use SInsert instead of LInsert to get sorted data in the list.

	data.posX = fromV;
	SInsert(&(pList[toV]), data);

	pg->numE += 1;

	PEnqueue(&(pg->pqueue), edge);								// whenever we add an edge, it is contained in the queue according to the weight.
}

void RemoveWayEdge(ALGraph* pg, int fromV, int toV)
{
	LData	data = { 0, NULL };
	int*	edge = &(data.posX);

	if (LFirst(&(pg->adjList[fromV]), &data))					// from the list belonged to 'fromV' it searches for an edge to 'toV'.
	{
		if (*edge == toV)
		{
			LRemove(&(pg->adjList[fromV]));
			return;
		}

		while (LNext(&(pg->adjList[fromV]), &data))
		{
			if (*edge == toV)
			{
				LRemove(&(pg->adjList[fromV]));
				return;
			}
		}
	}
}

void RemoveEdge(ALGraph* pg, int fromV, int toV)
{
	RemoveWayEdge(pg, fromV, toV);								// because ours is a bidirectional graph, we have to remove edges of both direction.
	RemoveWayEdge(pg, toV, fromV);

	(pg->numE)--;												// and then we dicrease by 1.
}

int IsConnVertex(ALGraph* pg, int v1, int v2)					// this function aims to mearly check whether or not vertexes are connected each other.
{
/*	This function first starts with two vertexes that we have to confirm they're connected via any of vertexes in the graph.
	Even though v1 isn't directly connected to v2, if v2 is reached by v1 through other edges and vertexes, 
	we can conclude so.
	*/

	Stack	stack;
	int		visitV	= v1;
	int		visitFlag;

	LData	lnextV	= {0, NULL};
	int*	nextV	= &(lnextV.posX);

	StackInit(&stack);											// the purpose of having a stack is to trace vertexes we pass through.
	VisitVertex(pg, visitV);
	SPush(&stack, visitV);

	while (LFirst(&(pg->adjList[visitV]), &lnextV) == TRUE)		// if LFirst and LNext's return is TRUE, it means visitV and lnextV are connected each other.
	{
		if (*nextV == v2)										// if this is v2, we can finish right away.
		{
			memset(pg->visitInfo, 0, (sizeof(int) * pg->numV));
			return TRUE;										// connected.
		}

		if (VisitVertex(pg, *nextV) == TRUE)					// if nextV hadn't ever been visited
		{
			SPush(&stack, visitV);
			visitV		= *nextV;								// , change visitV to make it the next searching vertex.

			continue;
		}

		visitFlag = FALSE;

		while (LNext(&(pg->adjList[visitV]), &lnextV) == TRUE)
		{
			if (*nextV == v2)
			{
				memset(pg->visitInfo, 0, (sizeof(int) * pg->numV));
				return TRUE;
			}

			if (VisitVertex(pg, *nextV) == TRUE)				// if this is not TRUE, it means that the vertex hasn't ever been visited.
			{
				SPush(&stack, visitV);					
				visitV		= *nextV;

				visitFlag	= TRUE;

				break;
			}
		}
		
		if (visitFlag == FALSE)									// even though v2 is not found connected at a vertex, 
		{
			if(SIsEmpty(&stack) == TRUE)						// if there is no vertexes in the stack, we declare v1 is not reached : not connected.
				break;
			else
				visitV = SPop(&stack);							// if stack is not empty, it is too early to lose hope for searching connection.
		}
	}

	memset(pg->visitInfo, 0, (sizeof(int) * pg->numV));
	
	return FALSE;
}

void RecoverEdge(ALGraph* pg, int fromV, int toV, int weight)		// this functions exactly as the oppostite to RemoveEdge().
{
	LData	data = {0, NULL};

	data.posX = toV;
	LInsert(&(pg->adjList[fromV]), data);

	data.posX = fromV;
	LInsert(&(pg->adjList[toV]), data);

	(pg->numE)++;
}

void ShowGraphEdge(ALGraph* pg)
{
	int		i;
	LData	vx;
	List*	pList;

	pList = pg->adjList;

	for (i = 0; i < pg->numV; i++)
	{
		printf("Vertex that connects %c is ", DEC_CHAR(i));

		if (LFirst(&(pList[i]), &vx))
		{
			printf("%c ", DEC_CHAR(vx.posX));

			while (LNext(&(pList[i]), &vx))
				printf("%c ", DEC_CHAR(vx.posX));
		}

		printf("\n");
	}
}

int	VisitVertex(ALGraph* pg, int visitV)
{
	if (pg->visitInfo[visitV] == 0)
	{
		pg->visitInfo[visitV] = 1;
		printf("%c ", DEC_CHAR(visitV));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void ShowDFSGraphVertex(ALGraph* pg, int startV)
{
	Stack	stack;

	List*	pList;
	LData	nextV;
	LData	visitV;

	visitV	= {startV,	NULL};								// we don't use posY in both of them.
	nextV	= {0,		NULL};

	StackInit(&stack);
	VisitVertex(pg, visitV.posX);
	SPush(&stack, visitV.posX);

	pList = pg->adjList;

	while (LFirst(&(pList[visitV.posX]), &nextV) == TRUE)
	{
		int	visitFlag = FALSE;

		if (VisitVertex(pg, nextV.posX) == TRUE)
		{
			SPush(&stack, visitV.posX);

			visitV.posX	= nextV.posX;
			visitFlag	= TRUE;
		}
		else
		{
			while (LNext(&(pList[visitV.posX]), &nextV) == TRUE)
			{
				if (VisitVertex(pg, nextV.posX) == TRUE)
				{
					SPush(&stack, visitV.posX);

					visitV.posX = nextV.posX;					// vertex is changed so we can trace.
					visitFlag	= TRUE;
					break;
				}
			}
		}

		if (visitFlag == FALSE)
		{
			if(SIsEmpty(&stack) == TRUE)
				break;
			else
				visitV.posX = (int)SPop(&stack);
		}
	}	

	memset(pg->visitInfo, 0, sizeof(int)*(pg->numV));
}

void ShowBFSGraphVertex(ALGraph* pg, int startV)
{
	Queue	queue;

	LData	dvisitV		= {startV,	NULL};
	LData	dnextV		= {NULL,	NULL};

	List*	pList		= pg->adjList;

	QueueInit(&queue);

	VisitVertex(pg, dvisitV.posX);								// this VisitVertex is to trace vertexes that we have passed through

	while (LFirst(&(pList[dvisitV.posX]), &dnextV) == TRUE)
	{
		if (VisitVertex(pg, dnextV.posX) == TRUE)				// we don't visit again that had been visited previously.
			Enqueue(&queue, dnextV.posX);

		while (LNext(&(pList[dvisitV.posX]), &dnextV) == TRUE)
		{
			if (VisitVertex(pg, dnextV.posX) == TRUE)
				Enqueue(&queue, dnextV.posX);
		}

		if(QIsEmpty(&queue) == TRUE)
			break;
		else
			dvisitV.posX = Dequeue(&queue);						// get the next vertex to visit
	}

	memset(pg->visitInfo, 0, sizeof(int) * pg->numV);
}

void ConKruskalMST(ALGraph* pg)
{
	Edge	recvEdge[20];
	Edge	edge;
	int		eidx	= 0;
	int		i;

	while ((pg->numE + 1) > pg->numV)							// try to remove edges with this finish condition : (numE + 1) == numV
	{
		edge = PDequeue(&(pg->pqueue));

		RemoveEdge(pg, edge.v1, edge.v2);

		if (!IsConnVertex(pg, edge.v1, edge.v2))
		{
			RecoverEdge(pg, edge.v1, edge.v2, edge.weight);		// why aren't we doing enqueue after recovering?
			recvEdge[eidx++] = edge;
		}
	}

	for(i = 0; i < eidx; i++)									// Q : why do we have to do this?
		PEnqueue(&(pg->pqueue), recvEdge[i]);					// A : it is not to repeat dequeuing. if I call this right after RecoverEdge(), while continues infinitely.
}

void ShowGraphEdgeWeightInfo(ALGraph* pg)
{
	PQueue	copyPQ = pg->pqueue;
	Edge	edge;

	while (!PQIsEmpty(&copyPQ))
	{
		edge = PDequeue(&copyPQ);
		printf("(%c-%c), w:%d \n", DEC_CHAR(edge.v1), DEC_CHAR(edge.v2), edge.weight);
	}
}