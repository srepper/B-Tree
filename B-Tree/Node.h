#pragma once
#include <vector>
#include <iostream>
#include <fstream>

class Node
{
	std::vector<int> keys;
	std::vector<Node*> children;
	Node *parent;
	Node *leftSibling;
	Node *rightSibling;
	

	void addChild(Node *childNode);
	Node* findNextSmallerElement();
	Node* getChild(int index);
	Node* getFirstChild();
	Node* getLastChild();
	int getLastKey();
	int getNumberOfChildren();
	bool hasChildren();
	int indexSearch(int number);
	bool isFull();
	void keysInsert(int number);
	void mergeNodes();
	void mergeLeftSibling();
	void mergeRightSibling();
	void removeChild(int index);
	void removeKey(int index);
	void removeLastChild();
	void removeLastKey();
	void rotateLeft();
	void rotateRight();
	void setSiblingLeft(Node *l_sibling);
	void setSiblingRight(Node *r_sibling);
	void split(Node *childNode);

public:
	static int order;
	static int minElements;

	Node();
	Node(int newOrder);
	Node* bulkload(std::vector<int> values);
	void deleteSubtree();
	int getKey(int index);
	int getNumberOfKeys();
	Node* getParent();
	bool hasParent();
	bool insert(int number);
	bool isEmpty();
	bool isRoot();
	void printSubtree(std::string prefix, bool isTail);
	void printValues();
	Node* rebalance();
	Node* removeFromTree(int number);
	void setParent(Node *parentNode);
	void writeToFile(std::ofstream& file);
	~Node();
};

bool less_than(Node *left, Node *right);