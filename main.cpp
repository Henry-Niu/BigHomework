#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <map>
using namespace std;

#include "classSet.hpp"
#include "trainClass.hpp"
#include "userClass.hpp"

const char TrainFile[] = "Train.inf"
const char UserFile[] = "User.inf"
const char TrainTree[] = "TrainTree.config"
const char UserTree[] = "UserTree.config"

typedef MyMap<QueryInformation> TMAP;
typedef MyMap<int> UMAP;

void readUpdFile(char *inFile);
void readOptFile(char *inFile);

void inTrainTree(char *inFile, TMAP &mp);
void inUserTree(char *infile, User &mp);
void outTrainTree(char *outFile, const TMAP &mp);
void outUserTree(char *outFile, const UMAP &mp);

int divideString(string inString, string *res, char divSymbol) {
	istringstream sin(inString);
	int len = 0;
	for (len = 0; getline(sin, res[len], divSymbol); ++len);
	return len;
}

int main(int argc, char *argv[]) {

	TMAP trainMap;
	UMAP userMap;
	
	if (argc == 1 || strcmp(argv[1], "--help") == 0) {
		cout << "-upd	To input the initiative train information" << '\n';
		cout << "-opt	To test the operation of tickets for the passangers" << '\n';
	}
	else if (strcmp(argv[1], "-upd") == 0) {
		readUpdFile(argv[2]);
		buildTrainTree(TrainFile, trainMap);
		outTrainTree(TrainTree, trainMap);
	}
	else if (strcmp(argv[1], "-opt") == 0) {
		inTrainTree(TrainTree, trainMap);
		inUserTree(UserTree, userMap);
		readOptFile(argv[2], trainMap, userMap);
		outUserTree(UserTree, userMap);
	}
	else {
		cout << "Sorry, there is no operation named " << argv[1] << ".\n";
	}
	
}

void readUpdFile(char *inFile) {
	ifstream Uin(inFile);
	string trg, trc, tmp, title[15], list[110][15];
	do {
		if (!getline(Uin, trg, '/')) break;
		getline(Uin, trc);
		do {
			getline(Uin, tmp);
			int n = divideString(tmp, title), len = 0;
			
			while (true) {
				getline(Uin, tmp);
				int m = divideString(tmp, list[len], ',');
				for (int j = 0; j < m; ++j) {
					if (list[len++][j] == "终到站") break;
				}
			}
			
			addTrain(title, n, list, len); //add a train with the lable title and station list
		
		} while (true);
		
	}while (true);
}

void readOptFile(char *inFile,TMAP &trainMap, UMAP &userMap) {
	ifstream Oin(inFile);
	
	string tmp, list[14];
	
	while (getline(Oin, tmp)) {
		int len = divideString(tmp, list, ' ');
		ticketOpt(list, trainMap, userMap, UserFile);
	}
}

/** 
  * 本函数已完成，把文件中的user信息读入并加到map里面
  * map建立的是id到User.inf存储位置的映射
*/
void buildUser(char *inFile, TMAP &mp) {
	ifstream inTree(inFile);
	int size;
	inTree.read(reinterpret_cast<char*> (&size), sizeof(int));
	
	User nod;
	for (int i = 0; i < size; ++i) {
		inTree.read(reinterpret_cast<char *> (&nod), sizeof(nod));
		mp[nod.getID()] = i;
	}
}

void inTrainTree(char *inFile, TMAP &mp) {
	ifstream fin(inFile);
	mp.inTree(fin);
}

void inUserTree(char *inFile, UMAP &mp) {
	ifstream fin(inFile);
	mp.inTree(fin);
}

void outTrainTree(char *outFile, const TMAP &mp) {
	ofstream fout(outFile);
	mp.outTree(fout);
}
void outUserTree(char *outFile, const UMAP &mp) {
	ofstream fout(outFile);
	mp.outTree(fout);
}

