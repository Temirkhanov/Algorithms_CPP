#include <fstream>
#include <iostream>
using namespace std;

class Node { 
public:
	int jobID, jobTime;
	Node *next;

	Node(int id, int time){
		jobID = id;
		jobTime = time;
	}
};

class Scheduling {
public:

	int numNodes, totalJobTimes, procGiven;
	int** adjMatrix, scheduleTable;
	int* jobTimeAry, processJob, processTime, 
		parentCount, jobDone, jobMarked;
	Node *open = new Node(0, 0);

	Scheduling(){
	}

	~Scheduling(){
		delete open;
		delete[] processJob;
		delete[] jobMarked;
		delete[] jobDone;
		delete[] parentCount;
		delete[] processTime;
		delete[] jobTimeAry;
		for(int i = 0; i < procGiven; i++)
			delete[] scheduleTable[i];
		delete[] scheduleTable;
		for(int i = 0; i <= numNodes; i++)
			delete[] adjMatrix[i];
		delete[] adjMatrix;

	}

	void init(){
		scheduleTable = new int*[procGiven];
		for(int i = 0; i < procGiven; i++)
			scheduleTable[i] = new [totalJobTimes+1];
		processJob = new int[procGiven];
		processTime = new int[procGiven];
		jobDone = new int[numNodes+1];
		jobMarked = new int[numNodes+1];
	}

	void loadMatrix(ifstream &inFile){
		inFile >> numNodes;
		adjMatrix = new int*[numNodes+1];
		for(int i = 0; i <= numNodes; i++)
			adjMatrix[i] = new int[numNodes+1];
		parentCount = new int[numNodes+1];
		int i, j;
		while(inFile >> i){
			inFile >> j;
			adjMatrix[i][j] = 1;
			parentCount[j]++;
		}
	}

	void computeTotalJobTimes(ifstream &inFile){
		jobTimeAry = new int[numNodes+1];
		totalJobTimes = 0;
		int i, t;
		inFile >> i;
		while(inFile >> i){
			inFile >> t;
			totalJobTimes += t;
			jobTimeAry[i] = t; 
		}
	}

	int getUnMarkOrphen(){
		for(int i = 1; i <= numNodes; i++)
			if(jobMarked[i] == 0 && parentCount[i] == 0)
				return i;
		return -1;
	}

	void insertOpen(Node *node){
		Node *spot = open;
		while(spot->next != NULL && spot->next->next->jobTime < node->jobTime)
			spot = spot->next;
		node->next = spot->next;
		spot->next = node;
	}

	int findProcessor() {
		for(int i = 1; i < procGiven; i++)
			if(processTime[i] <= 0)
				return i;
		return -1;
	}

	void deleteNode(int id) {
		jobDone[id] = 1;
	}

	void printList(){
		Node *node = open->next;
		cout<<endl;
		cout<<"OPEN: ";
		while(node != NULL){
			cout <<"("<<node->jobID<<", "<<node->jobTime<<")"<<" -> ";
			node = node->next;
		}
		cout<<endl;
	}

	void updateTable(int availProc, Node *newJob, int currentTime) {
		if(newJob->jobTime == 1)
			scheduleTable[availProc][currentTime] = newJob->jobID;
		else {
			int t = newJob->jobTime;
			while(t != 0) {
				scheduleTable[availProc][currentTime++] = newJob->jobID;
				--t;
			}
		}//else
	}

	int findDoneJob(int cTime) {
		cTime--;
		for(int i = 1; i < procGiven; i++)
			if(processTime[i] == 0) {      
				if(jobDone[scheduleTable[i][cTime]] != 1) {
					jobDone[scheduleTable[i][cTime]] = 1;	   	
					processJob[i] = 0;				
					return scheduleTable[i][cTime];
				}//else
			}//if
		return -1;
	}


	void printTable(ofstream &outFile){
		outFile << " "<<"-"<<0;
		for(int i = 1; i<= totalJobTimes; i++){
			if(i > 9)
				outFile << "---" << i;
			else 
				outFile << "----" << i;
		}
		outFile << endl;
		for(int i = 1; i < scheduleTable.length(); i++) {
			outFile << "P(" << i << ")" << "|";
			for(int j = 0; j < scheduleTable[i].length(); j++) {
				if(scheduleTable[i][j] == 0)
					outFile<<" -  |";
				else {
					if(scheduleTable[i][j]/10 < 1)
							outFile<<" "<<scheduleTable[i][j]<<" "<<" |";
					else 
						outFile <<" "<<scheduleTable[i][j]<<" |";
				}
			}
			outFile<<endl;
			outFile<<"--------------------------------------------------------------------------";
			outFile<<"--------------------------------------------------------------------------";
			outFile<<endl;
		}
		outFile<<endl;
	}

