/*
* Luke Schaefer
* 18186970
* LESDMG
*/

#include "rbconstruct.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    /*
    * 1.) Build tree
    * 2.) Output tree using in-order traversal and specifying color of each node
    * 3.) Output height
    * 4.) Output black height
    * 5.) Output second largest element
    * 6.) Free tree
    */

    RBTree * tree = RBConstruct("numbers.txt");

    PrintRBInOrder(tree);
    PrintHeight(tree);
    PrintBlackHeight(tree);
    PrintSecondBiggest(tree);
    
    freeTree(tree);
    return 0;
}