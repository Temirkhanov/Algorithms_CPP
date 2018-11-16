#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class DijkstraSSS{
public:	
	int numNodes;
	int** costMatrix;
	int *fatherAry, *markedAry, *bestCostAry;
	ofstream outFile, outFile2;

	DijkstraSSS(){
		
	}

	void init(){
		costMatrix = new int*[numNodes+1];
		for(int i = 0; i <= numNodes; i++){
			costMatrix[i] = new int[numNodes+1];
			for(int j = 0; j <= numNodes; j++)
				costMatrix[i][j] = 9999;
			costMatrix[i][i] = 0;
		}
		fatherAry = new int[numNodes+1];
		markedAry = new int[numNodes+1];
		bestCostAry = new int[numNodes+1];
		for(int i = 0; i <= numNodes; i++){
			fatherAry[i] = i;
			markedAry[i] = 0;
			bestCostAry[i] = 9999;
		}
	}

	void loadCostMatrix(ifstream &inFile){
		inFile >> numNodes;
		init();
		int i, j, cost;
		while(inFile >> i){
			inFile >> j;
			inFile >> cost;
			costMatrix[i][j] = cost;
		}
		inFile.close();
	}

	void setBestCostAry(int sourceNode){
		//copy the row of source node from costMatrix
		for(int i = 1; i <= numNodes; i++)
			bestCostAry[i] = costMatrix[sourceNode][i];
	}

	void setFatherAry(int sourceNode){
		//set all to source
		for(int i = 1; i <= numNodes; i++)
			fatherAry[i] = sourceNode;
	}

	void setMarkedAry(){
		//set all to 0
		for(int i = 1; i <= numNodes; i++)
			markedAry[i] = 0;
	}

	void markMinNode(int minNode){
		//set markedAry[minNode] to 1
		markedAry[minNode] = 1;
	}

	void changeFather(int node, int minNode){
		// set  fatherAry[node] to  minNode 
		fatherAry[node] = minNode;
	}

	void changeCost(int node, int newCost){
		// set bestCostAry[node] to newCost 
		bestCostAry[node] = newCost;
	}

	int findMinNode(int sourceNode){
		int minNode = 0; 
		int minC = 9999;
		for(int i = 1; i <= numNodes; i ++){
			if(markedAry[i] == 0 && bestCostAry[i] <= minC){
				minNode = i;
				minC = bestCostAry[i];
			}
		}
		return minNode;
	}

	int findUnmarkedNode(int node){
		for(int i = 1; i <= numNodes; i++)
			if(costMatrix[node][i] <= 9999 && markedAry[i] == 0)
				return i;
		return -1;
	}

	bool nodesMarked(int node){
		for(int i = 1; i <= numNodes; i++){
			if(markedAry[i] == 0)
				return false;
		}
		return true;
	}

	int computeCost(int minNode, int currentNode){
		return bestCostAry[minNode] + costMatrix[minNode][currentNode];
	}

	void debugPrint(int sourceNode){
		outFile2 <<"Source Node: "<<sourceNode<<endl;
		outFile2 <<"Father Array: ";
		for(int i = 1; i <= numNodes; i++){
			outFile2 <<fatherAry[i]<<", ";
		}
		outFile2 <<endl;
		outFile2 <<"Best Cost Array: ";
		for(int i = 1; i <= numNodes; i++){
			outFile2 <<bestCostAry[i]<<", ";
		}
		outFile2 <<endl;
		outFile2 <<"Marked Array: ";
		for(int i = 1; i <= numNodes; i++){
			outFile2 <<markedAry[i]<<", ";
		}
		outFile2 <<endl;
		outFile2 <<"===================================================="<<endl;
	}

	void printShortestPath(int currentNode, int sourceNode){
		// trace from currentNode back to sourceNode (via fatherAry),
		// and print the shortest path from sourceNode to currentNode  
		// with the cost to output-1 (with proper heading)

		outFile << "The path from "<<sourceNode<<" to "<<currentNode<<": "<<currentNode;
		int fatherNode = fatherAry[currentNode];
		int childNode = currentNode;
		while(fatherNode != sourceNode){
			outFile<<" <- "<<fatherNode;
			childNode = fatherNode;
			fatherNode = fatherAry[childNode];
		}
		outFile<<" <- "<< sourceNode<<" cost = "<<bestCostAry[currentNode];
		outFile<<endl;
		
		
	}

	~DijkstraSSS(){
		for(int i = 0; i <= numNodes; i++)
			delete[] costMatrix[i];
		delete[] costMatrix;
		delete[] fatherAry;
		delete[] markedAry;
		delete[] bestCostAry;
	}

};

int main(int argc, char** argv){
	ifstream inFile;
	inFile.open(argv[1]);
	DijkstraSSS* sss = new DijkstraSSS();
	sss->outFile.open(argv[2]);
	sss->outFile2.open(argv[3]);
	sss->loadCostMatrix(inFile);
	int numNodes = sss->numNodes;
	int currentNode, newCost, minNode, sourceNode = 1;
	sss->outFile<<"==============================="<<endl;
	sss->outFile<<"There are "<<numNodes<<" nodes in the graph"<<endl;
	sss->outFile<<"==============================="<<endl;
	sss->outFile<<endl;
	while(sourceNode <= numNodes){
		sss->setBestCostAry(sourceNode);
		sss->setFatherAry(sourceNode);
		sss->setMarkedAry();

		while(!sss->nodesMarked(sourceNode)){
			minNode = sss->findMinNode(sourceNode);
			sss->markMinNode(minNode);
			sss->debugPrint(sourceNode);

			//expanding the minNode
			//int numKids = sss->getNumKids(minNode)
			currentNode = 1;
			while(currentNode <= numNodes){
				if(sss->markedAry[currentNode] == 0){
					newCost = sss->computeCost(minNode, currentNode);
					if(newCost < sss->bestCostAry[currentNode]){
						sss->changeCost(currentNode, newCost);
						sss->changeFather(currentNode, minNode);
						sss->debugPrint(sourceNode);
					}
				}
				currentNode++;
			}
		}
		sss->outFile<<"Source Node is "<<sourceNode<<endl;
		for(int currentN = 1; currentN <= numNodes; currentN++)
			sss->printShortestPath(currentN, sourceNode);
		sss->outFile<<"================================================="<<endl;
		sss->outFile<<endl;

		sourceNode++;
	}
	sss->outFile.close();
	sss->outFile2.close();

}