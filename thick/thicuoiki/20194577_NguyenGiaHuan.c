#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jrb.h"
#include "dllist.h"
#include "fields.h"

#define INFINITIVE_VALUE 10000000

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;

Graph createGraph()  // tạo đồ thị 
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}

void addVertex(Graph g, int id, char *name)  // them dinh vao do thi voi id va ten
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph g, int id)  // lay dinh trong do thi dua vao id
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    return NULL;
  else
  {
    return jval_s(node->val);
  }
}

int getIdByName(Graph g, char *name)  // lay id bang ten dinh
{
  JRB node;
  jrb_traverse(node, g.vertices)
  {
    if (strcmp(jval_s(node->val), name) == 0)
    {
      return jval_i(node->key);
    }
  }
  return NULL;
}

void addEdge(Graph graph, int v1, int v2, double weight)   // them canh va trong so
{
  JRB node, tree;
  if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
  {
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
      tree = make_jrb();
      jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    }
    else
    {
      tree = (JRB)jval_v(node->val);
    }
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
}

double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB find = jrb_find_int(graph.edges, v1);
  JRB edge;
    if (find == NULL)
        return INFINITIVE_VALUE;
    JRB tree;
    tree= make_jrb();
    tree=jval_v(find->val);
    edge = jrb_find_int(tree, v2);
    if (edge == NULL)
        return INFINITIVE_VALUE;
    JRB find2=jrb_find_int(graph.vertices,v2);
    return jval_d(find2->val);
}

int indegree(Graph graph, int v, int *output)  //ban bac vao
{
  JRB tree, node;
  int total = 0;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v))
    {
      output[total] = jval_i(node->key);
      total++;
    }
  }
  return total;
}

int outdegree(Graph graph, int v, int *output)  // ban bac ra
{
  JRB tree, node;
  int total;
  node = jrb_find_int(graph.edges, v);
  if (node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
  {
    output[total] = jval_i(node->key);
    total++;
  }
  return total;
}

double shortestPath(Graph g, int s, int t, int *path, int *length) // s là start, t là end, path là id của các đỉnh, length là số các đỉnh đã đi qua
{
  double distance[1000], min, w, total;
  int previous[1000], tmp[1000];
  int n, output[100], i, u, v, start;
  Dllist ptr, queue, node;

  for (i = 0; i < 1000; i++)
    distance[i] = INFINITIVE_VALUE;
  distance[s] = 0;
  previous[s] = s;

  queue = new_dllist();             // queue là 1 dllist
  dll_append(queue, new_jval_i(s)); // thêm node s vào tận cùng của list
  while (!dll_empty(queue))         // xem list có trống hay ko
  {
    // get u from the priority queue
    min = INFINITIVE_VALUE;
    //ham tim ra trong queue diem co do dai tu s den min
    dll_traverse(ptr, queue) //ptr la 1 struct co val, flink, blink
    {
      u = jval_i(ptr->val);
      if (min > distance[u])
      {
        min = distance[u];
        node = ptr;
      }
    }

    dll_delete_node(node);
    if (u == t)
      break;                     // stop at t
    n = outdegree(g, u, output); // output ở đây là các đỉnh thuộc bán bậc ra của u
    for (i = 0; i < n; i++)      //
    {
      //xet cac lan can v cua u
      v = output[i];
      w = getEdgeValue(g, u, v);
      if (distance[v] > distance[u] + w) //neu s.v > s.u thi cap nhat lai
      {
        distance[v] = distance[u] + w;
        previous[v] = u;
      }
      dll_append(queue, new_jval_i(v)); //cho v vao trong queue
    }
  }
  total = distance[t]; //tim duoc khoang cach
  if (total != INFINITIVE_VALUE)
  {
    tmp[0] = t;
    n = 1;
    while (t != s)
    {
      t = previous[t];
      tmp[n++] = t;
    }
    for (i = n - 1; i >= 0; i--)
      path[n - i - 1] = tmp[i]; //mang path la dao lon cua mang t
    *length = n;
  }
  return total;
}

int DAG(Graph graph)   // xem xet do thi co chu trinh hay khong
{
  int visited[1000];
  int n, output[100], i, u, v, start;
  Dllist node, stack;
  JRB vertex;
  jrb_traverse(vertex, graph.vertices)
  {
    memset(visited, 0, sizeof(visited));
    start = jval_i(vertex->key);
    stack = new_dllist();
    dll_append(stack, new_jval_i(start));

    while (!dll_empty(stack))
    {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u])
      {
        visited[u] = 1;
        n = outdegree(graph, u, output); // lấy danh sách các bán bậc ra của u
        for (i = 0; i < n; i++)
        {
          v = output[i];
          if (v == start) // cycle detected
            return 0;     // nếu trong danh sách đó có đỉnh là start thì return về là có cycle
          if (!visited[v])
            dll_append(stack, new_jval_i(v)); // còn không có thì add thêm và dllist các bán bậc ra đó
        }
      }
    }
  }
  return 1; // no cycle
}

