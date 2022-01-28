#pragma once

typedef enum{
  MyBool_false,
  MyBool_true
}MyBool;

typedef struct tag_BtreeNode{
  int nodesNum;
  char *keys;         //max -> 2t-1 <-->  2t childs
  struct tag_BtreeNode **childs;//min ->  t-1 <-->   t childs  - mas of pointers to childs
  MyBool isLeaf;
}BtreeNode;

typedef struct tag_Btree{
  BtreeNode *root;
}Btree;

Btree *CreateTree();
void Insert(Btree *tree, char key);
void PrintTree(FILE *F, Btree *tree);