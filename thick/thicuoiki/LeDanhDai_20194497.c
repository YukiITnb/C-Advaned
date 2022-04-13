//#include<conio.h>
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
typedef struct
{
  int id;
  int number;
} soBan;

Graph createGraph();                                                    // tạo đồ thị
void addVertex(Graph graph, int id, char *name);                        // Thêm đỉnh vào đồ thị
char *getVertex(Graph graph, int id);                                   // lấy tên của đỉnh
void addEdge(Graph graph, int v1, int v2, double weight);               // Thêm cạnh và trọng số
int getEdgeValue(Graph graph, int v1, int v2);                          // lấy trọng số của cạnh
int indegree(Graph graph, int v, int *output);                          // bán bậc vào
int outdegree(Graph graph, int v, int *output);                         // bán bậc ra
void dropGraph(Graph graph);                                            // clear đồ thị
double shortestPath(Graph graph, int s, int t, int *path, int *length); // đường đi ngắn nhất
int DAG(Graph graph);                                                   // xét xem đồ thị có chu trình hay không

int main(int argc, char *argv[])
{
  Graph g = createGraph();
  int chon = 0;
  do
  {
    printf("============MENU================================\n");
    printf("|1. Input Data                                 |\n");
    printf("|2. List Friend                                |\n");
    printf("|3. Popular person                             |\n");
    printf("|4. Check friend                               |\n");
    printf("|5. Suggest friend                             |\n");
    printf("|6. Exit                                       |\n");
    printf("================================================\n");
    printf("Ban chon: ");
    scanf("%d", &chon);
    switch (chon)
    {
    case 1:
      inputData(g, argv);
      break;
    case 2:
      listFriend(g);
      break;
    case 3:
      numberFriend(g);
      break;
    case 4:
      checkFriend(g);
      break;
    case 5:
      printf("Em chua lam duoc");
      break;
    case 6:
      break;
    default:
      printf("Ban chon sai. Moi ban chon lai MENU!\n");
      break;
    }
  } while (chon != 6);
  return 0;
}

void inputData(Graph g, char *argv[])
{
  char file1[30], file2[30];
  int temp;
  strcpy(file1, argv[1]);
  strcpy(file2, argv[2]);
  printf("Nhap vao ten file thu nhat: %s\n", file1);
  IS in1, in2;
  in1 = new_inputstruct(file1);
  if (!in1)
  {
    puts("Cannot open the given file");
    exit(1);
  }
  printf("Noi dung %s la:\n", file1);
  while (get_line(in1) > 0)
  {
    printf("%s", in1->text1);
    if (in1->line != 1)
    {
      int id;
      char *name;
      id = atoi(in1->fields[0]);
      name = in1->fields[1];
      addVertex(g, id, name);
    }
  }
  printf("\n");
  in2 = new_inputstruct(file2);
  printf("Noi dung file %s la :\n", file2);
  while (get_line(in2) > 0)
  {
    printf("%s", in2->text1);
    for (int i = 1; i < in2->NF; i++)
    {
      addEdge(g, atoi(in2->fields[0]), atoi(in2->fields[i]), 1);
    }
  }
  printf("\n");
}
void checkFriend(Graph g)
{
  char ten1[30];
  char ten2[30];
  printf("nhap vao ten nguoi thu nhat: \n");
  scanf("%s", &ten1);
  printf("nhap vao ten nguoi thu hai: \n");
  scanf("%s", &ten2);
  printf("%s", ten2);
  int id1 = getIdByName(g, ten1);
  int id2 = getIdByName(g, ten2);
  if (getEdgeValue(g, id1, id2) == INFINITIVE_VALUE)
  {
    printf("Khong la ban be cua nhau \n");
  }
  else
    printf("La ban be cua nhau \n");
}
void listFriend(Graph g)
{
  char ten[30];
  JRB tree, node, tree2;
  int id;
  printf("nhap ten moi nguoi: \n");
  scanf("%s", &ten);
  id = getIdByName(g, ten);
  if (id == NULL)
  {
    printf("Ban da nhap sai ten !!!!");
    return;
  }
  else
  {
    node = jrb_find_int(g.edges, id);
    if (node == NULL)
    {
      printf("Nguoi nay khong co ban\n");
      return;
    }
    // tree = node->key;
    tree = (JRB)jval_v(node->val);
    printf("Ban cua %s: \n", ten);
    jrb_traverse(tree2, tree)
    {
      printf("%s\n", getVertex(g, jval_i(tree2->key)));
    }
  }
}
void numberFriend(Graph g)
{
  JRB tree, node, tree2;
  int i = 0;
  int soban;
  soBan arr[100];
  jrb_traverse(node, g.edges)
  {
    int j = 0;
    arr[i].id = jval_i(node->key);
    tree = (JRB)jval_v(node->val);
    jrb_traverse(tree2, tree)
    {
      arr[i].number++;
    }
    i++;
  }
  for (int j = 0; j < i; j++)
  {
    printf("%d\t", arr[j].id);
    printf("%d", arr[j].number);
    printf("\n");
  }
}

Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}

void addVertex(Graph g, int id, char *name)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph g, int id)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    return NULL;
  else
  {
    return jval_s(node->val);
  }
}

int getIdByName(Graph g, char *name)
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

void addEdge(Graph graph, int v1, int v2, double weight)
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

int getEdgeValue(Graph graph, int v1, int v2)
{
  JRB find = jrb_find_int(graph.edges, v1);
  JRB edge;
  if (find == NULL)
    return INFINITIVE_VALUE;
  JRB tree;
  tree = make_jrb();
  tree = jval_v(find->val);
  edge = jrb_find_int(tree, v2);
  if (edge == NULL)
    return INFINITIVE_VALUE;
  return jval_d(edge->val);
}

int indegree(Graph graph, int v, int *output)
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

int outdegree(Graph graph, int v, int *output)
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

    printf("u ben ngoai: %d \n", u);
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

int DAG(Graph graph)
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

void dropGraph(Graph graph)
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
