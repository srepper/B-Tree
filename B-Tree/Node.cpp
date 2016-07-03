#include "Node.h"
#include <limits.h>
#include <algorithm>
#include <string>

int Node::minElements = 0;
int Node::order = 0;

Node::Node()
{
	parent = NULL;
	leftSibling = NULL;
	rightSibling = NULL;
}

Node::Node(int newOrder)
{
	order = newOrder;
	minElements = ceil(order/2.0 - 1);
	parent = NULL;
	leftSibling = NULL;
	rightSibling = NULL;
}

void Node::addChild(Node *childNode)
{
	childNode->setParent(this);
	children.push_back(childNode);
	std::sort(children.begin(), children.end(), less_than);

	int index = 0;
	while(childNode != children[index])
		++index;
	
	if(index != 0)
	{
		children[index]->setSiblingLeft(children[index-1]);
		children[index-1]->setSiblingRight(children[index]);
	}
	else
	{
		children[index]->setSiblingLeft(NULL);
	}

	if(index != children.size() - 1)
	{
		children[index]->setSiblingRight(children[index+1]);
		children[index+1]->setSiblingLeft(children[index]);
	}
	else
	{
		children[index]->setSiblingRight(NULL);
	}
}

Node* Node::bulkload(std::vector<int> values)
{
	std::vector<Node*> workingNodes;
	Node *newNode = new Node(order);

	int i = 0;
	while(i < values.size())
	{
		if(newNode->isFull())
		{
			workingNodes.push_back(newNode);
			newNode = new Node(order);
		}
		newNode->keysInsert(values[i++]);
	}

	if(newNode != workingNodes.back())
	{
		workingNodes.push_back(newNode);
	}

	if(workingNodes.back()->getNumberOfKeys() == order)
	{
		newNode = new Node(order);
		while(newNode->getNumberOfKeys() < minElements)
		{
			newNode->keysInsert(workingNodes.back()->getLastKey());
			workingNodes.back()->removeLastKey();
		}
		workingNodes.push_back(newNode);
	}

	i = workingNodes.size() - 1;
	while(workingNodes[i]->getNumberOfKeys() < minElements + 1)
	{
		while(workingNodes[i]->getNumberOfKeys() < minElements + 1)
		{
			workingNodes[i]->keysInsert(workingNodes[i-1]->getLastKey());
			workingNodes[i-1]->removeLastKey();
		}
		--i;
	}

	int childrenPerNode = order;
	int x;
	while((x = workingNodes.size() % childrenPerNode) < minElements + 1)
	{
		--childrenPerNode;
		if(childrenPerNode == minElements + 1)
			break;
		
	}

	std::vector<Node*> parentNodes;
	std::vector<Node*>::iterator it;
	do
	{
		if(!parentNodes.empty())
		{
			parentNodes.clear();
		}

		while(workingNodes.size() > 1)
		{
			newNode = new Node(order);
			while(newNode->getNumberOfChildren() < childrenPerNode && 
					workingNodes.size() > 1)
			{
				newNode->keysInsert(workingNodes[0]->getLastKey());
				workingNodes[0]->removeLastKey();
				newNode->addChild(workingNodes[0]);
				it = workingNodes.begin();
				workingNodes.erase(it);
			}
			
			parentNodes.push_back(newNode);
		}

		parentNodes.back()->addChild(workingNodes[0]);
		it = workingNodes.begin();
		workingNodes.erase(it);
		workingNodes = parentNodes;
		
	} while(parentNodes.size() > 1);

	return parentNodes[0];
}

void Node::deleteSubtree()
{
	if(hasChildren())
	{
		while(!children.empty())
		{
			children.back()->deleteSubtree();
			delete children.back();
			children.pop_back();
		}
	}
}

Node* Node::findNextSmallerElement() 
{
	if(hasChildren())
	{
		return children.back()->findNextSmallerElement();
	}
	else
	{
		return this;
	}
}

Node *Node::getChild(int index)
{
	if(index < 0 || index > children.size() - 1)
	{
		return NULL;
	}
	else
	{
		return children[index];
	}
}

int Node::getKey(int index)
{
	return keys[index];
}

Node *Node::getFirstChild()
{
	return children[0];
}

Node *Node::getLastChild()
{
	return children.back();
}

int Node::getLastKey()
{
	return keys.back();
}

int Node::getNumberOfChildren()
{
	return children.size();
}

int Node::getNumberOfKeys()
{
	return keys.size();
}

Node *Node::getParent()
{
	return parent;
}

bool Node::hasChildren()
{
	return children.empty() ? false : true;
}

bool Node::hasParent()
{
	return parent == NULL ? false : true;
}