void dropGraph(Graph graph)  // clear do thi
{
  JRB node, tree;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    jrb_free_tree(tree);
  }
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}

void menu(){
    printf("\n============MENU================================\n");
    printf("|1. Read                                         |\n");
    printf("|2. Adjacency                                    |\n");
    printf("|3. Path                                         |\n");
    printf("|4. Summarize                                    |\n");
    printf("|5. Exit                                         |\n");
    printf("==================================================\n");
    printf("Ban chon: ");
}

void inputdata(Graph g){
    int id,maxuser;
    char name[50];
    char str[10000];
    int usid,frid;
    FILE *fp1 = fopen("vocab.txt","r");
    FILE *fp2 = fopen("graph.txt","r");
    if (fp1 == NULL)
    {
        printf("error!");
        exit(1);
    }
    if (fp2 == NULL)
    {
        printf("error!");
        exit(1);
    }
    fflush(stdin);
    fscanf(fp1,"%d",&maxuser);
    while (!feof(fp1) )
    {
        fflush(stdin);
        fscanf(fp1,"%s",&name);
        fflush(stdin);
        fscanf(fp1,"%d",&id);
        addVertex(g,id,name);
    }
    int maxedg;
    double dis;
    int id1,id2;
    fflush(stdin);
    fscanf(fp2,"%d",&maxedg);
    while (!feof(fp2))
    {
        fflush(stdin);
        fscanf(fp2,"%lf",&dis);
        fflush(stdin);
        fscanf(fp2,"%d",&id1);
        fflush(stdin);
        fscanf(fp2,"%d",&id2);
        addEdge(g,id1,id2,dis);
    }

    fclose(fp1);
    fclose(fp2);
}

void adjacency(Graph g)
{
  char ten1[30];
  char ten2[30];
  printf("nhap vao tu thu nhat: \n");
  scanf("%s", &ten1);
  printf("nhap vao tu thu hai: \n");
  scanf("%s", &ten2);
  int id1 = getIdByName(g, ten1);
  int id2 = getIdByName(g, ten2);
  if (getEdgeValue(g, id1, id2) == INFINITIVE_VALUE)
  {
    printf("Not adjacency \n");
  }
  else
  {
    printf("Adjacency \n");
    double dis;
    dis = getEdgeValue(g, id1, id2);
    printf("%f\n", dis);
  }
}

void Path(Graph g)
{
    char word1[100],word2[100];
    int path[10000];
    double length,weight;
    int id1,id2;
    int i;
    printf("nhap tu thu nhat :\n");
    scanf("%s", &word1);
    printf("nhap tu thu hai :\n");
    scanf("%s", &word2);
    id1 = getIdByName(g,word1);
    id2 = getIdByName(g,word2);
    weight = shortestPath(g, id1, id2, path, &length);
    if (weight == INFINITIVE_VALUE)
      printf("No path between %d and %d\n", id1, id2);
    else
      {
	    printf("Total weight: %lf\n", weight);
      }

}

void Summarize(Graph g)
{
  printf("em chua lam duoc !!!");
}
int main(){
    int choice;
    Graph g = createGraph();
    do
    {
        menu();
        scanf("%d", &choice);
        if (choice == 1)
            inputdata(g);
        if (choice == 2)
            adjacency(g);
        if (choice == 3)
            Path(g);
        if (choice == 4)
            Summarize(g);
        if (choice == 5)
            break;
    } while (choice != 5);
}