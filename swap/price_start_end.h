#include"std_lib_facilities.h"

class month_info
{
public:
	string the_month;
	vector<string> st_en;		//'start - end '
	month_info(string month_name) { the_month = month_name; }
};
class price_start_end
{
public:
	vector<month_info> WTI_in_month;
	vector<month_info> BRENT_in_month;
	price_start_end();
}; 
