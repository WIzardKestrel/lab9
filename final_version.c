#include <stdio.h>
#include <stdlib.h>
#define MAX 3
#define MIN 1
#define MAX_Q_SIZE 500
#define null NULL

typedef struct node{
  int keys[MAX + 1];
  int key_count;
  struct node *children[MAX + 1];
}node;

node ** init_q(int* front, int* rear)
{
    node ** queue = (node**)malloc(sizeof(struct node*) * MAX_Q_SIZE);
    *front = *rear = 0;
    return queue;
}
void enq(struct node** queue, int* rear,
         struct node* new_node)
{
    queue[*rear] = new_node;
    (*rear)++;
}

struct node* deq(struct node** queue, int* front) {
    (*front)++;
    return queue[*front - 1];
}
void prt_lvl(struct node* my_root)
{
    int rear, front;
    node** queue = init_q(&front, &rear);
    node* temp_node = my_root;
    node * prev_temp = my_root;
    int print_nl = 0;
    int i;
    while (temp_node) {
        for (i = 0; i < temp_node -> key_count; ++i) {
            printf("%d", temp_node -> keys[i + 1]);

            if(i < temp_node -> key_count - 1)
                printf("-");
        }
        for (int j = 0; j <= temp_node -> key_count; ++j) {
            enq(queue, &rear, temp_node->children[j]);
        }
        prev_temp = temp_node;
        temp_node = deq(queue, &front);
        if(temp_node != null && prev_temp -> keys[i] > temp_node -> keys[i])
            printf("\n");
        else
            printf(", ");
    }

}


node * peek(struct node** queue, int front) {

    return queue[front - 1];
}


struct node *root;

struct node *create_node(int val, struct node *child) {
  struct node *newNode;
  newNode = (struct node *)malloc(sizeof(struct node));
  newNode->keys[1] = val;
  newNode->key_count = 1;
  newNode->children[0] = root;
  newNode->children[1] = child;
  return newNode;
}

void insert_key(int val, int pos, struct node *node,
                struct node *child) {
  int j = node->key_count;
  while (j > pos) {
    node->keys[j + 1] = node->keys[j];
    node->children[j + 1] = node->children[j];
    j--;
  }
  node->keys[j + 1] = val;
  node->children[j + 1] = child;
  node->key_count++;
}

void split(int val, int *pval, int pos, struct node *node,
           struct node *child, struct node **newNode) {
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *newNode = (struct node *)malloc(sizeof(struct node));
  j = median + 1;
  while (j <= MAX) {
    (*newNode)->keys[j - median] = node->keys[j];
    (*newNode)->children[j - median] = node->children[j];
    j++;
  }
  node->key_count = median;
  (*newNode)->key_count = MAX - median;

  if (pos <= MIN) {
      insert_key(val, pos, node, child);
  } else {
      insert_key(val, pos - median, *newNode, child);
  }
  *pval = node->keys[node->key_count];
  (*newNode)->children[0] = node->children[node->key_count];
  node->key_count--;
}

int setValue(int val, int *pval,
             struct node *node, struct node **child) {
  int pos;
  if (!node) {
    *pval = val;
    *child = NULL;
    return 1;
  }

  if (val < node->keys[1]) {
    pos = 0;
  } else {
    for (pos = node->key_count;
         (val < node->keys[pos] && pos > 1); pos--)
      ;
    if (val == node->keys[pos]) {
      return 0;
    }
  }
  if (setValue(val, pval, node->children[pos], child)) {
    if (node->key_count < MAX) {
        insert_key(*pval, pos, node, *child);
    } else {
        split(*pval, pval, pos, node, *child, child);
      return 1;
    }
  }
  return 0;
}

// Insert the value
void insert(int val) {
  int flag, i;
  struct node *child;

  flag = setValue(val, &i, root, &child);
  if (flag)
    root = create_node(i, child);
}


int main() {
  int data = 0;
  scanf("%d", &data);
  while(data != -1){
      insert(data);
      scanf("%d", &data);
  }
    prt_lvl(root);
}
