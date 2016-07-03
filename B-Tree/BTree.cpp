#include "BTree.h"

BTree::BTree()
{
	order = 3;
	root = new Node(order);
}

BTree::BTree(int newOrder)
{
	order = newOrder;
	root = new Node(order);
}

void BTree::buildTreeFromFile(std::vector<int> values)
{
	root = root->bulkload(values);
}


Node* BTree::getRoot()
{
	return root;
}

void BTree::deleteTree()
{
	root->deleteSubtree();
	delete root;
	root = new Node();
}

void BTree::generateTree()
{
	if(!root->isEmpty())
	{
		deleteTree();
	}

	int numValues = rand() % (order * 10) + (order * 4);
	if(numValues > 999)
	{
		numValues = 999;
	}

	for(int i = 0; i < numValues; i++)
	{
		insert(rand() % 999 + 1);
	}
}

bool BTree::insert(int number)
{
	if(!root->insert(number))
	{
		return false;
	}

	if(!root->isRoot())
	{
		setRoot();
	}

	return true;
}

void BTree::newTree(int newOrder)
{
	order = newOrder;
	root->deleteSubtree();
	delete root;
	root = new Node(order);
}

void BTree::print()
{
	if(root->isEmpty())
	{
		std::cout << "\nTree contains no values.\n\n";
	}
	else
	{
		std::cout << "\nThe tree contains the following values:  \n";
		root->printValues();
		std::cout << "\n";
	
		std::cout << "\nContents of tree in directory structure format: \n";
		root->printSubtree("", true);
		std::cout << "\n\n";
	}
}

void BTree::removeFromTree(int number)
{
	if(root->isEmpty())
	{
		std::cout << "\nTree is empty; cannot remove number (" << number << ").\n\n";
		return;
	}

	Node* leafNode = root->removeFromTree(number);
	if(leafNode)
	{
		root = leafNode->rebalance();
	}
}

void BTree::saveToFile(std::string filename)
{
	std::ofstream file(filename);
	file << order << ":";
	root->writeToFile(file);
	file.close();
}

void BTree::setRoot()
{
	while(root->hasParent())
	{
		root = root->getParent();
	}
}

BTree::~BTree(){}