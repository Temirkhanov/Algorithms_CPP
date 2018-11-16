#include <fstream>
#include <iostream> 
#include <math.h>
using namespace std;

struct xyCoord{
public:
	int x, y, label;
	xyCoord(){
	}
};

class Point{
public:
	int x, y, label; 
	Point(){
	}
};

class Kmean{
public:
	int kCluster, numPts, numRows, numCols;
	Point* ptList;
	int changeFlag; 
	xyCoord* kCentroids;
    int** imgAry;

	Kmean(int kCl, string filename){
		ifstream fin;
		fin.open(filename);
		fin >> numRows; 
		fin >> numCols;
		fin >> numPts;
		ptList = new Point[numPts+1];
		kCluster = kCl;
		loadPtList(fin);
		assignLabel();
		printPtList();

		while(changeFlag > 0){
		mapPoint2Image();
		PrettyPrintImgAry();
		changeFlag = 0;
		computeCentroid();
		clusterCheck();
		} 
		fin.close();
	}

	void loadPtList(ifstream fs){
		Point pt;
		for(int i = 0; i < numPts; i++){
			//pt = new Point();
			fs >> pt.x;
			fs >> pt.y;
			ptList[i] = pt;
		}
	}

	void assignLabel(){
		for(int i = 0; i < numPts; i++)
			ptList[i].label = i % kCluster + 1;
	}

	void computeCentroid(){
		kCentroids = new xyCoord[kCluster + 1];
		int* sumCnt = new int[kCluster + 1];
		int* sumX = new int[kCluster + 1]; 
		int* sumY = new int[kCluster + 1];
		int label;
		for(int i = 0; i < numPts; i++){
			label = ptList[i].label;
			sumCnt[label]++;
			sumX[label] + ptList[i].x;
			sumY[label] + ptList[i].y;
		}
		int clusterIndex = 1;
		for(int i = 0; i < kCluster; i++){
			kCentroids[clusterIndex].x = sumX[clusterIndex] / sumCnt[clusterIndex];
			kCentroids[clusterIndex].y = sumY[clusterIndex] / sumCnt[clusterIndex]; 
			clusterIndex++;
		}
	}

	void clusterCheck(){
		double* dist = new double[kCluster + 1];
		int label, minLabel, ptIndex = 0;
		while(ptIndex < numPts){
			label = ptList[ptIndex].label;
			double minDist = 99999.0;
			int clusterIndex = 1;
			while(clusterIndex <= kCluster){
				dist[clusterIndex] = computeDist(ptList[ptIndex], kCentroids[clusterIndex]);
				if(dist[clusterIndex] < minDist){
					minDist = dist[clusterIndex];
					minLabel = clusterIndex;
				}//if
				clusterIndex++;
			}//while inner
			if(ptList[ptIndex].label != minLabel){
				ptList[ptIndex].label = minLabel;
				changeFlag++;
			}//if
		}//while
	}

	double computeDist(Point p1, xyCoord p2){
		return sqrt((p1.x - p2.x) + (p1.y - p2.y));
	}

	void printPtList(){
		for(int i = 0; i < numPts; i++)
			cout<<ptList[i].x<<", "<<ptList[i].y<<", "<<ptList[i].label<<endl;
	}

	void mapPoint2Image(){
		imgAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
			imgAry = new int*[numCols];
		for(int i = 0; i < numPts; i++){
			imgAry[ptList[i].y][ptList[i].x] = ptList[i].label;
		}
	}
	void PrettyPrintImgAry(){
		for(int i = 0; i < numRows; i++){
			for(int j = 0; j < numCols; j++){
				if(imgAry[i][j] > 0)
					cout << imgAry[i][j];
				else cout << ' ';
			}
			cout << endl;
		}
	}
};//Kmean


int main(int argc, char** argv){

	//ifstream inFile;
	int kClust;
	cout << "Enter kCluster: ";
	cin >> kClust;
	/*
	inFile.open(argv[1]);
	inFile >> rows; 
	inFile >> cols;
	inFile >> numP;
  	*/
	Kmean *km = new Kmean(kClust, argv[1]);
	/*
	km.kCluster = kClust;
	km.loadPtList(inFile);
	km.assignLabel();
	km.printPtList();
	while(km.changeFlag > 0){
		km.mapPoint2Image();
		km.PrettyPrintImgAry();
		km.changeFlag = 0;
		km.computeCentroid();
		km.clusterCheck();
	} 
	*/

	return 0;
}