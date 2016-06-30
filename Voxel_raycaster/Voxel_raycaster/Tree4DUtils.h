#ifndef TREE4DUTILS_H
#define TREE4DUTILS_H

#include "Node4D.h"

enum AmountOfChildren { TWO, EIGHT, SIXTEEN };
enum LongestDimension { SPACE, TIME };


int amountOfChildren_enumToInt(const AmountOfChildren& amountOfChildren);

AmountOfChildren getMaxAmountOfChildren(const Node4D* node, const LongestDimension& longest_dimention);


#endif
