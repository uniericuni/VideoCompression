#include "huffman.h"

//------------- NODE -------------//
NODE::NODE(char v, int n) { 
	val = v;
	num = n;
	parent 	= nullptr; 
	r_child = nullptr; 
	l_child = nullptr;
	p_found = false;
}

char NODE::getVal() 	{ return val; }
int  NODE::getNum() 	{ return num; }
void NODE::setVal(char v){ val  = v; }
void NODE::setNum(int n){ num  = n; }
void NODE::parentFound(NODE* p){ parent = p; p_found = true; }

//------------- NODE -------------//

//----------- HUFFTREE -----------//

// pulic functions

HUFFTREE::HUFFTREE(){}

HUFFTREE::HUFFTREE(FREQTABLE table){
	bit_str = 0;
	count = 0;
	root = new NODE(40, 0);
	freq_table = table;
	freq_table.insert(pair<char,int>(END, 1));
	buildTree();
	buildTable();
}

HUFFTREE::HUFFTREE(HUFFTABLE table){
	huff_table = table;
	bit_str = 0;
	count = 0;
}

HUFFTREE::~HUFFTREE(){
	while(!node_list.empty()){
		node_list.pop_back();
	}
	//cout << "system: All nodes has been popped out ..." << endl;
	//cout << "system: Hufftree has been cut down ..." << endl << endl;
}

void HUFFTREE::buildTree(){
	// push value into node list
	
	FREQTABLE::iterator it;
	for(it = freq_table.begin(); it != freq_table.end(); ++it){
		NODE* node = new NODE(it->first, it->second);
		node_list.push_back(node);
	}

	//cout << "system: All nodes has been push in ..." << endl;
	
	// build tree
	NODELIST::iterator it1, it2;
	int sum, num1, num2;
	NODE* node;

	while(1){
		node = findMinRoot();
		root->l_child = node;
		node->parentFound(root);
		num1 = node->getNum();

		if(findMinRoot()==nullptr){
			root = root->l_child;
			delete root->parent;
			//cout << "system: Total number: " << root->getNum() << endl;
			break;
		}

		node = findMinRoot();
		root->r_child = node;
		node->parentFound(root);
		num2 = node->getNum();

		sum = num1 + num2;
		root->setNum(sum);
		node_list.push_back(root);

		root = new NODE(40, 0);
	}
}

void HUFFTREE::buildTable(){
	NODE* node = root;
	branchSearch("0", node->l_child);
	branchSearch("1", node->r_child);
}

void HUFFTREE::encriptFile(string ifilename, string ofilename){
	
	//cout << huff_table.find(END)->second << endl << endl;

	char temp[1024];
	char val = 0, c_size = 0;
	string code = "";

	FILE *ifile;
	strcpy(temp, ifilename.c_str());
	ifile = fopen(temp, "rb");
	ofstream ofile;
	ofile.open(ofilename);
	if(!ofile)
		cout << "Fail to create " + ofilename + " !" << endl;

	// encript table
	for(FREQTABLE::iterator it = freq_table.begin(); it != freq_table.end(); ++it){
		val = (it->first);
		code = bitset<8>((int)val).to_string();
		encript(code, ofile);
		
		c_size = (huff_table.find(val)->second).size() + '0';
		code = bitset<8>((int)c_size).to_string();
		encript(code, ofile);
		
		code = huff_table.find(val)->second;
		encript(code, ofile);
	}
	val = '0';
	code = bitset<8>((int)val).to_string();
	encript(code, ofile);
	encript(code, ofile);

	// encript file
	while((val=fgetc(ifile)) != EOF){
		code = huff_table.find(val)->second;
		encript(code, ofile);
	}

	// set ending
	code = huff_table.find(END)->second;
	encript(code, ofile);	
	while(count != 0)
		encript("0", ofile);

	fclose(ifile);
	ofile.close();
}

