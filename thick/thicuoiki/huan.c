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

int getEdgeValue(Graph graph, int v1, int v2)  // lay trong so cua canh
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
    printf("|1. Input Data                                 |\n");
    printf("|2. List Friend                                |\n");
    printf("|3. Popular person                             |\n");
    printf("|4. Check friend                               |\n");
    printf("|5. Suggest friend                             |\n");
    printf("|6. Exit                                       |\n");
    printf("================================================\n");
    printf("Ban chon: ");
}
void inputdata(Graph g){
    int id,maxuser;
    char name[50];
    char str[10000];
    int usid,frid;
    FILE *fp1 = fopen("users.txt","r");
    FILE *fp2 = fopen("friends.txt","r");
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
    printf("%d\n",maxuser);
    while (!feof(fp1) )
    {
        fflush(stdin);
        fscanf(fp1,"%d",&id);
        fflush(stdin);
        fscanf(fp1,"%s",&name);
        printf("%d %s\n",id,name);
        addVertex(g,id,name);
    }
    while (!feof(fp2))
    {
        fflush(stdin);
        fgets(str,sizeof(str),fp2);
        printf("%s",str);
        char *userid;
        char *friendid;
        char str1[100],str2[100];
        userid = strtok(str, " ");
        strcpy(str1,userid);
        usid = atoi(str1);
        friendid = strtok(NULL, " ");
        while (friendid != NULL)
        {
            strcpy(str2,friendid);
            frid = atoi(str2);
            addEdge(g,usid,frid,1);
            friendid = strtok(NULL, " ");
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void listfriend(Graph g)
{
    char name[50];
    int id;
    JRB tree, node, tree2;
    printf("nhap ten:");
    scanf("%s", &name);
    id = getIdByName(g,name);
    node = jrb_find_int(g.edges, id);
    if (node == NULL)
    {
      printf("Nguoi nay khong co ban\n");
      return;
    }
    // tree = node->key;
    tree = (JRB)jval_v(node->val);
    printf("Ban cua %s: \n", name);
    jrb_traverse(tree2, tree)
    {
      printf("%s\n", getVertex(g, jval_i(tree2->key)));
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

void checkFriend(Graph g)
{
  char ten1[30];
  char ten2[30];
  printf("nhap vao ten nguoi thu nhat: \n");
  scanf("%s", &ten1);
  printf("nhap vao ten nguoi thu hai: \n");
  scanf("%s", &ten2);
  int id1 = getIdByName(g, ten1);
  int id2 = getIdByName(g, ten2);
  if (getEdgeValue(g, id1, id2) == INFINITIVE_VALUE)
  {
    printf("Khong la ban be cua nhau \n");
  }
  else
    printf("La ban be cua nhau \n");
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
            listfriend(g);
        if (choice == 3)
            numberFriend(g);
        if (choice == 4)
            checkFriend(g);
        if (choice == 6)
            break;
    } while (choice != 6);
    
}