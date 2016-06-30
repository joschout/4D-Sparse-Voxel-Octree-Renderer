#ifndef TREE4DPRINTER_H
#define TREE4DPRINTER_H
#include "Node4D.h"
#include "Tree4D.h"
#include "Tree4DUtils.h"

void printNodeRecursive(Tree4D *tree, int nodeIndex, int depthInTree);
void printNodeRecursive2(Tree4D *tree, int nodeIndex, string indentation);
//void printNodeRecursive3(Tree4D *tree, int nodeIndex, string indentation, bool last);
void printNodeRecursive2ToFile(Tree4D *tree, int nodeIndex, string indentation, ofstream &outputfile);
void printNodeRecursive2ToFile_alternate(Tree4D *tree, Node4D* node, string indentation, ofstream &outputfile);
void printNodeRecursive2ToFile_alternate_different_sides(Tree4D *tree, Node4D* node, string indentation, int number, int maxNbOfChildrenOfParent, LongestDimension longest_dimension, bool isRoot, ofstream &outputfile);


void printNode(Node4D &node);

void printTree4D(Tree4D *tree);
void printTree4D2(Tree4D *tree);
void printTree4D2ToFile(Tree4D *tree, string filename);
void printTree4D2ToFile_alternate(Tree4D *tree, string filename);

void printTree4D2ToFile_alternate_different_sides(Tree4D *tree, string filename);

void printTree4D_DataToFile(Tree4D *tree, string filename);

// void printOctree3(Octree *octree);

#endif
