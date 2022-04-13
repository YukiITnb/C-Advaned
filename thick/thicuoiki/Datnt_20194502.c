#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jrb.h"
#include "dllist.h"

typedef struct
{
    JRB user_by_name;
    JRB user_by_ID;
    JRB user_friend;
} Graph;

Graph createGraph()
{
    Graph g;
    g.user_by_name = make_jrb();
    g.user_by_ID = make_jrb();
    g.user_friend = make_jrb();
    return g;
}

void addUser(Graph g, int id, char *name)
{
    JRB node = jrb_find_str(g.user_by_name, name);
    if (node == NULL)
        jrb_insert_str(g.user_by_name, strdup(name), new_jval_i(id));
    JRB node2 = jrb_find_int(g.user_by_ID, id);
    if (node2 == NULL)
        jrb_insert_int(g.user_by_ID, id, new_jval_s(strdup(name)));
}

int getUser(Graph g, char *name)
{
    JRB node = jrb_find_str(g.user_by_name, name);
    if (node == NULL)
        return 0;
    else
        return jval_i(node->val);
}

void addFriend(Graph graph, int v1, int v2)
{
    JRB node, tree;
    if (!hasFriend(graph, v1, v2))
    {
        node = jrb_find_int(graph.user_friend, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.user_friend, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
    if (!hasFriend(graph, v2, v1))
    {
        node = jrb_find_int(graph.user_friend, v2);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.user_friend, v2, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}

int hasFriend(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.user_friend, v1);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int getFriend(Graph graph, int id, int *output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.user_friend, id);
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

char *getFriendName(Graph g, int id)
{
   JRB node = jrb_find_int(g.user_by_ID, id);
   if (node == NULL)
      return NULL;
   else
   {
      return jval_s(node->val);
   }
}


void splitString(char str1[], char str2[], int userID, Graph g)
{
    int friendID;
    int init_size = strlen(str1);
    char delim[] = " ";
    char *ptr = strtok(str1, delim);
    strcpy(str2, ptr);
    userID = atoi(str2);
    printf("%d\n", userID);
    ptr = strtok(NULL, delim);
    while (ptr != NULL)
    {
        strcpy(str2, ptr);
        friendID = atoi(str2);
        printf("%d\n", friendID);
        addFriend(g, userID, friendID);
        ptr = strtok(NULL, delim);
    }
}

int main()
{
    int choice;
    Graph g = createGraph();
    while (1)
    {
        printf("Your choice?\n1.Input Data\n2.List friend\n3.Popular person\n4.Check friend\n5.Suggest friend\n6.Exit\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            // printf("1");
            printf("submitted at 3:21\n");
            int quantity;
            int userID;
            char userName[25];
            char buffer[100];
            char userfriends[25];

            addFriend(g, 1002, 1003);
            addFriend(g, 1002, 1004);
            addFriend(g, 1003, 1005);

            FILE *fptr1 = fopen("user.txt", "r");
            FILE *fptr2 = fopen("friends.txt", "r");
            if (fptr1 == NULL)
            {
                printf("Error!");
                exit(1);
            }
            if (fptr2 == NULL)
            {
                printf("Error!");
                exit(1);
            }

            fscanf(fptr1, "%d", &quantity);
            printf("%d\n", quantity);
            for (int i = 0; i < quantity; i++)
            {
                fscanf(fptr1, "%d %s", &userID, userName);
                printf("%d %s\n", userID, userName);
                addUser(g, userID, userName);
            }

            while (fgets(buffer, 100, fptr2))
            {
                splitString(buffer, userfriends, userID, g);
            }

            fclose(fptr1);
            fclose(fptr2);
        }
        else if (choice == 2)
        {
            char name[25];
            printf("User name?\n");
            scanf("%s", name);
            int output[100];
            int total = getFriend(g, getUser(g, name), output);
            if (total == 0)
            {
                printf("Khong co ban!\n");
            }
            else 
            {
                for (int i = 0; i < total; i++)
                {
                    printf("%s\n", getFriendName(g, output[i]));
                }
            }
        }
        else if (choice == 3)
        {
            
        }
        else if (choice == 4)
        {
            printf("4");
        }
        else if (choice == 5)
        {
            printf("5");
        }
        else if (choice == 6)
        {
            break;
        }
        else
        {
            printf("Khong hop le. Vui long nhap lai!\n");
        }
    }
    return 0;
}
