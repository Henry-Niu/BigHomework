#ifndef __classSet__
#define __classSet__
	
class Time {
	int year, month, day, hour, min;
	friend bool operator < (const Time &a, const Time &b) {
		return (a.year == b.year ?
			    a.month == b.month?
			    a.day == b.day ?
			    a.hour == b.hour ?
			    a.min < b.min :
			    a.hour < b.hour :
			    a.day < b.day :
			    a.month < b.month :
			    a.year < b.year);
	}
};

class QueryInformation {
	string st, ed, tr;
	QueryInformation() {}
	QueryInformation(char *a, char *b, char *c) {
		st = a; ed = b; tr = c;
	}
	QueryInformation(string _st, string _ed, string _tr): 
		st(_st), ed(_ed), tr(_tr) {}
	friend bool operator < (const QueryInformation &a, const QueryInformation &b) {
		return a.st == b.st ? a.ed == b.ed ? a.tr < b.tr : a.ed < b.ed : a.st < b.st;
	}
};


/** 
  * xj
*/
class Ticket {
	
}

/** 
  * xj
*/
class User {

private:


public:

	void getID();
	void modifyName();
	void modifyPswd();
	
};

/** 
  * wtl
*/
class Train {
	
};

#endif
