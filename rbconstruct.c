/*
* Luke Schaefer
* 18186970
* LESDMG
*/

#include "rbconstruct.h"
#include <stdio.h>
#include <stdlib.h>

/*
* *** PP2 ASSIGNMENT FUNCTIONS ***
*/ 

//prints the given RBTree with an inorder traversal
void PrintRBInOrder(RBTree * tree) {
    if(tree->root != NULL) {
        puts("In-Order traversal of the tree:");
        PrintInOrderTraversal(tree->root);
        puts("");
    }
    else {
        puts("Tree is empty.");
    }
    
}

//prints the height of the given RBTree
void PrintHeight(RBTree * tree) {
    printf("The height of the RB tree is %d\n", GetHeight(tree->root));
}

//prints the black height of the given RBTree
void PrintBlackHeight(RBTree * tree) {
    printf("The black height of the RB tree is %d\n", GetBlackHeight(tree->root));
}

//prints the second largest value in the RBTree
void PrintSecondBiggest(RBTree * tree) {
    if(tree->root != NULL) {
        printf("The second largest element in the RB tree is %d\n", GetSecondBiggest(tree->root));
    }
    else {
        puts("Tree is empty.");
    }
    
}


/* 
* *** PP2 ASSIGNMENT HELPER FUNCTIONS
*/

//recursively traverses the tree with an in-order traversal and prints out the value at each node and it's color in the tree
void PrintInOrderTraversal(Node * tracer) {
    if(tracer != NULL) {
        PrintInOrderTraversal(tracer->left);
        if(tracer->color) {
            printf("%d-R ", tracer->data);
        }
        else {
            printf("%d-B ", tracer->data);
        }
        
        PrintInOrderTraversal(tracer->right);
    }
}

//recursively gets the height of the tree by checking the left and right subtree heights 
//returns the larger of the left and right subtrees
int GetHeight(Node * tracer) {
    if(tracer == NULL) {
        return 0;
    }
    else {
        int leftHeight = GetHeight(tracer->left) + 1;
        int rightHeight = GetHeight(tracer->right) + 1;
        if(leftHeight > rightHeight) {
            return leftHeight;
        }
        else {
            return rightHeight;
        }
    }
    
}

//recursively gets the black height of the tree by first checking if the left and right black height are the same
//returns the black height if the left and right subtrees are equal, returns -1 if they are not
int GetBlackHeight(Node * tracer) {
    
    if(tracer == NULL) {
        return 1;
    }
    else {
        int leftBHeight = GetBlackHeight(tracer->left);
        int rightBHeight = GetBlackHeight(tracer->right);
        if(leftBHeight != rightBHeight) {
            return -1;
        }
        else {
            if(tracer->color == 0) {
                return leftBHeight + 1;
            }
            else {
                return leftBHeight;
            }
        }
    }
    
}

//iterively finds then returns the second largest number in the tree
//precondition: tracer is not null
int GetSecondBiggest(Node * tracer) {

    while(tracer->right != NULL) {
        tracer = tracer->right;
    }

    if(tracer->left != NULL) {
        return tracer->left->data;
    }
    else {
        if(tracer->parent != NULL) {
            return tracer->parent->data;
        }
        else {
            return tracer->data;
        }
        
    }
}



/*
* *** RBTREE FUNCTIONS
*/

//contructs a RBTree from a given file by iteratively using RBInsert with the read-in integer.
//returns the constructed tree, or NULL if malloc failed on tree creation or if the file couldn't be opened.

RBTree * RBConstruct(char * filename) {
    
    FILE * fp = fopen(filename, "r");
    if(fp == NULL) {
        puts("File could not be opened.");
        return NULL;
    }

    RBTree * newTree = malloc(sizeof(RBTree));
    if(newTree == NULL) {
        puts("Malloc failure");
        return NULL;
    }
    
    int a;
    while(fscanf(fp, " %d,", &a) == 1) {
        RBInsert(newTree, a);
    }
    return newTree;
}


//returns 1 on successful insert, returns 0 if there was an error
int RBInsert(RBTree * tree, int data) {
    //create a new node to insert and check if creation was successful
    Node * newNode = makeNode(data, 1);
    if(newNode == NULL) {
        return 0;
    }

    //if the tree is empty, insert at the root
    if(tree->root == NULL) {
        newNode->color = 0;
        tree->root = newNode;
        return 1;
    }
    else {
        Insert(tree, newNode);

        //if the parent is not red (black) then return
        if(newNode->parent->color == 0) {
            return 1;
        } 
        //if the parent is red, then must do checks
        else {
            RBTreeFixup(tree, newNode);
            tree->root->color = 0;
            return 1;
        }
            
    }
        
    
}

//recusively fixes the tree using 3 main case fixes
void RBTreeFixup(RBTree * tree, Node * tracer) {

    //fix if the parent is red
    if(tracer->parent != NULL && tracer->parent->color) {

        //create an uncle
        Node * uncle = getUncle(tracer);

        //if the uncle's color is red, case1 fix is required. if not, case2 or case3
        if(uncle != NULL && uncle->color) {
            Node * grandparent = Case1Fix(tree, tracer);
            RBTreeFixup(tree, grandparent);
        }
        else {
            int whichCase = CheckCase(tracer);
            if(whichCase == 2) {
                Node * newRotate = Case2Fix(tracer);
                Case3Fix(tree, newRotate);
            }
            else {
                Case3Fix(tree, tracer);
            }

        }
    }
    
}

