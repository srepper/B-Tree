#include "mainProgram.h"
#include <string>

int main(int argc, char *args[])
{
	srand(time(NULL));
	std::string input;
	bool exit = false;
	BTree *bTree = new BTree();
	
	while(!exit)
	{
		std::cout << "Please input a selection:\n";
		std::cout << "\t1)  Add a number\n";
		std::cout << "\t2)  Remove a number\n";
		std::cout << "\t3)  Print tree\n";
		std::cout << "\t4)  Remove all elements from current tree\n";
		std::cout << "\t5)  Generate new random tree (destroys current tree in memory)\n";
		std::cout << "\t6)  Change order of tree (destroys current tree in memory)\n";
		std::cout << "\t7)  Read tree from file "
			 << "(destroys current tree in memory)\n";
		std::cout << "\t8)  Save current tree to file\n";
		std::cout << "\t0)  Exit program\n";

		std::getline(std::cin, input);
		if(input.compare("0") == 0)
		{
			exit = true;
		}
		else
		{
			processInput(input, bTree);
		}
	}
	
	return 0;
}

void processInput(std::string input, BTree *bTree)
{
	int val;
	switch(atoi(input.c_str()))
	{
	case 1:
		{
			int attempts = 0;
			while(attempts < 3)
			{
				std::cout << "\nInput integer (1-999)\n";
				std::getline(std::cin, input);
				val = atoi(input.c_str());

				if(!checkInteger(val))
				{
					++attempts;
					continue;
				}

				if(bTree->insert(val))
				{
					std::cout << "Value (" << val <<") successfully inserted.\n\n";
				}
				else
				{
					std::cout << "Number already exists in tree (" << val << ")\n\n";
				}
				
				break;
			}

			if(attempts == 3)
			{
				std::cout << "\nInsertion aborted.\n\n";
			}
		}
		break;
	case 2:
		{
			int attempts = 0;
			while(attempts < 3)
			{
				std::cout << "\nInput integer (1-999)\n";
				std::getline(std::cin, input);
				val = atoi(input.c_str());

				if(!checkInteger(val))
				{
					++attempts;
					continue;
				}

				bTree->removeFromTree(val);
				std::cout << "Value (" << val << ") successfully removed.\n\n";
				break;
			}

			if(attempts == 3)
			{
				std::cout << "\nRemoval aborted.\n\n";
			}
		}
		break;
	case 3:
		{
			bTree->print();
		}
		break;
	case 4:
		{
			bTree->deleteTree();
			std::cout << "\nAll values removed from tree.\n\n";
		}
		break;
	case 5:
		{
			bTree->generateTree();
			std::cout << "\nA random tree has been generated.\n\n";
		}
		break;
	case 6:
		{
			int attempts = 0;
			while(attempts < 3)
			{
				std::cout << "\nInput new order (number of children):  ";
				std::getline(std::cin, input);
				val = atoi(input.c_str());
				
				if(!checkOrder(val))
				{
					++attempts;
					continue;
				}

				bTree->newTree(val);
				std::cout << "\nA new tree of order " << val << " has been created.  "
					<< "Tree is currently empty.\n\n";
				break;
				
			}
			if(attempts == 3)
			{
				std::cout << "\nOperation aborted - previous tree remains intact.\n\n";
			}
		}
		break;
	case 7:
		{
			int attempts = 0;
			while(attempts < 3)
			{
				std::string filename;
				std::cout << "Enter file to read:  ";
				std::getline(std::cin, filename);
				if(filename.size() > 4)
				{
					std::string extension = filename.substr(filename.size() - 4);
					if(extension != ".btr" && extension != ".txt")
					{
						std::cout << "\nInvalid file extension.\n\n";
						++attempts;
						continue;
					}
				}
				if(checkFileExists(filename))
				{
					buildTreeFromFile(filename, bTree);
				}
				else
				{
					std::cout<<"\nFile not found.\n\n";
					++attempts;
					continue;
				}
				break;
			}

			if(attempts == 3)
			{
				std::cout << "File open aborted.\n\n";
			}
		}
		break;
	case 8:
		{
			if(bTree->getRoot()->isEmpty())
			{
				std::cout << "\nTree contains no values - file save aborted.\n\n";
				break;
			}
			int attempts = 0;
			while(attempts < 3)
			{
				std::string filename;
				std::cout << "Please name the file:  ";
				std::getline(std::cin, filename);
				std::string extension = filename.substr(filename.size() - 4);
				
				if(checkFileExists(filename) || checkFileExists(filename + ".btr"))
				{
					if(!verifyOverwrite())
					{
						++attempts;
						continue;
					}
				}
						
				if(extension == "")
				{
					filename = filename + ".btr";
					std::cout << "\nFile will be saved as " << filename;
				}
				else if(extension != ".btr" && extension != ".txt")
				{
					std::cout << "\nInvalid file extension.\n\n";
					++attempts;
					continue;
				}
				
				bTree->saveToFile("..\\Saved Trees\\" + filename);
				std::cout << "\nSuccessfully saved " << filename << "\n\n";
				break;
			}

			if(attempts == 3)
			{
				std::cout << "\nFile open aborted.\n\n";
			}

		}
		break;
	case 9:
		{
			/*  TESTING FOR KEY REMOVAL
			for(int i = 1; i < 1000; ++i)
			{
				if(i == 999)
					int x = 1;
				bTree->removeFromTree(i);
			}
			*/

			/* TESTING FOR KEY INSERTION AND/OR HIGH ORDER TREES
			
			for(int i = 1; i < 1000; ++i)
			{
				bTree->insert(i);
			}
			*/
		}
		break;
	default:
		std::cout << "Invalid input.\n\n";
	}
}

