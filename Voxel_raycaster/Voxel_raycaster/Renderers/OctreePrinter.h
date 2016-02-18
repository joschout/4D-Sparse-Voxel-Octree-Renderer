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

/*
inline void printNodeRecursive3(Octree *octree, int nodeIndex, string indentation, bool last)
{

	Node node = octree->nodes[nodeIndex];
	if(node.isLeaf())
	{
		cout << indentation << "Leaf";
	}else
	{
		cout << indentation << "Node";
	}

	string lastIndent = indentation + "  ";
	string normalIndent = indentation + "|-";
	if(last)
	{
		cout << indentation << "\-";
		indentation += "  ";
	}
	else
	{
		cout << indentation <<"|-";
		indentation += "| ";
	}
	cout << name << endl;
	for (int i = 0; i < 8; i++)
	{
		if(i == 7)
		{
			tempIndentation = indentation + "   "
		}
		if (node.hasChild(i))
		{
			printNodeRecursive3(octree, node.getChildPos(i), indentation, i == 7);
		}
		else
		{
			cout << indentation << " /" << endl;
		}
	}


}*/


inline void printNodeRecursive2(Octree *octree, int nodeIndex, string indentation)
{
	Node node = octree->nodes[nodeIndex];
	if (node.isLeaf())
	{
		cout << indentation << "Leaf" << endl;
	}
	else
	{
		cout << indentation << "Node" << endl;
		//for each of its possible children:
		string tempindentation;
		for (int i = 0; i < 8; i++)
		{
			if( i == 7)
			{
				tempindentation = indentation + "  ";
				if (node.hasChild(i))
				{
					printNodeRecursive2(octree, node.getChildPos(i), tempindentation);
				}
				else
				{
					cout << tempindentation << " /" << endl;
				}

			}else{
				tempindentation = indentation + "|-";
				if (node.hasChild(i))
				{
					printNodeRecursive2(octree, node.getChildPos(i), tempindentation);
				}
				else
				{
					cout << tempindentation << " /" << endl;
				}
			}

		}
		cout << endl;
	}
}


inline void printOctree(Octree *octree)
{
	printNodeRecursive(octree, octree->n_nodes - 1, 0);
}

inline void printOctree2(Octree *octree)
{
	printNodeRecursive2(octree, octree->n_nodes - 1, "");
}
/*inline void printOctree3(Octree *octree)
{
	printNodeRecursive3(octree, octree->n_nodes - 1, "", true);
}*/

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
