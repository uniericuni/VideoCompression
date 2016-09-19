#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <map>
#include <bitset>
#include "def.h" 
#include "func.h"
using namespace func;

typedef map<char, int> 		FREQTABLE;
typedef map<char, string>	HUFFTABLE;

//------------- NODE -------------//
class NODE{
friend class HUFFTREE;

public:
	NODE(char v, int n);

	char getVal();
	int  getNum();
	void setVal(char v);
	void setNum(int n);
	void parentFound(NODE* p);

private:
	char val;
	int num;
	bool p_found;
	NODE* parent;
	NODE* l_child;
	NODE* r_child;
};
//------------- NODE -------------//

typedef vector<NODE*> 	NODELIST;

//----------- HUFFTREE -----------//
class HUFFTREE{
public:
	HUFFTREE();
	HUFFTREE(FREQTABLE table);
	HUFFTREE(HUFFTABLE table);
	~HUFFTREE();

	void buildTree();
	void buildTable();
	void encriptFile(string ifilename, string ofilename);
	void decriptFile(string ifilename, string ofilename);
	void printFreqTable();
	void printHuffTable();
	string findHuffTable(char val);
	
private:
	NODE* findMinRoot();
	void branchSearch(string code, NODE* node);
	void encript(string code, ofstream &ofile);

	FREQTABLE freq_table;
	HUFFTABLE huff_table;
	NODELIST  node_list;
	NODE*	  root;
	unsigned char bit_str;
	int count;

};
//----------- HUFFTREE -----------//

#endif