#ifndef TREE4DPRINTER_H
#define TREE4DPRINTER_H
#include "Node4D.h"
#include "Tree4D.h"

void printNodeRecursive(Tree4D *tree, int nodeIndex, int depthInTree);
void printNodeRecursive2(Tree4D *tree, int nodeIndex, string indentation);
//void printNodeRecursive3(Tree4D *tree, int nodeIndex, string indentation, bool last);
void printNodeRecursive2ToFile(Tree4D *tree, int nodeIndex, string indentation, ofstream &outputfile);

void printNode(Node4D &node);

void printTree4D(Tree4D *tree);
void printTree4D2(Tree4D *tree);
void printTree4D2ToFile(Tree4D *tree, string filename);

// void printOctree3(Octree *octree);

#endif
