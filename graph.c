// Graph ADT interface for Ass2 (COMP2521)
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include "Graph.h"

struct GraphRep{//ADJACENCY LIST
   AdjList *edges;   // adjacency matrix
   int    nV;      // #vertices
   int    nE;      // #edges
}GraphRep;

static bool validV(Graph g, Vertex v);
static adjListNode *makeNode(int n);
static AdjList insertAdjL(AdjList L, int n, int weight);
static AdjList deleteAdjL(AdjList L, int n);
static AdjList inAdjL(AdjList L, int n);
static void showAdjL(AdjList L);
static void freeAdjL(AdjList L);

Graph newGraph(int nV) {
  assert(nV!=0);
  int i;
  Graph g = malloc(sizeof(GraphRep)); 
  assert(g); 
  //let graph have input number of nodes (vertices)
  g->nV = nV;  //CHECK
  g->nE = 0; 
  //allocating memory for linked list of edges
  g->edges = malloc(nV * sizeof(AdjList)); 
  //initiating the list as empty 
  for(i=0;i<nV;i++){
    g->edges[i]=NULL; //for an empty list
  }
	return g;
}

int numVerticies(Graph g) {
  int num = g->nV; 
	return num;
}
//checks if the vertex is valid in g 
static bool validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}


void insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
  //checking if vertice and graph are valid
  assert(g != NULL && validV(g,src) && validV(g,dest));
	if(!inAdjL(g->edges[src],dest)){ //if vertice are not in graph, 
    g->edges[src] = insertAdjL(g->edges[src],dest,weight);
    g->edges[dest] = insertAdjL(g->edges[dest],src,weight);
    g->nE = g->nE + 1; 
  }
}

void  removeEdge(Graph g, Vertex src, Vertex dest) {
  assert(g != NULL && validV(g,src) && validV(g,dest));

  if (inAdjL(g->edges[src], dest)) {   //vertices src and dest in graph
    g->edges[src] = deleteAdjL(g->edges[src], dest);
    g->edges[dest] = deleteAdjL(g->edges[dest], src);
    g->nE = g->nE - 1;
  }
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
  assert(g != NULL && validV(g,src));
  return inAdjL(g->edges[src], dest);//if there is a list holding src & dest, itll return 0; if not, 1 
}
//outgoing edges
AdjList outIncident(Graph g, Vertex v) {
  Vertex i; 
  //allocating memory for the list
  AdjList *outList = malloc(sizeof(AdjList)); //CHECK WHETHER ADJLISTNODE OR ADJLIST
  for(i=0; i<g->nV ; i++){
    if(adjacent(g,i,v)){
      outList[i]= inAdjL(g->edges[i], v);//g->edges[i];//CHECK 
    }else{ //no outgoing edges
      return 0; 
    }
  }
	return *outList;
}
//ingoing edges
AdjList inIncident(Graph g, Vertex v) {
  Vertex i;
  AdjList *inList = malloc(sizeof(AdjList));
  for(i=0; i<g->nV ; i++){
    if(adjacent(g,i,v)){
      inList[i]= g->edges[i];//adjacent(g,i,v); 
    }else{ //no ingoing edges
      return 0; 
    }
  }
	return *inList;
}

void  showGraph(Graph g) {
  assert(g != NULL);
  int i;

  printf("Number of vertices: %d\n", g->nV);
  printf("Number of edges: %d\n", g->nE);
  for (i = 0; i < g->nV; i++) {
      printf("%d - ", i);
      showAdjL(g->edges[i]);
  }
}

void  freeGraph(Graph g) {
  assert(g != NULL);
  int i;
  for (i = 0; i < g->nV; i++){
    freeAdjL(g->edges[i]);
  }
  free(g);
}

//Linked List helper functions
//had https://www.cse.unsw.edu.au/~cs2521/19T1/lecs/week04a/exercises/list.c open for help 

//MAKES A NEW adjList NODE
static adjListNode *makeNode(int n){
   AdjList new = malloc(sizeof(AdjList));
   assert(new != NULL);
   new -> w = n; //new vertex w
   new -> weight = 0; 
   new -> next = NULL;
   return new;
}

//inserting into list 
static AdjList insertAdjL(AdjList L, int n, int weight) {
  if (inAdjL(L, n)){
    return L;
  }
   // add new node at the beginning
   adjListNode *new = makeNode(n);
   new->next = L;
   new->weight = weight;
   return new;
}

//EDGE REMOVAL
//delete list 
static AdjList deleteAdjL(AdjList L, int n) {
  if (L == NULL){
    return L;
  } 
  if (L->w == n){
    return L->next;
  }
  L->next = deleteAdjL(L->next, n);
  return L;
}
//in list? 
static AdjList inAdjL(AdjList L, int n) {
  if (L == NULL){
    return 0;
  } 
  if (L->w == n){
   // return 1;
  //}
   return inAdjL(L->next, n);
  }
  return inAdjL(L->next, n);
}
//check 
//format 
static void showAdjL(AdjList L) {
   if (L == NULL)
      putchar('\n');
   else {
      printf("%d ", L->w);
      showAdjL(L->next);
   }
}

static void freeAdjL(AdjList L) {
   if (L != NULL) {
      freeAdjL(L->next);
      free(L);
   }
}
