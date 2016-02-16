#ifndef OCTREEPRINTER_H
#define OCTREEPRINTER_H
#include "../Node.h"
#include "../Octree.h"

inline void printNodeRecursive(Octree *octree, int nodeIndex, int depthInOctree)
{
	Node node = octree->nodes[nodeIndex];
	if(node.isLeaf())
	{
		cout << string(depthInOctree, ' ') << "|- Leaf" << endl;
	}else
	{
		cout << string(depthInOctree, ' ') << "|- Node" << endl;
		//for each of its possible children:
		for (int i = 0; i < 8; i++)
		{
			if (node.hasChild(i))
			{
				printNodeRecursive(octree, node.getChildPos(i), depthInOctree + 1);
			}
			else
			{
				cout << string(depthInOctree+1 , ' ') << "|-  /"<< endl;
			}
		}
		cout << endl;
	}
}


inline void printOctree(Octree *octree)
{
	printNodeRecursive(octree, octree->n_nodes - 1, 0);
}

inline void printNode(Node &node)
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
