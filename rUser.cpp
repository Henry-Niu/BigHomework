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
  * 本函数要完成买票退票操作，需要判一下如果用户id在map里面没有，要加一个进去(文件、userMap)
  * list是输入文件里的一条记录，关键信息位置已标出，enum里面的元素可直接当成数组下标
  * 涉及到火车修改， 可调用如下函数：
  * void modifyTicket(string start, string target, string id, const Date &date);
*/
void ticketOpt(string *list, const TMAP &trainMap, UMAP &userMap, char *userFile) {
	enum userList{UsrName, UsrId, UsrOpt, TicketNum, TicketClass, TrainId = 7, Start = 9, Target = 11, Date = 13};
	
}