int Node::indexSearch(int number)
{
	int lo = 0;
	int hi = keys.size() - 1;
	int mid = (hi - lo) / 2;
	while(hi > lo)
	{
		if(number > keys[mid])
		{
			lo = mid + 1;
		}
		else if (number < keys[mid])
		{
			hi = mid - 1;
		}
		else
		{
			hi = mid;
			lo = mid;
		}
		
		mid = int(lo + ((hi - lo) / 2.0));
	}

	if(number > keys[mid])
	{
			++mid;
	}

	return mid;
}

bool Node::insert(int number)
{
	if(std::binary_search(keys.begin(), keys.end(), number))
	{
		return false;
	}

	if(hasChildren())
	{
		int index = indexSearch(number);
		return children[index]->insert(number);
	}
	else
	{
		keysInsert(number);

		if(isFull())
		{
			split(NULL);
		}
	}

	return true;
}

bool Node::isEmpty()
{
	return keys.size() == 0 ? true : false;
}

bool Node::isFull()
{
	return (keys.size() == order) ? true : false;
}

bool Node::isRoot()
{
	return hasParent() ? false : true;
}

void Node::keysInsert(int number)
{
	keys.push_back(number);
	std::sort(keys.begin(), keys.end());
}

void Node::mergeNodes()
{
	int index = parent->indexSearch(keys[0]);

	if(rightSibling)
	{
		mergeRightSibling();
	}
	else if(leftSibling)
	{
		mergeLeftSibling();
	}
	else
	{
		std::cout << "\n\nA node has somehow ended up with fewer than the minimum elements.\n\n";
	}
}

void Node::mergeLeftSibling()
{
	int index = parent->indexSearch(leftSibling->getKey(0));
	keysInsert(parent->getKey(index));
	parent->removeKey(index);

	if(hasChildren())
	{
		while(leftSibling->hasChildren())
		{
			addChild(leftSibling->getChild(0));
			leftSibling->removeChild(0);
		}
	}

	while(!leftSibling->isEmpty())
	{
		keysInsert(leftSibling->getKey(0));
		leftSibling->removeKey(0);
	}

	parent->removeChild(index);
	delete leftSibling;
	
	setSiblingLeft(parent->getChild(index - 1));
	if(leftSibling)
	{
		parent->getChild(index - 1)->setSiblingRight(this);
	}

}

void Node::mergeRightSibling()
{
	int index = parent->indexSearch(keys[0]);
	keysInsert(parent->getKey(index));
	parent->removeKey(index);

	if(hasChildren())
	{
		while(rightSibling->hasChildren())
		{
			addChild(rightSibling->getChild(0));
			rightSibling->removeChild(0);
		}
	}

	while(!rightSibling->isEmpty())
	{
		keysInsert(rightSibling->getKey(0));
		rightSibling->removeKey(0);
	}

	parent->removeChild(index + 1);
	delete rightSibling;
	
	setSiblingRight(parent->getChild(index + 1));
	if(rightSibling)
	{
		parent->getChild(index + 1)->setSiblingLeft(this);
	}

}

void Node::printSubtree(std::string prefix, bool isTail)
{
	std::cout << prefix << "|__ ";

	for(int i = 0; i < keys.size(); i++)
	{
		std::cout << keys[i] << "  ";
	}
	std::cout << std::endl;

	if(hasChildren())
	{
		int i = 0;
		while(i < keys.size())
		{
			children[i++]->printSubtree(prefix + (isTail? "    " : "|   "), false);
		}
		children[i]->printSubtree(prefix + (isTail? "    " : "|   "), true);
	}
}

void Node::printValues()
{
	int i = 0;
	if(hasChildren())
	{
		while(i < keys.size())
		{
			/*  TESTING FOR SIBLING ERRORS
			if(i == 0 && children[i]->leftSibling != NULL)
				std::cout << "\n\nChild with key " << children[i]->getKey(0) << 
				" is pointing to " << children[i]->leftSibling->getKey(0) << "\n\n";
			if(i == children.size() - 1 && children[i]->rightSibling != NULL)
				std::cout << "\n\nChild with key " << children[i]->getKey(0) << 
				" is pointing to " << children[i]->rightSibling->getKey(0) << "\n\n";
			*/

			children[i]->printValues();
			std::cout << keys[i++] << " ";
		}

		children[i]->printValues();
	}
	else
	{
		while(i < keys.size())
		{
			std::cout << keys[i++] << " ";
		}
	}
}

Node* Node::rebalance()
{
	if(isRoot())
	{
		return this;
	}
	else if(getNumberOfKeys() >= minElements)
	{
		return parent->rebalance();
	}
	
	if(rightSibling && rightSibling->getNumberOfKeys() > minElements)
	{
		rotateLeft();
	}
	else if(leftSibling && leftSibling->getNumberOfKeys() > minElements)
	{
		rotateRight();
	}
	else
	{
		mergeNodes();
	}

	if(parent->isRoot() && parent->isEmpty())
	{
		delete parent;
		parent = NULL;
		return this;
	}
	else
	{
		return parent->rebalance();
	}
}

