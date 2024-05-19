#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int sudo[9][9];
} Node;

Node *createNode() {
  Node *n = (Node *)malloc(sizeof(Node));
  return n;
}

Node *copy(Node *n) {
  Node *new = (Node *)malloc(sizeof(Node));
  *new = *n;
  return new;
}

Node *read_file(char *file_name) {
  Node *n = createNode();
  FILE *file = fopen(file_name, "r");
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (!fscanf(file, "%d", &n->sudo[i][j]))
        printf("failed to read data!");
    }
  }

  fclose(file);
  return n;
}

void print_node(Node *n) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++)
      printf("%d ", n->sudo[i][j]);
    printf("\n");
  }
  printf("\n");
}

int is_valid(Node *n) {
  int filas_check[9][9] = {0};
  int columnas_check[9][9] = {0};
  int submatriz_check[9][9] = {0};
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      int num = n->sudo[i][j];
      if (num != 0) {
        // Si es distinto de 0, verificar que no se repita en la fila
        int submatriz = (i / 3) * 3 + (j / 3);
        if (filas_check[i][num - 1] == 1 || columnas_check[j][num - 1] == 1 ||
            submatriz_check[submatriz][num - 1] == 1) {
          return 0;
        }
        filas_check[i][num - 1] = 1;
        columnas_check[j][num - 1] = 1;
        submatriz_check[submatriz][num - 1] = 1;
      }
    }
  }
  return 1;
}

List *get_adj_nodes(Node *n) {
  List *list = createList();

  int fila = -1;
  int columna = -1;
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) {
        fila = i;
        columna = j;
        break;
      }
    }
    if (fila != -1)
      break;
  }
  if (fila == -1 && columna == -1) {
    return list;
  }
  for (i = 0; i < 9; i++) {
    Node *new_node = copy(n);
    new_node->sudo[fila][columna] = i + 1;
    if (is_valid(new_node)) {
      pushBack(list, new_node);
    } else {
      free(new_node);
    }
  }
  return list;
}

int is_final(Node *n) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) {
        return 0;
      }
    }
  }
  return 1;
}

Node *DFS(Node *initial, int *cont) {
  Stack *stack = createStack();
  push(stack, initial);
  *cont = 0;

  while (!is_empty(stack)) {
    Node *n = top(stack);
    pop(stack);
    (*cont)++;
    if (is_final(n)) {
      while (is_empty(stack)) {
        Node *aux = top(stack);
        pop(stack);
        free(aux);
      }
      free(stack);
      return n;
    }

    List *adj = get_adj_nodes(n);
    Node *aux = first(adj);
    while (aux) {
      push(stack, aux);
      aux = next(adj);
    }
    free(n);
    free(adj);
  }
  free(stack);
  return NULL;
}

/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/