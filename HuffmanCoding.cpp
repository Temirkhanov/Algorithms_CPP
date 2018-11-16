#include <fstream>
#include <iostream>
using namespace std;

class  listBinTreeNode{
public:
	string code;
	string chStr;
	int prob;
	listBinTreeNode* next;
	listBinTreeNode* left;
	listBinTreeNode* right;

	listBinTreeNode(){
	}

	listBinTreeNode(string ch, int pr){
		chStr = ch;
		prob = pr; 
	}

	bool hasNext(){
		if(next != NULL)
			return true;
		else return false;
	}

	bool isLeaf(){
		if(left == NULL && right == NULL)
			return true;
		return false;
	}

	void printNode(ostream &out){
		out << "(" << chStr << ", " << prob << ", " << code << ", ";
		if(next == NULL)
			out << "NULL, ";
		else
			out << next->chStr << ", ";
		if(left == NULL)
			out << "NULL, ";
		else
			out << left->chStr << ", "; 
		if(right == NULL)
			out << "NULL)   ";
		else
			out << right->chStr << ")   ";
		out << endl;
	}
};

class HuffmanLListTree {
public:
	ofstream outFile1, outFile2, outFile3, outFile4;
	listBinTreeNode* listHead;
	listBinTreeNode* root;
	HuffmanLListTree() {
	}

	HuffmanLListTree(string in, string o1, string o2, string o3, string o4){
		listHead = new listBinTreeNode("dummy", 0);
		ifstream fin;
        fin.open(in);
		outFile1.open(o1);
		outFile2.open(o2);
		outFile3.open(o3);
		outFile4.open(o4);
		constructLinkedList(fin);
	}

	~HuffmanLListTree(){
		outFile1.flush();
		outFile2.flush();
		outFile3.flush();
		outFile4.flush();

	}

	//step 2
	void constructLinkedList(ifstream &inFile){
		string c;
		int p;
		listBinTreeNode* newNode;
		listBinTreeNode* spot;
		while(inFile >> c){
			inFile >> p;
			newNode = new listBinTreeNode(c, p);
			spot = findSpot(p);
			insertNewNode(spot, newNode);
			printList(outFile1);
			if(inFile.eof())
				break;
		}
		inFile.close();
	}

	void printList(ostream &outFile){
		listBinTreeNode* node;
		node = listHead;
		outFile << "listHead --> ";
		while(node->next != NULL){
			outFile << "(" << node->chStr << ", " << node->prob 
						<< ", " << node->next->chStr << ")" << " --> ";
			node = node->next;
		}
		outFile << "(" << node->chStr << ", " << node->prob 
					<< ", " << "NULL" << ")" << " --> " << "NULL";
		outFile << endl;
	}

	listBinTreeNode* findSpot(int p){
		listBinTreeNode* spot = listHead;
		while(spot->next != NULL && spot->next->prob < p)
			spot = spot->next;
		return spot;
	}

	void insertNewNode(listBinTreeNode *spot, listBinTreeNode *newNode){
		newNode->next = spot->next;
		spot->next = newNode;
	}

	void constructBinTree(){
		listBinTreeNode* spot;
		listBinTreeNode* newNode;
		listHead = listHead->next;
		string ch = "";
		int pr;
		while(listHead->next != NULL){
			ch = listHead->chStr + listHead->next->chStr;
			pr = listHead->prob + listHead->next->prob;
			newNode = new listBinTreeNode(ch, pr);
			newNode->left = listHead;
			newNode->right = listHead->next;
			newNode->printNode(outFile2);
			spot = findSpot(newNode->prob);
			insertNewNode(spot, newNode);
			if(listHead->next->next == NULL){
				listHead = listHead->next;
				printList(outFile2);
				break;
			}
			listHead = listHead->next->next;
			printList(outFile2);
		}//while
		root = listHead;
	}

	void constructCode(listBinTreeNode *node, string c){
		if(node == NULL)
			cout << "Empty tree...";
		else if(node->isLeaf()){
			node->code = c;
			outFile3 << node->chStr << ", " << node->code;
			outFile3 << endl;
		}
		else{
			constructCode(node->left, c + "0");
			constructCode(node->right, c + "1");
		}
	}	

	void preOrder(listBinTreeNode *n){
		if(n == root) 
			outFile4 << "Pre-Order:" << endl;
		if(n == NULL);
		else {
			n->printNode(outFile4);
			preOrder(n->left);
			preOrder(n->right);
		}
	}

	void inOrder(listBinTreeNode *n){
		if(n == root) 
			outFile4 << "In-Order:" << endl;
		if(n == NULL){}
		else {
			inOrder(n->left);
			n->printNode(outFile4);
			inOrder(n->right);
		}
	}

	void postOrder(listBinTreeNode *n){
		if(n == root) 
			outFile4 << "Post-Order:" << endl;
		if(n == NULL){}
		else {
			postOrder(n->left);
			postOrder(n->right);
			n->printNode(outFile4);
		}
	}
};

int main(int argc, char** argv){

	if(argc > 4){
		HuffmanLListTree* hc = new HuffmanLListTree(argv[1], argv[2], argv[3], argv[4], argv[5]);
		hc->constructBinTree();
		hc->constructCode(hc->root, "");

		hc->preOrder(hc->root);
		hc->outFile4 << endl;
		hc->inOrder(hc->root);
		hc->outFile4 << endl;
		hc->postOrder(hc->root);
		hc->outFile4 << endl;
	}
	return 0;
}