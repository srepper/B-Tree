#pragma once
#include "BTree.h"
#include <time.h>

int invalidInteger(std::string input);
void processInput(std::string input, BTree *bTree);
bool checkFileExists(std::string filename);
bool checkInteger(int val);
bool checkOrder(int newOrder);
void buildTreeFromFile(std::string fname, BTree *bTree);
bool verifyOverwrite();