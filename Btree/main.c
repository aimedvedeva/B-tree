#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Btree.h"

int main(void){
  Btree *tree = CreateTree();

  Insert(tree, 'G');
  Insert(tree, 'M');
  Insert(tree, 'X');
  Insert(tree, 'A');
  Insert(tree, 'P'); 
  Insert(tree, 'C');
  Insert(tree, 'D');
  Insert(tree, 'E');
  Insert(tree, 'J');
  Insert(tree, 'K');
  Insert(tree, 'N'); 
  Insert(tree, 'O');
  Insert(tree, 'R');
  Insert(tree, 'S');
  Insert(tree, 'T');
  Insert(tree, 'U');
  Insert(tree, 'V');
  Insert(tree, 'Y');
  Insert(tree, 'Z');
  Insert(tree, 'W');
  Insert(tree, 'H');
  Insert(tree, 'L');
  FILE *F = fopen("output.txt", "w");
  PrintTree(F, tree);
  fclose(F);
  return 0;
}