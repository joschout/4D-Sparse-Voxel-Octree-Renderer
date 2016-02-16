#ifndef OCTREEPRINTER_H
#define OCTREEPRINTER_H
#include "../Node.h"

void printNode(Node &node)
{
	if(node.isLeaf())
	{
		cout << "Leaf Node"<< endl <<endl;
	}
	else {
		cout << "Node: " << endl;
		cout << "   children's base address:" << node.children_base << endl;
		cout << "   children's base offsets:" << endl;
		for (int i = 0; i < 8; i++)
		{
			if(node.hasChild(i))
			{
				cout << "   "<< i << ": child at ofset: " << +node.children_offset[i] 
					<< " (full address: " << node.getChildPos(i) << ")" << endl;
			}else
			{
				cout << "   "<< i << ": /" << endl;
			}
		}
		cout << endl;
	}
}

#endif
