#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class listNode { 
public:
	friend class linkedListStack;
	friend class linkedQueue;
	friend class RadixSort;

	int data;
	listNode* next;

	listNode(int d){
		data = d;
		next = NULL;
	}

	listNode(){
	}

	~listNode(){
		delete next;
	}

};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class linkedListStack {
public:
	friend class RadixSort;
	listNode* top;

	linkedListStack(){
		top = NULL;
	}

	void push(listNode* newNode){
		//insert new node on top of the stack
		newNode->next = top;
		top = newNode;
	}

	listNode* pop(){
		//pop and return node on the top of the stack
		if(top == NULL)
			return NULL;
		else {
			listNode* temp = top; 
			top = top->next;
			temp->next = NULL; // disattach the node from the list
			return temp;
		}
	}

	bool isEmpty(){
		if(top == NULL)
			return true;
		return false;
	}

	void printStack(ofstream &outFile){
		outFile << "Stack: " << endl;
		listNode* iter = top;
		outFile << "TOP -> ";
		while(iter != NULL){
			if(iter->next == NULL){
				outFile <<"("<<iter->data<<", NULL) -> NULL";
				break;
			}
			outFile <<"("<<iter->data<<", "<<iter->next->data<<") -> ";
			iter = iter->next;
		}
		outFile <<endl;
		outFile <<"=============================="<<endl;
	}

};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class linkedListQueue {
public:
	friend class RadixSort;

	listNode* head;
	listNode* tail;
	listNode* dummy;

	linkedListQueue(){
		dummy = new listNode();
		tail = dummy; 
		head = dummy;
	}

	void addTail(listNode* newNode){
		tail->next = newNode;
		tail = newNode;

	}

	listNode* deleteFront(){
		//deletes and returns the node at the dummy node
		if(dummy->next == tail) // if only one node left
			tail = dummy;	// set tail to pint to dummy
		listNode* temp = head->next;
		head->next = head->next->next;
		temp->next = NULL; // disattach the node from the list
		return temp;
	}

	bool isEmpty(){
		if(head->next == NULL)
			return true;
		return false;
	}

	void printQueue(ofstream &outFile, int index){
		listNode* iter = head->next;
		outFile <<"Front ("<<index<<") -> ";
		while(iter != NULL){
			if(iter->next == NULL){
				outFile <<"("<<iter->data<<", NULL) -> NULL";
				break;
			}
			outFile <<"("<<iter->data<<", "<<iter->next->data<<") -> ";
			iter = iter->next;
		}
		outFile <<endl;
		outFile <<"Tail ("<<index<<") -> "<<"("<<tail->data;
		outFile <<", NULL) -> NULL"<<endl;
		outFile <<"=============================="<<endl;
	}

	
	~linkedListQueue(){
		listNode* iter;
		while(head != tail){
			iter = head->next;
			delete head;
			head = iter; 
		}
		delete head;
	}

};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class RadixSort {
public:
	ofstream outFile[2];
	linkedListStack* stack;
	linkedListQueue** hashTable;
	int data, currentTable, previousTable, tableSize,
		maxDigits, offSet, currentDigit, currentQueue;

	RadixSort(string out1, string out2){
		tableSize = 10;
		outFile[0].open(out1);
		outFile[1].open(out2);
		hashTable = new linkedListQueue*[2];
		hashTable[0] = new linkedListQueue[10];
		hashTable[1] = new linkedListQueue[10];
	}

	void firstReading(ifstream &inFile){
		//determine the maxDigits and offSet
		int negativeNum = 0,
			positiveNum = 0;
		while(inFile >> data){
			if(data < negativeNum)
				negativeNum = data;
			else if(data > positiveNum)
				positiveNum = data;
		}
		offSet = abs(negativeNum);
		positiveNum += offSet;
		maxDigits = getMaxDigits(positiveNum);
	}

	void loadStack(ifstream &inFile){
		stack = new linkedListStack();
		while(inFile >> data){
			data += offSet;
			listNode* node = new listNode(data);
			stack->push(node);
		}
		stack->printStack(outFile[0]);
	}

	void dumpStack(){
		listNode* node;
		int hashIndex;
		while(!stack->isEmpty()){
			node = stack->pop();
			hashIndex = getOneDigit(node);
			hashTable[currentTable][hashIndex].addTail(node);
		}
		printTable(0);
		for(int i = 0; i < 10; i++)
			if(!hashTable[currentTable][i].isEmpty())
				//printing only non-empty queues
				hashTable[currentTable][i].printQueue(outFile[0], i);
		delete stack;
	}

	int getMaxDigits(int num){
		//find out total digits of the given int
		string n = to_string(num);
		num = n.length();
		return num;
	}

	int getOneDigit(listNode* node){
		//returns the current digit of the data in the node
		int digit = node->data;
		for(int i = 0; i < currentDigit; i++){
			digit /= 10;
		}
		return digit%10;
	}

	void sort(){
		listNode* node;
		int hashIndex, temp, digit;
		currentDigit = 0;
		currentTable = 0;
		dumpStack();
		currentDigit++;
		currentTable = 1;
		previousTable = 0;
		currentQueue = 0;
		while(currentDigit <= maxDigits){
			while(currentQueue < tableSize){
				while(!hashTable[previousTable][currentQueue].isEmpty()){
					node = hashTable[previousTable][currentQueue].deleteFront();
					hashIndex = getOneDigit(node);
					hashTable[currentTable][hashIndex].addTail(node);
				}
				printTable(0);
				currentQueue++;
			}
			temp = currentTable;
			currentTable = previousTable;
			previousTable = temp;
			currentQueue = 0;
			++currentDigit;
		}
	}

	void printTable(int fileNum){
		//print only non empty queues in the table
		listNode* iter;
		for(int i = 0; i < 10; i++){
			if(!hashTable[currentTable][i].isEmpty()){
				outFile[fileNum] <<"Table ["<<currentTable<<"]["<<i<<"]: ";
				iter = hashTable[currentTable][i].head->next;
				while(iter != NULL){
					outFile[fileNum] <<iter->data<<", ";
					iter = iter->next;
				}
				outFile[fileNum] << endl;
			}//if
		}//for
		outFile[fileNum] <<"=============================="<<endl;
	}

	void substractOffset(){
		currentTable = previousTable;
		listNode* node = hashTable[currentTable][0].head->next;
		while(node != NULL){
			node->data -= offSet;
			node = node->next;
		}
	}


};

int main(int argc, char** argv){
	if(argc > 2){
		RadixSort* radix = new RadixSort(argv[2], argv[3]);
		ifstream inFile;
		inFile.open(argv[1]);
		radix->firstReading(inFile);
		inFile.close();
		inFile.open(argv[1]);
		radix->loadStack(inFile);
		inFile.close();
		radix->sort();
		radix->printTable(0);
		radix->substractOffset();
		radix->printTable(1);
	}
	return 0;
}