bool checkFileExists(std::string filename)
{
	std::ifstream file("..\\Saved Trees\\" + filename);
	if(file)
	{
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool checkInteger(int val)
{
	if(val < 1 || val > 999)
	{
		std::cout << "Value out of range.\n";
		return false;
	}

	return true;
}

bool checkOrder(int newOrder)
{
	if(newOrder < 3)
	{
		std::cout << "\nOrder must be 3 or higher.\n\n";
		return false;
	}
				
	else if(newOrder > 500)
	{
		std::cout << "\nOrder is too high (root node would never split).\n\n";
		return false;
	}
	else if(newOrder > 32)
	{
		std::cout << "\nOrder is impractically high - depth will never exceed 1." <<
			"  Continue? (y/n)  ";
		std::string input;
		std::getline(std::cin, input);
		if(input != "y" && input != "Y")
		{
			return false;
		}

		return true;
	}
	
	return true;
}

void buildTreeFromFile(std::string filename, BTree *bTree)
{
	filename = "..\\Saved Trees\\" + filename;
	std::ifstream file(filename);
	
	std::string order_str;
	std::getline(file, order_str, ':');
	int order_int = atoi(order_str.c_str());
	if(!checkOrder(order_int))
	{
		std::cout << "Build process aborted.\n\n";
		return;
	}

	bTree->newTree(order_int);

	std::vector<int> values;
	int nextValue;
	while(!file.eof())
	{
		if(!(file >> nextValue))
		{
			if(file.eof())
			{
				continue;
			}
			std::cout << "\nFile contains invalid data - build process aborted.\n\n";
			return;
		}
		if(!checkInteger(nextValue))
		{
			std::cout << "\nOnly integers from 1 to 999 are accepted - build process aborted.\n\n";
			return;
		}
		if(!values.empty())
		{
			if(nextValue < values.back())
			{
				std::cout << "\nValues should be sorted (low to high) - build process aborted.\n\n";
				return;
			}
			else if(nextValue == values.back())
			{
				std::cout << "\nTree cannot contain duplicate values - build process aborted.\n\n";
				return;
			}
		}
		values.push_back(nextValue);
	}

	bTree->buildTreeFromFile(values);
}

bool verifyOverwrite()
{
	std::cout << "File with this name already exists. Overwrite? (y/n)\n";
	std::string input;
	std::getline(std::cin, input);
	if(input != "y" && input != "Y")
	{
		return false;
	}

	return true;
}
