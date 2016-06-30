#include "Tree4DUtils.h"

int amountOfChildren_enumToInt(const AmountOfChildren& amountOfChildren)
{
	switch (amountOfChildren)
	{
	case TWO: return 2;
	case EIGHT: return 8;
	case SIXTEEN: return 16;
	}
	return 16;
}

AmountOfChildren getMaxAmountOfChildren(const Node4D* node, const LongestDimension& longest_dimention)
{

	AmountOfChildren maxAmountOfChildren = SIXTEEN;
	switch (longest_dimention)
	{
	case SPACE:
		if (node->isOctaryNode())
		{
			maxAmountOfChildren = EIGHT;
			return maxAmountOfChildren;
		}
	case TIME:
		if (node->isBinaryNode())
		{
			maxAmountOfChildren = TWO;
			return maxAmountOfChildren;
		}
	}
	return maxAmountOfChildren;
}
