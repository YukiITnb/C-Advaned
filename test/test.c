#include <stdio.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int time[10000];
    int ver[1000][1000];
}Time;

typedef struct
{
  JRB edges;
  JRB vertices;
}Graph;

Graph createGraph()
{
  Graph graph;
  graph.edges=make_jrb();
  graph.vertices=make_jrb();
  return graph;
}

void addVertex(Graph graph,int id,char *name)
{
  JRB node;
  JRB tree;
  node=jrb_find_int(graph.vertices,id);
  if(node==NULL)
    {
      jrb_insert_int(graph.vertices,id,new_jval_s(name));
      tree=make_jrb();
      jrb_insert_int(graph.edges,id,new_jval_v(tree));
    }
  else return;
}

char *getVertex(Graph graph,int id)
{
  JRB node;
  node=jrb_find_int(graph.vertices,id);
  if(node==NULL)
    return NULL;
  else return jval_s(node->val);
}

void addEdge(Graph graph,int v1,int v2)
{
  JRB node,tree,node2;
   node=jrb_find_int(graph.edges,v1);
  if(node==NULL)
    {
      tree=make_jrb();
      jrb_insert_int(graph.edges,v1,new_jval_v(tree));
      jrb_insert_int(tree,v2,new_jval_i(1));
    }
  else
    {
      tree=make_jrb();
      tree=(JRB)jval_v(node->val);
      node2=jrb_find_int(tree,v2);
      if(node2==NULL)
	{
	  jrb_insert_int(tree,v2,new_jval_i(1));
	}
      // jrb_insert_int(tree,v2,new_jval_i(1));
      node->val=new_jval_v(tree);
    }
}

int hasEdge(Graph graph,int v1,int v2)
{
  JRB node,tree;
  node=jrb_find_int(graph.edges,v1);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      jrb_traverse(node,tree)
	if(v2==jval_i(node->key))
	   return 1;
    }
  return 0;
}

int outdegree(Graph graph,int v,int* output)
{
  JRB node,tree;
  int total;
  node=jrb_find_int(graph.edges,v);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      total=0;
      jrb_traverse(node,tree)
	output[total++]=jval_i(node->key);
      return total;
    }
}

int indegree(Graph graph,int v,int*output)
{
  JRB node,tree;
  node=NULL;
  int key;
  int total;
  total=0;
  jrb_traverse(node,graph.vertices)
    {
      key = jval_i(node->key);
      if(hasEdge(graph,key,v)==1&&key!=v)
	output[total++]=key;
    }
  return total;
}

int getAdjacentVertices(JRB graph,int v,int *output)
{
  JRB node,tree;
  int total;
  node=jrb_find_int(graph,v);
  if(node==NULL)
    return 0;
  else
    {
      tree=(JRB) jval_v(node->val);
      total=0;
      jrb_traverse(node,tree)
	output[total++]=jval_i(node->key);
      return total;
    }
}

void dropGraph(Graph graph)
{
  JRB node;
  jrb_traverse(node, graph.vertices)
    jrb_free_tree(jval_v(node->val));
  jrb_traverse(node, graph.edges)
    jrb_free_tree(jval_v(node->val));
  
}

void menu(){
    printf("\n=========Menu=========\n");
    printf("1. Ma tran ke\n");
    printf("2. Danh sach ke\n");
    printf("3. \n");
    printf("4. \n");
    printf("5. Quit\n");
    printf("Choose: ");
}

int loadfile(FILE *fp,Graph g,Time t){
    int edg,ver1,ver2;
    int i,n,m,time,dem,dem1;
    fflush(stdin);
    fscanf(fp,"%d",&n);

    for (i=1;i<=n;i++){
    addVertex(g, i,(char)i);
    }
    
    fflush(stdin);
    fscanf(fp,"%d",&m);

    while(!feof(fp)){
        fflush(stdin);
        fscanf(fp,"%d",&ver1);
        fflush(stdin);
        fscanf(fp,"%d",&ver2);
        fflush(stdin);
        fscanf(fp,"%d",&time);
        addEdge(g, ver1, ver2);
        addEdge(g, ver2, ver1);
        t.time[dem] = time;
    }
    return n;
}

int main()
{
   int choice;
   int n,m,i,j;
   Time t;
   FILE *fp;
   fp = fopen("dothi.txt","r");
   Graph g = createGraph();
   n = loadfile(fp,g,t);
   do{
       menu();
       scanf("%d",&choice);
       if (choice == 1){
           for (i=1;i<=n;i++){
               for (j=1;j<=n;j++)
               {
                   printf("%d ",hasEdge(g,i,j));
               }
               printf("\n");
           }
       }
       if (choice == 2){
           for (i=1;i<=n;i++){
               printf("Castle %d: ",i);
               for (j=1;j<=n;j++){
                   if (hasEdge(g,i,j) == 1)
                   {
                       printf("%d " ,j);
                   }
                   
               }
               printf("\n");
           }
       }
   }while(choice!=5);
   return 0;
   
}