	int checkCycle() {
		boolean allNodesDone = true;
		boolean allProcFree = true;
		for(int i = 1; i <= numNodes; i++)
			if(jobDone[i] == 0)
				allNodesDone = false;
		for(int i = 1; i < procGiven; i++)
			if(processTime[i] > 0)
				allProcFree = false;
		
		if(open->next == null && !allNodesDone && allProcFree)
			return 1;
		return 0;
	}

	void deleteEdge(int id){
		for(int kid = 1; kid <= numNodes; kid++)
			if(adjMatrix[id][kid] > 0)
				parentCount[kid] = 0;
	}

};

	void debuggingPrint(Scheduling s, int c){
		int n = s.numNodes;
		cout<<"\n *******************************"<<endl;;
		cout<<"Current Time: " << c <<endl;
		cout<<"Jobs Marked: ";
		for(int i = 1; i <= n; i++)
			if(s.jobMarked[i] == 1)
				cout<<i << ", ";
		cout<<"\nProcessing Time: ";
		for(int i = 1; i < s.procGiven; i++)
			cout<<"P(" << i << ") = " << s.processTime[i] << ", ";
		cout<<"\nProcessing Jobs: ";
		for(int i = 1; i < s.procGiven; i++)
			if(s.processJob[i] != 0)
				cout<<"P(" << i << ")->" << s.processJob[i] << ", ";
		cout<<"\nDone Jobs: ";
		for(int i = 1; i <= n; i++)
			if(s.jobDone[i] == 1)
				cout<<i << ", ";
		cout<<"\n *******************************"<<endl;
	}
	

int main(argc, char**argv){

	ifstream inFile1, inFile2;
	ofstream outFile;
	inFile1.open(argv[1]);
	inFile2.open(argv[2]);
	outFile.open(argv[3]);

	Scheduling schedules = new Scheduling();
	schedules.loadMatrix(inFile1);
	schedules.computeTotalJobTimes(inFile2);

	int procNum;
	cout<<"Enter number of processors: ";
	cin >> procNum;
	if(procNum <= 0)
		cout<<"Invalid number of processors!";
	if(procNum > schedules.numNodes)
		procNum = schedules.numNodes;
	procNum++;
	schedules.procGiven = procNum;
	schedules.init();
	int procUsed = 0, currentTime = 0;
	boolean allJobsAreDone = false;

	while(!allJobsAreDone) {
		while(true) {
			int orphenNode = schedules.getUnMarkOrphen();
			if(orphenNode == -1) break;
			schedules.jobMarked[orphenNode] = 1;
			Node *node = new Node(orphenNode, schedules.jobTimeAry[orphenNode]);
			schedules.insertOpen(node);
		}
		schedules.printList();
				
		while(schedules.open->next != null && procUsed < schedules.procGiven) {
			int availProc = schedules.findProcessor();
			if(availProc > 0) {
				procUsed++;
				Node *newJob = schedules.open->next;
				schedules.open->next = schedules.open->next->next;
				newJob->next = null;
				schedules.processJob[availProc] = newJob->jobID;
				schedules.processTime[availProc] = newJob->jobTime;
				schedules.updateTable(availProc, newJob, currentTime);
			}//if
			else if(availProc == -1) break;
		}

		if(schedules.checkCycle() >= 1) {
			cout<<"Cycle...";
			inFile1.close();
			inFile2.close();
			outFile.close();
			return;
		}
		
		schedules.printTable(outFile);
		currentTime++;
				
		int count = 0;
		for(int i = 1; i < schedules.procGiven; i++)
			if(schedules.processTime[i] == 1)
				count++;
		procUsed = count;
				
		for(int i = 1; i < schedules.procGiven; i++)
			schedules.processTime[i]--;
				
		int job = schedules.findDoneJob(currentTime);
		while(job != -1) {
			schedules.deleteNode(job);
			schedules.deleteEdge(job);
			job = schedules.findDoneJob(currentTime);
		}
		debuggingPrint(schedules, currentTime);
				
		allJobsAreDone = true;
		for(int i = 1; i <= schedules.numNodes; i++)
			if(schedules.jobDone[i] == 0)
				allJobsAreDone = false;
			//if(currentTime == 5) break;
	}//while

	schedules.printTable(outFile);
	inFile1.close();
	inFile2.close();
	outFile.close();

	return 0;
}