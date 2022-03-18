/*
* Luke Schaefer
* 18186970
* LESDMG
*/

#include <stdio.h>
#include <stdlib.h>


// *** Structure Declarations ***
typedef struct NodeStruct {
    int data;
    struct NodeStruct * left;
    struct NodeStruct * right;
    struct NodeStruct * parent;
    //black = 0; red = 1;
    int color;
} Node;

typedef struct TreeStruct {
    Node * root;
} RBTree;

typedef struct StackNode {
    int data;
    struct StackNode * next;
} SNode;

typedef struct StackStruct {
    SNode * head;
    int size;
} Stack;

// *** RBTree Functions
RBTree * RBConstruct(char * filename);
int RBInsert(RBTree * tree, int data);
int BuildRBTree(RBTree tree, int * data);
Node * makeNode(int data, int color);
void Insert(RBTree * tree, Node * newNode);
void RBTreeFixup(RBTree * tree, Node * tracer);
Node * Case1Fix(RBTree * tree, Node * newNode);
Node * Case2Fix(Node * tracer);
void Case3Fix(RBTree * tree, Node * tracer);
Node * LeftRotate(Node * tracer);
Node * RightRotate(Node * tracer);
int CheckCase(Node * tracer);
int CheckPosition(Node * tracer);
Node * FindParent(Node * tracer, int data);
Node * getUncle(Node * tracer);
Node * getGrandparent(Node * tracer);
void freeTree(RBTree * tree);
void freeNodes(Node * tracer);

// *** PP2 Functions ***
void PrintInOrderTraversal(Node * tracer);
void PrintRBInOrder(RBTree * tree);
int GetHeight(Node * tracer);
void PrintHeight(RBTree * tree);
int GetSecondBiggest(Node * tracer);
void PrintSecondBiggest(RBTree * tree);
int GetBlackHeight(Node * tracer);
void PrintBlackHeight(RBTree * tree);