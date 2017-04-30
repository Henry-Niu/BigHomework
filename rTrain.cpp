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

typedef MyMap<QueryInformation> TMAP;
typedef MyMap<int> UMAP;


/** nh
  * title是一个字符串数组，表示输入的表头，list表示的是每个表头对应的信息
  * 要做的事把火车加到二进制文件里面
*/
void addTrain(string *title, int titleLen, string *list, int listLen) {
	
}

/** 
  * 本函数已完成
  * 把车次添加到mp里面 
*/
void buildTrain(char *inFile, map<QueryInformation, int> &mp) {
	ifstream inTree(inFile);
	int size;
	inTree.read(reinterpret_cast<char *> (&size), sizeof(int)); //the number of trains.
	
	Train nod;
	for (int i = 0; i < size; ++i) {
		inTree.read(reinterpret_cast<char *> (&nod), sizeof(nod));
		insert(nod);
	}
}

/** wtl
  * 修改票数，先找到在文件里的位置，读入车，修改，输出到文件
*/
void modifyTicket(const string &start, const string &target, const string &trainNum, const Date &date, TMAP &trainMap) {
	
}

/** wtl
  * 插入
*/
void insert(const Train &nod, TMAP &trainMap) {
	
}

/**
  * 删除
*/
void erase(const Train &nod, TMAP &trainMap) {
	
}

/**
  * 修改
*/
void modify() {

}