void HUFFTREE::decriptFile(string ifilename, string ofilename){
	char temp[1024];
	char val;
	size_t pos, state = 0, c_size;
	char* ptrEnd;
	string str;
	
	FILE *ifile;
	strcpy(temp, ifilename.c_str());
	ifile = fopen(temp, "rb");

	ofstream ofile;
	ofile.open(ofilename);
	if(!ofile)
		cout << "Fail to create " + FILENAME + ".rlc !" << endl;
	
	while((val=fgetc(ifile)) != EOF){
		str = str + bitset<8>((int)val).to_string();
		if(state == 0){
			state = 1;
			str = str.substr(8);
		}
		else if( state == 1){
			const char* c_str = (str.substr(0, 8)).data();
			long int c_int = strtol(c_str, &ptrEnd, 2);
			c_size = (int)((char)c_int - '0');

			state = 2;
			str = str.substr(8);
		}
		else if( state == 2 && str.size() >= c_size){
			state = 0;
			str = str.substr(c_size);
		}
		if( c_size == 0)
			break;
	}
	bool end_of_file = false;

	HUFFTABLE::iterator it = huff_table.begin();
	while(!end_of_file){
		val=fgetc(ifile);
		str = str + bitset<8>((int)val).to_string();
		for(it = huff_table.begin(); it != huff_table.end(); ++it){
			if((pos=str.substr(0,it->second.size()).find(it->second)) != string::npos){
				if(it->first==END){
					end_of_file = true;
					break;
				}
				ofile << (it->first);
				str = str.substr(pos+(it->second).size());
				it = huff_table.begin();
			}
		}
	}
}

void HUFFTREE::printFreqTable(){
	cout << "system: Frequency table ..." << endl << endl;
	FREQTABLE::iterator it;
	for(it = freq_table.begin(); it != freq_table.end(); ++it){
		if(it->first == '\n')
			cout << setw(5) << "\\n"  << ": ";
		else if(it->first == END)
			cout << setw(5) << "END"  << ": ";
		else
			cout << setw(5) << it->first  << ": ";
		cout << setw(5) << it->second << " times"<< endl;
	}
	cout << endl;
}

void HUFFTREE::printHuffTable(){
	cout << "system: Huffman table ..." << endl << endl;
	for(HUFFTABLE::iterator it = huff_table.begin(); it != huff_table.end(); ++it) {
		if(it->first == '\n')
			cout << setw(5) << "\\n"  << ": ";
		else if(it->first == END)
			cout << setw(5) << "END"  << ": ";
		
		else
			cout << setw(5) << it->first  << ": ";
		cout << it->second << endl;
	}
	cout << endl;
}

string HUFFTREE::findHuffTable(char val){
	return huff_table.find(val)->second;
}

// private functions

void HUFFTREE::branchSearch(string code, NODE* node){
	if(node->l_child == nullptr && node->r_child == nullptr){
			huff_table.insert(pair<char,string>(node->getVal(), code));
			node = node->parent;
	}
	else{
		branchSearch(code+"1",   node->l_child);
		branchSearch(code+"0", node->r_child);
	}
}

NODE* HUFFTREE::findMinRoot(){
	NODELIST::iterator it = node_list.begin();
	while((*it)->p_found){
		++it;
		if(it == node_list.end()){
				//cout << "system: MinRoot not found ..." << endl;
				return nullptr;
		}
	}
	NODE* min_root = (*it);
	
	for(it = it; it != node_list.end(); ++it){
		while((*it)->p_found){
			++it;
			if(it == node_list.end())
				return min_root;
		}
		if((*it)->getNum() < min_root->getNum())
			min_root = (*it);
	}
	//cout << "system: MinRoot found ..." << endl;
	return min_root;
}

void HUFFTREE::encript(string code, ofstream &ofile){
	for(int i=0; i<code.size(); ++i){
		if(code[i]=='1')
			bit_str = bit_str*2+1;
		else
			bit_str = bit_str*2;
		count++;
		if(count == 8){
			ofile << bit_str;
			bit_str = 0;
			count = 0;
		}
	}
}
//----------- HUFFTREE -----------//
