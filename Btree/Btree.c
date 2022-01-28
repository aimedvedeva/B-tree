#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Btree.h"
#define TreeDegree 3
#define MaxNumOfChilds TreeDegree * 2 
#define MaxNumOfKeys   TreeDegree * 2 - 1
#define MinNumOfChilds TreeDegree 
#define MinNumOfKeys   TreeDegree - 1
#define SIZE 20

void f_InsertNonFull(BtreeNode *node, char key);
void f_BtreeSplitChild(BtreeNode *parent, BtreeNode *child, int childNum);
void f_DestroyTreeNode(BtreeNode *node);
BtreeNode *f_AllocateNode();

Btree *CreateTree(){
  Btree *tree = (Btree*)malloc(sizeof(Btree));
  tree->root = (BtreeNode*)malloc(sizeof(BtreeNode));
  
  tree->root->isLeaf = MyBool_true;
  tree->root->nodesNum = 0;
  tree->root->keys = (char*)malloc(sizeof(char) * MaxNumOfKeys);

  for (int i = 0; i < MaxNumOfKeys; i++){
    tree->root->keys[i] = 0;
  }

  tree->root->childs = (BtreeNode**)malloc(sizeof(BtreeNode*) * MaxNumOfChilds);

  for (int i = 0; i < MaxNumOfChilds; i++){
    tree->root->childs[i] = NULL;
  }
  return tree;
}

void f_InsertNonFull(BtreeNode *node, char key){
  int i = node->nodesNum;

  if (node->isLeaf == MyBool_true){
    while (i > 0 && key < node->keys[i - 1]){
      node->keys[i] = node->keys[i - 1];
      i -= 1;
    }
    node->keys[i] = key;
    node->nodesNum += 1;
  }
  else{
    while (i >= 0 && key < node->keys[i - 1]){
      i -= 1;
    }
//    i += 1;
    if (node->childs[i]->nodesNum == MaxNumOfKeys){
      f_BtreeSplitChild(node, node->childs[i], i);
      if (key > node->keys[i]){
        i += 1;
      }
    }
    f_InsertNonFull(node->childs[i], key);
  }
}

void Insert(Btree *tree, char key){
  BtreeNode *root = tree->root;
  if (root->nodesNum == MaxNumOfKeys){
    BtreeNode *newNode = f_AllocateNode();
    tree->root = newNode;
    newNode->isLeaf = MyBool_false;
    newNode->nodesNum = 0;
    newNode->childs[0] = root;
    f_BtreeSplitChild(newNode, root, 0);
    f_InsertNonFull(newNode, key);
  }
  else{
    f_InsertNonFull(root, key);
  }
}

void f_BtreeSplitChild(BtreeNode *parent, BtreeNode *child, int childNum){
  BtreeNode *newChild = f_AllocateNode();

  newChild->isLeaf = child->isLeaf;
  newChild->nodesNum = MinNumOfKeys;

  //копируем последние t - 1 ключи из child в mewChild
  for (int j = 0; j < MinNumOfKeys; j++){
    newChild->keys[j] = child->keys[j + TreeDegree];
    child->keys[j + TreeDegree] = 0;
  }

  //копируем соответствующие ссылки на детей 
  if (child->isLeaf != MyBool_true){
    for (int j = 0; j < MinNumOfChilds; j++){
      newChild->childs[j] = child->childs[j + TreeDegree];
      child->childs[j + TreeDegree] = NULL;
    }
  }

  child->nodesNum = MinNumOfKeys;

  //сдвигаем прауб половину детей вправо на одну позицию, чтобы вставить ребенка-медиану
  for (int j = parent->nodesNum; j >= childNum + 1; j--){
    parent->childs[j + 1] = parent->childs[j];
  }

  //вставляем ребенка-медиану
  parent->childs[childNum + 1] = newChild;

  //сдвигаем у родителя ключи вправо, освобождая позицию для ключа-медианы
  for (int j = parent->nodesNum - 1; j >= childNum; j--){
    parent->keys[j + 1] = parent->keys[j];
  }

  //вставляем ключ-медиану
  parent->keys[childNum] = child->keys[TreeDegree - 1];
  child->keys[TreeDegree - 1] = 0;

  //редактируем кол-во узлов в родителе
  parent->nodesNum += 1;
}

BtreeNode *f_AllocateNode(){
  BtreeNode *node = (BtreeNode*)malloc(sizeof(BtreeNode));

  node->keys = (char*)malloc(sizeof(char) * MaxNumOfKeys);

  for (int i = 0; i < MaxNumOfKeys; i++){
    node->keys[i] = 0;
  }

  node->childs = (BtreeNode**)malloc(sizeof(BtreeNode*) * MaxNumOfChilds);

  for (int i = 0; i < MaxNumOfChilds; i++){
    node->childs[i] = NULL;
  }

  return node;
}

void f_DestroyTreeNode(BtreeNode *node){
  free(node->keys);
  for (int i = 0; i < MaxNumOfChilds; i++){
    free(node->childs[i]);
  }
  free(node->childs);
}


void StrCopy(char *from, char *to){
  int i;
  for (i = 0; i < MaxNumOfKeys; i++){
    if (from[i] == '\0'){
      break;
    }
    to[i] = from[i];
  }
  to[i] = '\0';
}

void f_InitMatrixForPrint(int y, int *x, BtreeNode *node, char ***matrixForPrint){
  if (node != NULL){
    for (int i = 0; i < node->nodesNum + 1; i++){
      f_InitMatrixForPrint(y + 2, x, node->childs[i], matrixForPrint);
    }

    
    StrCopy(node->keys, matrixForPrint[y][*x]);
    (*x) += 1;
    /*for (int i = (node->nodesNum) / 2; i < node->nodesNum; i++){
      f_InitMatrixForPrint(y + (node->nodesNum) / 2, x, node->childs[i], matrixForPrint);
    }*/
    
  }
}

void f_PrintTreeMatrix(FILE *Output, char ***matrixForPrint){
  for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      if (matrixForPrint[i][j] == NULL){
        fprintf(Output, "  ");
      }
      else{
        fprintf(Output, "%s  ", matrixForPrint[i][j]);
      }
    }
    fprintf(Output, "\n");
  }
  fprintf(Output, "\n------------------------------\n");
}

void f_InitMatrixByZero(char ***matrixForPrint){
  for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      matrixForPrint[i][j][0] = '\0';
    }
  }
}

char ***f_CreateMatrix(){
  char ***MatrixForPrint = (char***)malloc(sizeof(char**) * SIZE);
  for (int i = 0; i < SIZE; i++){
    MatrixForPrint[i] = (char**)malloc(sizeof(char*) * SIZE);
    for (int j = 0; j < SIZE; j++){
      MatrixForPrint[i][j] = (char*)malloc(sizeof(char) * (MaxNumOfKeys + 1));
    }
  }
  return MatrixForPrint;
}

void f_DeleteMatrix(char ***Matrix){
  for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      free(Matrix[i][j]);
    }
    free(Matrix[i]);
  }
  free(Matrix);
}

void PrintTree(FILE *F, Btree *tree){
  char ***MatrixForPrint = f_CreateMatrix();
  f_InitMatrixByZero(MatrixForPrint);

  int x = 0;
  f_InitMatrixForPrint(0, &x, tree->root, MatrixForPrint);
  f_PrintTreeMatrix(F, MatrixForPrint);
  f_DeleteMatrix(MatrixForPrint);
}