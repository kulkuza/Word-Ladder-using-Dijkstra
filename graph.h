/*graph.h*/

// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

//
// Graph:
//
typedef int Vertex;

typedef struct Edge
{
  Vertex  src;
  Vertex  dest;
  int     weight;
  struct Edge *next;
} Edge;

typedef struct Graph
{
  Edge    **Vertices;
  char    **Names;
  AVLNode  *NamesTree;
  int       NumVertices;
  int       NumEdges;
  int       Capacity;
} Graph;

Graph  *CreateGraph(int N);
void    DeleteGraph(Graph *G);
int     AddVertex(Graph *G, char *name);
int     Name2Vertex(Graph *G, char *Name);
char   *Vertex2Name(Graph *G, Vertex v);
int     AddEdge(Graph *G, Vertex src, Vertex dest, int weight);

Vertex *Neighbors(Graph *G, Vertex v);
void    PrintGraph(Graph *G, char *title, int complete);
Vertex *BFS(Graph *G, Vertex v);
Vertex *BFSd(Graph *G, Vertex v, int distance);
Vertex *DFS(Graph *G, Vertex v);

int getEdgeWeight(Graph *G, Vertex src, Vertex dest);