void Node::removeChild(int index)
{
	std::vector<Node*>::iterator it = children.begin() + index;
	children.erase(it);
}

Node* Node::removeFromTree(int number)
{
	int index = indexSearch(number);
		
	if(index == keys.size())
	{
		if(this->hasChildren())
		{
			return children[index]->removeFromTree(number);
		}
		else
		{
			std::cout << "Number (" << number << ") does not exist in tree.\n\n";
			return NULL;
		}
	}
	else if(keys[index] == number)
	{
		removeKey(index);
		if(hasChildren())
		{
			Node* leafNode = children[index]->findNextSmallerElement();
			keysInsert(leafNode->getKey(leafNode->getNumberOfKeys() - 1));
			leafNode->removeKey(leafNode->getNumberOfKeys() - 1);
			return leafNode;
		}
		else
		{
			return this;
		}
	}
	else if(this->hasChildren())
	{
		return children[index]->removeFromTree(number);
	}
	else
	{
		std::cout << "Number (" << number << ") does not exist in tree.\n\n";
		return NULL;
	}
}

void Node::removeKey(int index)
{
	std::vector<int>::iterator it = keys.begin() + index;
	keys.erase(it);
}

void Node::removeLastChild()
{
	removeChild(children.size() - 1);
}

void Node::removeLastKey()
{
	keys.pop_back();
}

void Node::rotateLeft()
{
	int index = parent->indexSearch(rightSibling->getKey(0)) - 1;
	keysInsert(parent->getKey(index));
	parent->removeKey(index);
	parent->keysInsert(rightSibling->getKey(0));
	rightSibling->removeKey(0);

	if(hasChildren())
	{
		addChild(rightSibling->getChild(0));
		rightSibling->removeChild(0);
	}
}

void Node::rotateRight()
{
	int index = parent->indexSearch(leftSibling->getKey(0));
	keysInsert(parent->getKey(index));
	parent->removeKey(index);
	parent->keysInsert(leftSibling->getKey(leftSibling->getNumberOfKeys() - 1));
	leftSibling->removeKey(leftSibling->getNumberOfKeys() - 1);

	if(hasChildren())
	{
		addChild(rightSibling->getLastChild());
		rightSibling->removeLastChild();
	}
}

void Node::writeToFile(std::ofstream& file)
{
	int i = 0;
	if(hasChildren())
	{
		while(i < keys.size())
		{
			children[i]->writeToFile(file);
			file << " " << keys[i++];
		}

		children[i]->writeToFile(file);
	}
	else
	{
		while(i < keys.size())
		{
			file << " " << keys[i++];
		}
	}
}

void Node::setParent(Node *parentNode)
{
	parent = parentNode;
}

void Node::setSiblingLeft(Node *l_sibling)
{
	leftSibling = l_sibling;
}

void Node::setSiblingRight(Node *r_sibling)
{
	rightSibling = r_sibling;
}

void Node::split(Node *childNode)
{
	Node *newNode = new Node();

	std::vector<int>::iterator midPtr = keys.begin() + order / 2;
	std::vector<int>::reverse_iterator keysReverse = keys.rbegin();
	int midValue = *midPtr;
	
	for(keysReverse; *keysReverse != *midPtr; ++keysReverse)
	{
		newNode->keysInsert(*keysReverse);
	}
	
	if(childNode)
	{
		if(childNode->getKey(0) < getLastChild()->getKey(0))
		{
			Node *tempNode = children.back();
			children.pop_back();
			addChild(childNode);
			childNode = tempNode;
		}

		newNode->addChild(childNode);		
	}

	int midIndex = order / 2;
	if(this->hasChildren())
	{
		for(int i = children.size() - 1; i > midIndex; --i)
		{
			newNode->addChild(children[i]);
			children.pop_back();
			children.back()->setSiblingRight(NULL);
		}
	}

	for(int i = keys.size() - 1; i >= midIndex; --i)
	{
		keys.pop_back();
	}

	if(parent)
	{
		parent->keysInsert(midValue);

		if(parent->isFull())
		{
			parent->split(newNode);
		}
		else
		{
			parent->addChild(newNode);
		}
	}
	else
	{
		Node *parentNode = new Node();
		parentNode->keysInsert(midValue);
		parentNode->addChild(this);
		parentNode->addChild(newNode);
	}	
}

Node::~Node(){}

/*  Comparison operator to compare Nodes by lowest key*/
bool less_than(Node *left, Node *right)
{
	return left->getKey(0) < right->getKey(0);
}