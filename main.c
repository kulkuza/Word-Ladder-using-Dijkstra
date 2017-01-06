/*main.c*/

//
// Word Ladders using Graphs and Dijkstra's Algorithm
//
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

#include "avl.h"
#include "queue.h"
#include "graph.h"
#include "mymem.h"
#include "timer.h"

#include "dijkstra-graph.h"


//
// Read_and_AddWords:
//
Graph *Read_and_AddWords(char *filename)
{
  FILE  *input;
  char   line[256];
  int    linesize = sizeof(line) / sizeof(line[0]);

  input = fopen(filename, "r");
  if (input == NULL)
  {
    printf("**ERROR: '%s' not found\n\n", filename);
    exit(-1);
  }

  //
  // (1) input words and insert each word as a vertex:
  //
  printf(">>Building Graph from '%s'...\n", filename);

  Graph *G = CreateGraph(256);  // 256 => initial size:

  fgets(line, linesize, input);

  while (!feof(input))
  {
    line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

    if (AddVertex(G, line) < 0)
    {
      printf("**Error: AddVertex failed?!\n\n");
      exit(-1);
    }

    fgets(line, linesize, input);
  }

  //
  // done:
  //
  fclose(input);

  return G;
}


//
// AddEdges:
//
void AddEdges(Graph *G)
{
  //
  // (2) Now for each word, let's generate all possible
  // words that differ by one letter, and add edges to/from
  // these words in the graph:
  //
  int  v;

  for (v = 0; v < G->NumVertices; ++v)
  {
    char *word = G->Names[v];

    char *temp = (char *)mymalloc(((int)(strlen(word) + 1)) * sizeof(char));

    int  i;
    for (i = 0; i < (int)strlen(word); ++i)
    {
      strcpy(temp, word);

      char  c = 'a';
      while (c <= 'z')
      {
        temp[i] = c;  // change one letter:

        int v2 = Name2Vertex(G, temp);
        if (v2 >= 0 && v2 != v)  // dest exists, add edge:
        {
          if (!AddEdge(G, v, v2, 1))
          {
            printf("**Error: AddEdge failed?!\n\n");
            exit(-1);
          }
        }//if

        ++c;
      }
    }

    myfree(temp);
  }
}


//
// PrintNeighborsAndBFS:
//
void PrintNeighborsAndBFS(Graph *G, int v)
{
  char line[256];
  int  linesize = sizeof(line) / sizeof(line[0]);

  Vertex *V = Neighbors(G, v);

  //
  // Neighbors:
  //
  printf("** Neighbors:\n");
  printf("   ");

  int  i = 0;
  while (V[i] != -1)  // ends with -1:
  {
    Vertex v = V[i];

    printf("(%d,%s) ", v, Vertex2Name(G, v));
    ++i;
  }

  printf("\n");

  myfree(V);

  //
  // Now call BFS with a distance and output those results:
  //
  int distance;
  printf(">> enter a BFS distance: ");

  scanf("%d", &distance);
  fgets(line, linesize, stdin);  // discard rest of line:

  V = BFSd(G, v, distance);

  //
  // BFSd returns vertices separated by "markers" of -1
  // between each step of the BFS.  Given a distance d, there
  // will be d+1 markers in the returned array.  Example:
  //
  //    0, -1, 1, 2, 3, 5, -1, ...
  //
  printf("** BFS:\n");

  int  d;
  i = 0;

  for (d = 0; d <= distance; ++d)  // d+1 markers:
  {
    printf("   distance %d: ", d);

    while (V[i] != -1)  // loop until next marker:
    {
      Vertex v = V[i];

      printf("(%d,%s) ", v, Vertex2Name(G, v));
      ++i;
    }

    ++i;  // skip over the -1:

    printf("\n");
  }

  myfree(V);
}

void PrintDijkstra(Graph *G, Vertex src, Vertex dest)
{
	timer_start();

	Vertex *dijkstra = Dijkstra(G, src, dest);

	if (dijkstra[0] < 0)
	{
		char *word1 = Vertex2Name(G, src);
		char *word2 = Vertex2Name(G, dest);
		printf("** There is no word from '%s' to '%s'.\n", word1, word2);
		timer_stop();
	}
	else
	{
		printf("** Shortest word ladder:\n");
		int i;
		int length = -1;
		for (i = 0; dijkstra[i] != -1; i++)
		{
			char *word = Vertex2Name(G, dijkstra[i]);
			printf("   %s\n", word);
			length++;
		}

		printf("   Length: %d\n", length);

		timer_stop();
		timer_stats("   Time:   ");
	}

	myfree(dijkstra);


}


//
// main:
//
int main()
{
  Graph *G;
  char  *filename = "merriam-webster.txt";
  char   line[256];
  int    linesize = sizeof(line) / sizeof(line[0]);

  printf("** Starting Word Ladder App **\n\n");

  //
  // (1) input words and insert each word as a vertex:
  //
  timer_start();

  G = Read_and_AddWords(filename);

  //
  // (2) Now for each word, let's generate all possible
  // words that differ by one letter, and add edges to/from
  // these words in the graph:
  //
  AddEdges(G);

  //
  // (3) print some graph stats:
  //
  PrintGraph(G, "Word Ladder", 0 /*false*/);

  timer_stop();
  timer_stats(">>Build time:    ");

  printf("\n");

  //
  // (4) input words from the user and perform BFS:
  //
  printf(">> enter a word (ENTER to quit): ");

  fgets(line, linesize, stdin);
  line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  while (strlen(line) > 0)
  {
    int v = Name2Vertex(G, line);

    printf(">> enter another word: ");

    fgets(line, linesize, stdin);
  	line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  	int v2 = Name2Vertex(G, line);

    if (v < 0)
    {
      	printf("Word1 not found, please try again...\n");
    }
    else if (v2 < 0)
    {
    	printf("Word2 not found, please try again...\n");
    }
    else
    {
      	//PrintNeighborsAndBFS(G, v);
      	PrintDijkstra(G, v, v2);
    }

    printf("\n");
    printf(">> enter a word (ENTER to quit): ");

    fgets(line, linesize, stdin);
    line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:
  }

  //
  // done:
  //
  DeleteGraph(G);
  FreeAVLTree(G->NamesTree);

  printf("\n** Done **\n");
  mymem_stats();

  printf("\n");

  return 0;
}
