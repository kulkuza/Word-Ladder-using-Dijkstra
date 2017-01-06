/* djikstra-graph.c */

// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#include "avl.h"
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "graph.h"
#include "mymem.h"

#include "dijkstra-graph.h"

//
// Performs Dijkstra's shortest path algorithm to find the shortest path
// from src to dest.  Returns a dynamically-allocated array of vertices
// denoting this path; the array will start with src, contain 0 or more
// vertices that lead to dest, followed by dest, and ending with -1.  
// If there is no pat from src to dest, the array will contain only -1.
//
// NOTE: returns NULL if src or dest are not valid vertex ids.
//
// NOTE: it is the responsibility of the CALLER to free the 
// returned array when they are done.
//
int PopMin(Queue *unvisitedQ, int distance[])
{
  int    N = unvisitedQ->NumElements;
  Stack *S = CreateStack(N); 

  assert(!isEmptyQueue(unvisitedQ));

  //
  // search for smallest vertex distance in the queue,
  // saving other elements in a stack so we can put back
  // later:
  //
  int minV = Dequeue(unvisitedQ);  // assume first vertex is the min:

  while (!isEmptyQueue(unvisitedQ))  // look for smaller vertex:
  {
    int v = Dequeue(unvisitedQ);

    if (distance[v] < distance[minV])
    {
      Push(S, minV);  // save so we can put back in queue:
      minV = v;  // our new min:
    }
    else  // not smaller, so save to enqueue later:
    {
      Push(S, v);
    }
  }

  //
  // empty the stack back into the queue:
  //
  while (!isEmptyStack(S))
  {
    Enqueue(unvisitedQ, Pop(S));
  }

  // NOTE: at this point queue is now one smaller, with
  // min element removed.  Let's confirm:
  assert(unvisitedQ->NumElements == (N - 1));

  //
  // done:
  // 
  DeleteStack(S);

  return minV;
}


Vertex *Dijkstra(Graph *G, Vertex src, Vertex dest)
{
  int  INF = INT_MAX;

  if (src < 0 || src >= G->NumVertices)  // invalid vertex #:
    return NULL;
  if (dest < 0 || dest >= G->NumVertices)  // invalid vertex #:
    return NULL;

  //
  // allocate distances array:
  //
  int N = G->NumVertices;

  int *distance = (int *)mymalloc(N * sizeof(int));
  if (distance == NULL)
  {
    printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
    exit(-1);
  }

  Vertex *predecessor = (Vertex *)mymalloc(N * sizeof(Vertex));
  if (predecessor == NULL)
  {
    printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
    exit(-1);
  }

  //
  // for each vertex, add to Queue to be visited,
  // initialize distance to Infinity, and set predecessor
  // to -1:
  //
  Queue *unvisitedQ = CreateQueue(N);
  int currentV;

  for (currentV = 0; currentV < N; ++currentV)
  {
    Enqueue(unvisitedQ, currentV);
    distance[currentV] = INF; 
    predecessor[currentV] = -1;
  }

  //
  // starting vertex has a distance of 0 from itself:
  //
  distance[src] = 0;

  //
  // Now run Dijkstra's algorithm:
  //
  while (!isEmptyQueue(unvisitedQ))
  {
    //
    // find the vertex with the smallest distance from
    // the start, that's the vertex to explore next:
    //
    currentV = PopMin(unvisitedQ, distance);

    // is there a path to the current vertex?  if not, 
    // there's no point in looking at neighbors...
    if (distance[currentV] == INF)
      break;

    //
    // now see if we have found any shorter paths for minV's
    // neighboring vertices:
    //
    Vertex *neighbors = Neighbors(G, currentV);

    int i = 0;
    while (neighbors[i] != -1)  // for each neighbor:
    {
      int adjV = neighbors[i];

      int edgeWeight = getEdgeWeight(G, currentV, adjV);
      int altDistance = distance[currentV] + edgeWeight;

      if (altDistance < distance[adjV])
      {
        distance[adjV] = altDistance;
        predecessor[adjV] = currentV;
      }

      ++i;
    }

    myfree(neighbors);
  }

  //
  // Okay, algorithm has run to completion, and the path (if
  // any) is stored backwards in predecessor array.  So let's 
  // use a stack to reverse it:
  //
  Stack *S = CreateStack(N);

  int v = dest;
  while (predecessor[v] != -1)
  {
    Push(S, v);
    v = predecessor[v];
  }

  // loop stops when it gets to src, so push src to finish:
  Push(S, src);

  //
  // at this point we have the path on the stack, or if there's
  // no path, just src is on the stack.  Allocated an array and
  // fill with path, or if no path, just -1:
  //
  Vertex  *path;

  if (S->NumElements == 1) // just src on stack, no path:
  {
    N = 1;  // just the -1:

    path = (Vertex *)mymalloc(N * sizeof(Vertex));
    if (path == NULL)
    {
      printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
      exit(-1);
    }

    path[0] = -1;  // no path from src to dest:
  }
  else
  {
    N = S->NumElements + 1;  // path + -1 at the end

    path = (Vertex *)mymalloc(N * sizeof(Vertex));
    if (path == NULL)
    {
      printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
      exit(-1);
    }

    // now empty the stack into the path array:
    int i = 0;
    while (!isEmptyStack(S))
    {
      path[i] = Pop(S);
      ++i;
    }

    path[i] = -1;  // need -1 terminator at the end:
  }

  //
  // done!
  //
  DeleteStack(S);
  DeleteQueue(unvisitedQ);
  myfree(distance); 
  myfree(predecessor);

  return path;
}
