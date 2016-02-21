#include "Tree4DPrinter.h"

 void printNodeRecursive(Tree4D *tree, int nodeIndex, int depthInTree)
{
	Node4D node = tree->nodes[nodeIndex];
	if (node.isLeaf())
	{
		cout << string(depthInTree, ' ') << "|- Leaf" << endl;
	}
	else
	{
		cout << string(depthInTree, ' ') << "|- Node" << endl;
		//for each of its possible children:
		for (int i = 0; i < 16; i++)
		{
			if (node.hasChild(i))
			{
				printNodeRecursive(tree, node.getChildPos(i), depthInTree + 1);
			}
			else
			{
				cout << string(depthInTree + 1, ' ') << "|-  /" << endl;
			}
		}
		cout << endl;
	}
}

void printNodeRecursive2(Tree4D *tree, int nodeIndex, string indentation)
{
	Node4D node = tree->nodes[nodeIndex];
	if (node.isLeaf())
	{
		cout << indentation << "Leaf" << endl;
	}
	else
	{
		cout << indentation << "Node" << endl;
		//for each of its possible children:
		string tempindentation;
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				tempindentation = indentation + "  ";
				if (node.hasChild(i))
				{
					printNodeRecursive2(tree, node.getChildPos(i), tempindentation);
				}
				else
				{
					cout << tempindentation << " /" << endl;
				}

			}
			else {
				tempindentation = indentation + "|-";
				if (node.hasChild(i))
				{
					printNodeRecursive2(tree, node.getChildPos(i), tempindentation);
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

void printNodeRecursive2ToFile(Tree4D *tree, int nodeIndex, string indentation, ofstream &outputfile)
{
	Node4D node = tree->nodes[nodeIndex];
	if (node.isLeaf())
	{
		outputfile << indentation << "Leaf\n";
	}
	else
	{
		outputfile << indentation << "Node\n";
		//for each of its possible children:
		string tempindentation;
		for (int i = 0; i < 16; i++)
		{
			/*if (i == 15)
			{
			tempindentation = indentation + "  ";
			if (node.hasChild(i))
			{
			printNodeRecursive2ToFile(tree, node.getChildPos(i), tempindentation, outputfile);
			}
			else
			{
			outputfile << tempindentation << " /\n";
			}

			}
			else {*/
			tempindentation = indentation + "|-(" + to_string(i) + ")";
			if (node.hasChild(i))
			{
				printNodeRecursive2ToFile(tree, node.getChildPos(i), tempindentation, outputfile);
			}
			else
			{
				outputfile << tempindentation << " /\n";
			}
			//}

		}
		outputfile << "\n";
	}
}
void printNodeRecursive2ToFile_alternate(Tree4D *tree, Node4D* node, string indentation, ofstream &outputfile)
{
	
	if (node->isLeaf())
	{
		outputfile << indentation << "Leaf\n";
	}
	else
	{
		outputfile << indentation << "Node\n";
		//for each of its possible children:
		string tempindentation;
		for (int i = 0; i < 16; i++)
		{
			/*if (i == 15)
			{
			tempindentation = indentation + "  ";
			if (node.hasChild(i))
			{
			printNodeRecursive2ToFile(tree, node.getChildPos(i), tempindentation, outputfile);
			}
			else
			{
			outputfile << tempindentation << " /\n";
			}

			}
			else {*/
			tempindentation = indentation + "|-(" + to_string(i) + ")";
			if (node->hasChild(i))
			{
				printNodeRecursive2ToFile(tree, node->getChildPos(i), tempindentation, outputfile);
			}
			else
			{
				outputfile << tempindentation << " /\n";
			}
			//}

		}
		outputfile << "\n";
	}
}
void printNode(Node4D &node)
{
	if (node.isLeaf())
	{
		cout << "Leaf Node" << endl << endl;
	}
	else {
		cout << "Node: " << endl;
		cout << "   children's base address:" << node.children_base << endl;
		cout << "   children's base offsets:" << endl;
		for (int i = 0; i < 16; i++)
		{
			if (node.hasChild(i))
			{
				cout << "   " << i << ": child at ofset: " << +node.children_offset[i]
					<< " (full address: " << node.getChildPos(i) << ")" << endl;
			}
			else
			{
				cout << "   " << i << ": /" << endl;
			}
		}
		cout << endl;
	}
}

void printDataRecursive2ToFile(Tree4D *tree, int dataIndex, string indentation, ofstream &outputfile)
{
	Node4D node = tree->nodes[dataIndex];
	if (node.isLeaf())
	{
		outputfile << indentation << "Leaf\n";
	}
	else
	{
		outputfile << indentation << "Node\n";
		//for each of its possible children:
		string tempindentation;
		for (int i = 0; i < 16; i++)
		{
			/*if (i == 15)
			{
			tempindentation = indentation + "  ";
			if (node.hasChild(i))
			{
			printNodeRecursive2ToFile(tree, node.getChildPos(i), tempindentation, outputfile);
			}
			else
			{
			outputfile << tempindentation << " /\n";
			}

			}
			else {*/
			tempindentation = indentation + "|-(" + to_string(i) + ")";
			if (node.hasChild(i))
			{
				printNodeRecursive2ToFile(tree, node.getChildPos(i), tempindentation, outputfile);
			}
			else
			{
				outputfile << tempindentation << " /\n";
			}
			//}

		}
		outputfile << "\n";
	}
}

void printTree4D(Tree4D *tree)
{
	printNodeRecursive(tree, tree->n_nodes - 1, 0);
}

void printTree4D2(Tree4D *tree)
{
	printNodeRecursive2(tree, tree->n_nodes - 1, "");
}


void printTree4D2ToFile(Tree4D *tree, string filename)
{
	ofstream outputfile(filename);

	if (outputfile.is_open())
	{
		printNodeRecursive2ToFile(tree, tree->n_nodes - 1, "", outputfile);
		outputfile.close();
	}
	else cout << "Unable to open output file to write tree structure to";
}

void printTree4D2ToFile_alternate(Tree4D *tree, string filename)
{
	ofstream outputfile(filename);

	if (outputfile.is_open())
	{
		Node4D* root = tree->getRootNode();
		printNodeRecursive2ToFile_alternate(tree, root, "", outputfile);
		outputfile.close();
	}
	else cout << "Unable to open output file to write tree structure to";
}

void printTree4D_DataToFile(Tree4D *tree, string filename)
 {
	 ofstream outputfile(filename);

	 if (outputfile.is_open())
	 {
		 printDataRecursive2ToFile(tree, tree->n_data - 1, "", outputfile);
		 outputfile.close();
	 }
	 else cout << "Unable to open output file to write tree structure to";
 }


/*inline void printOctree3(Octree *octree)
{
printNodeRecursive3(octree, octree->n_nodes - 1, "", true);
}*/