//fixes case3. Case3 occurs when the uncle is black and the parent node is in the same position as the child node (left and left, right and right)
void Case3Fix(RBTree * tree, Node * tracer) {

    Node * parent = tracer->parent;
    Node * grandparent = getGrandparent(tracer);
    parent->color = 0;
    grandparent->color = 1;

    
    int a = CheckPosition(tracer);
    if(a == 1) {
        if(grandparent == tree->root) {
            tree->root = RightRotate(grandparent);
        }
        else {
            RightRotate(grandparent);
        }
    }
    else if(a == 2) {
        if(grandparent == tree->root) {
            tree->root = LeftRotate(grandparent);
        }
        else {
            LeftRotate(grandparent);
        }
    }
    else {
        //do nothing. should never come here
    }

}

//fixes case 2. Case2 occurs when the uncle is black and the parent node and the child node are in differnt positions (left and right, right and left) "zig zag"
//right after case2 is fixed, case3 should be called with the returned node as the case3 fix
//returns the node to operate a case3 fix around
Node * Case2Fix(Node * tracer) {
    
    int position = CheckPosition(tracer);
    if(position == 2) {
        LeftRotate(tracer->parent);
        return tracer->left;
    }
    else if(position == 1) {
        RightRotate(tracer->parent);
        return tracer->right;
    }
    else {
        //do nothing. should never come here
    }
}

//left rotatation.
//returns the new "heighest" node in the rotation
Node * LeftRotate(Node * tracer) {

    Node * x = tracer->right;
    Node * y = tracer;

    //if who we are rotating around has a parent, insert the soon to be "head" node in the parent's correct position
    int a = CheckPosition(tracer);
    if(a) {
        if(a == 1) {
            tracer->parent->left = x;
        }
        else {
            tracer->parent->right = x;
        }
    }

    x->parent = y->parent;
    y->parent = x;
    if(x->left != NULL) {
        x->left->parent = y;
    }
    y->right = x->left;
    x->left = y;
    
    return x;
}

//right rotatation.
//returns the new "heighest" node in the rotation
Node * RightRotate(Node * tracer) {
    Node * x = tracer;
    Node * y = tracer->left;

    //if who we are rotating around has a parent, insert the soon to be "head" node in the parent's correct position
    int a = CheckPosition(tracer);
    if(a) {
        if(a == 1) {
            tracer->parent->left = y;
        }
        else {
            tracer->parent->right = y;
        }
    }
    
    y->parent = x->parent;
    x->parent = y;
    if(y->right != NULL) {
        y->right->parent = x;
    }
    x->left = y->right;
    y->right = x;
    
    return y;
}

//fixes case1. Case1 occurs when the uncle is red
//returns grandparent node
Node * Case1Fix(RBTree * tree, Node * tracer) {

    Node * uncle = getUncle(tracer);
    Node * grandparent = getGrandparent(tracer);

    tracer->parent->color = 0;
    uncle->color = 0;
    grandparent->color = 1;

    return grandparent;

}

//checks which case (two or three) need to be performed
int CheckCase(Node * tracer) {
    int tracerPosition = CheckPosition(tracer);
    int parentPosition = CheckPosition(tracer->parent);
    if(parentPosition == tracerPosition) {
        return 3;
    }
    else {
        return 2;
    }
     

}

//checks the position of the given node
//returns 1 if left child, 2 if right child, and zero if it doesn't have a parent (root)
//precondition: tracer is not NULL
int CheckPosition(Node * tracer) {
    if(tracer->parent != NULL) {
        if(tracer->parent->left == tracer) {
            return 1;
        }
        else {
            return 2;
        }
    }
    else {
        return 0;
    }
}

//returns the grandparent of the given node
Node * getGrandparent(Node * tracer) {
    if(tracer->parent != NULL) {
        return tracer->parent->parent;
    }
    else {
        return NULL;
    }
}

//returns the uncle of the given node.
//predcondition: tracer != tree->root
Node * getUncle(Node * tracer) {
    Node * grandparent = getGrandparent(tracer);
    if(grandparent != NULL) {
        if(grandparent->left == tracer->parent) {
            return grandparent->right;
        }
        else {
            return grandparent->left;
        }
    }
    else {
        return NULL;
    }
}

//inserts a node at the correct spot in a red black tree
void Insert(RBTree * tree, Node * newNode) {
    Node * whereToInsert = FindParent(tree->root, newNode->data);
    newNode->parent = whereToInsert;
    if(newNode->data > whereToInsert->data) {
        whereToInsert->right = newNode;
    }
    else {
        whereToInsert->left = newNode;
    }
}

//traverses a tree to find the parent of specific data. Used for insertion. 
//returns node that would be fit to be the parent of the specified data
Node * FindParent(Node * tracer, int data) {
    if(tracer != NULL) {
        if(data > tracer->data) {
            if(tracer->right != NULL) {
                return FindParent(tracer->right, data);
            }
            else {
                return tracer;
            }
        }
        else{
            if(tracer->left != NULL) {
                return FindParent(tracer->left, data);
            }
            else {
                return tracer;
            }
        }
        
    }
    else {
        return tracer;
    }
}

//creates a node for a RBTree. 
//Returns NULL if malloc failed
Node * makeNode(int data, int color) {
    Node * newNode = malloc(sizeof(Node));
    if(newNode != NULL) {
        newNode->data = data;
        newNode->color = color;
        newNode->parent = NULL;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

/*
* *** Cleanup Functions ***
*/
void freeTree(RBTree * tree) {
    freeNodes(tree->root);
}

void freeNodes(Node * tracer) {
    if(tracer != NULL) {
        freeNodes(tracer->left);
        freeNodes(tracer->right);
        free(tracer);
    }
}
