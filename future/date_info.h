#include "std_lib_facilities.h"
#include <map>

class date {	//date.the_date=20170515: information in 20170515
public:
	string the_date;

	vector<map<string, string>> WTI;
	vector<map<string, string>> BRENT;
	date(string date) { the_date = date; }
	date() { the_date = ""; }
};

class date_info {
public:
	vector<date> dates;
	date_info();
};
