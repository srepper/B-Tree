#pragma once
#include "Node.h"

class BTree
{
	Node *root;
	int order;
	
public:
	BTree();
	BTree(int newOrder);
	void buildTreeFromFile(std::vector<int> values);
	void deleteTree();
	void generateTree();
	Node* getRoot();
	bool insert(int number);
	void newTree(int newOrder);
	void print();
	void removeFromTree(int number);
	void saveToFile(std::string filename);
	void setRoot();
	~BTree();
};