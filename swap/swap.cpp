#include"date.h"
#include"holiday.h"
#include"price_start_end.h"
class date;
class date_info;
class holiday;
class month_info;
class date_info;

string re_str(string ss, char replaced_char);		//this function used to process lines; replace char like';' '{' ',' with replace
bool is_holiday(string date, holiday holidays);		//check whether if holiday or not
void update_info(string& lines, int& trade_numb, string &Counterparty, char& trade_type, double&fixe_price, string& float_index, double& quantity, string&UOM, string& pricing_start, string& pricing_end);
string get_standard_day1(int day, int month, int year);//  form:20170516
string get_standard_day2(int day, int month, int year);//  form:5/17/2017
string transform_month(int month);	//12 - december;
double stringtoint(string ss);		//transform string to int

int main()
{
	date_info days;
	holiday holidays;
	price_start_end forwards;

	string user_date;
	cout << "today date: ";
	bool date_exist = false;
	while (cin >> user_date)			//form: 20170515
	{
		for (int i = 0; i < days.dates.size(); i++)
		{
			if (days.dates[i].the_date == user_date)
				date_exist = true;
		}
		if (!date_exist)
		{
			cout << "date information not found" << endl;
			cout << "today date: ";
		}
		else
			break;
	}

	string lines;

	string file_name1 = "Swap Trade Sample_Commodities.csv";
	ifstream is1(file_name1);
	getline(is1, lines);

	while (getline(is1, lines))		//get trade infomation
	{
		string replace;

		int trade_numb;//1000001
		string Counterparty;//company A
		char trade_type;//S B
		double fixed_price;//50  user price
		string float_index;//WTI1nb
		double quantity;//22000
		string UOM;//bbl
		string pricing_start;// 5/1/2017
		string pricing_end;  // 5/31/2017

		double standard_price=0;

		update_info(lines, trade_numb, Counterparty, trade_type, fixed_price, float_index, quantity, UOM, pricing_start, pricing_end);   //10 parameters 
		int contractmonth;
		int prosperity;
		int start_day;
		int end_day;
		int year;
		string temp_start = re_str(pricing_start, '/');
		stringstream stringtoint1(temp_start);
		stringtoint1 >> contractmonth;
		stringtoint1 >> start_day;
		stringtoint1 >> year;
		prosperity = contractmonth;
		string temp_end = re_str(pricing_end, '/');
		stringstream stringtoint2(temp_end);
		stringtoint2 >> end_day >> end_day;

		date contract_date;
		vector<map<string, string>> more_details;

		for (int i = 0; i < days.dates.size(); i++)
		{
			if (days.dates[i].the_date == user_date)
			{
				if (float_index == "WTI")
					more_details = days.dates[i].WTI;
				else
					more_details = days.dates[i].BRENT;
			}
		}
		int current_day = start_day;		//loop
		int day_amounts=0;
		string enr_month = transform_month(contractmonth);
		double this_month_mark=0;
		double next_month_mark=0;
		for (int i = 0; i < more_details.size(); i++) {
			string standard_date2 = get_standard_day1(current_day, contractmonth, year);//transfer form
			if (more_details[i]["Date"] == standard_date2) {
				if (more_details[i]["Type"] == "SettlementPrice")	//settlement
					prosperity++;
			}
			enr_month = transform_month(prosperity);
			if (more_details[i]["Type"] == "Forward") {			//forward
				if (more_details[i]["ContractMonth"] == enr_month.substr(0, 3) + '-' + "17")
				{
					this_month_mark = stringtoint(more_details[i]["Mark"]);
					if (contractmonth != 12)
						next_month_mark = stringtoint(more_details[i + 1]["Mark"]);
				}
			}
		}
		string current_month_forward;
		if (float_index == "WTI"){
			for (int i = 0; i < forwards.WTI_in_month.size(); i++) {
				if (forwards.WTI_in_month[i].the_month.substr(3) == transform_month(prosperity).substr(0, 3))//watch out!
					current_month_forward = forwards.WTI_in_month[i].st_en[1];
			}
		}
		else if (float_index == "BRENT") {
			for (int i = 0; i < forwards.BRENT_in_month.size(); i++) {
				if (forwards.BRENT_in_month[i].the_month.substr(3) == transform_month(prosperity).substr(0, 3))//watch out!
					current_month_forward = forwards.BRENT_in_month[i].st_en[1];
			}
		}
		current_month_forward = current_month_forward.substr(current_month_forward.size() - 2);//22 20
		int int_month_forwad = stringtoint(current_month_forward);
		while (current_day <= end_day)
		{
			string standard_date1 = get_standard_day1(current_day, contractmonth, year);
			
			if (is_holiday(get_standard_day2(current_day, contractmonth, year), holidays)) {
				current_day++;
				continue;
			}
			bool isexist = false;
			for (int i = 0; i < more_details.size(); i++)
			{
				if (more_details[i]["Date"] == standard_date1) {
					if (more_details[i]["Type"] == "SettlementPrice") {
						string mark = more_details[i]["Mark"];
						istringstream stringtodouble(mark);
						double today_price;
						stringtodouble >> today_price;
						standard_price += today_price;
						isexist = true;
					}		
				}
			}
			if (!isexist) {
				if (current_day <= int_month_forwad)
					standard_price += this_month_mark;
				else
					standard_price += next_month_mark;
			}
			current_day++;
			day_amounts++;
		}
		double floating_price = standard_price / day_amounts;
		cout << "Trade number: " << trade_numb << endl;
		cout<<"floating price: " <<floating_price <<"   ";
		if (trade_type == 'B')
			cout << "profit: " << setprecision(4)<< fixed << (floating_price - fixed_price) *quantity << endl;
		else
			cout << "profit: " << fixed << (fixed_price - floating_price)*quantity << endl;
	}
	keep_window_open();
}
string re_str(string ss, char replaced_char)
{
		string replace;
		for (int i = 0; i < ss.size(); i++)
		{
			if (ss[i] == replaced_char)
			{
				replace = replace + " ";
			}
			else
				replace = replace + ss[i];
		}
		return replace;
}
bool is_holiday(string date, holiday holidays)// 5/1/2017
{
		date = re_str(date, '/');
		stringstream holiday_stream(date);
		int month;
		int day;
		int year;
		holiday_stream >> month;
		holiday_stream >> day;
		holiday_stream >> year;
		if (month == 1)
		{
			month = 13;
			year--;
		}
		if (month == 2)
		{
			month = 14;
			year--;
		}
		int W;
		W = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7+1;
		if (W == 6 || W == 7)
			return true;
		for (int i = 0; i < holidays.holiday_in_law.size(); i++) {
			if (holidays.holiday_in_law[i] == date)
				return true;
		}
		return false;
}
void update_info(string& lines, int& trade_numb, string &Counterparty,char& trade_type,double&fixe_price, string& float_index, double& quantity, string&UOM,string& pricing_start,string& pricing_end)
{
	string replace;
	replace = re_str(lines, ',');
	istringstream is(replace);
	is >> trade_numb;
	is >> Counterparty;
	string temp_counterparty;
	is >> temp_counterparty;
	Counterparty = Counterparty + ' ' + temp_counterparty;
	is >> trade_type;
	is >> fixe_price;
	is >> float_index;
	is >> quantity;
	is >> UOM;
	is >> pricing_start;
	is >> pricing_end;
	quantity = quantity; //* 2500;
	for (int i = 0; i < float_index.size(); i++)
	{
		if (isdigit(float_index[i]))
			float_index = float_index.substr(0, i);
	}
}
string get_standard_day1(int day, int month, int year)//20170515
{
	stringstream temp_stream1;
	temp_stream1 << year;
	string standard_date;
	string temp_std_date;
	temp_stream1 >> standard_date;
	stringstream temp_stream2;
	temp_stream2 <<month;
	temp_stream2 >> temp_std_date;
	if (month < 10)
		temp_std_date = '0' + temp_std_date;
	standard_date += temp_std_date;
	stringstream temp_stream3;
	temp_stream3 <<day;
	temp_stream3 >> temp_std_date;
	if (day < 10)
		temp_std_date = '0' + temp_std_date;
	standard_date += temp_std_date;
	return standard_date;
}
string get_standard_day2(int day, int month, int year)//  5/17/2017
{
	stringstream temp_stream1;
	temp_stream1 << month;
	string standard_date;
	string temp_std_date;
	temp_stream1 >> standard_date;
	stringstream temp_stream2;
	temp_stream2 << day;
	temp_stream2 >> temp_std_date;
	standard_date =standard_date+'/'+ temp_std_date;
	stringstream temp_stream3;
	temp_stream3 << year;
	temp_stream3 >> temp_std_date;
	standard_date = standard_date + '/' + temp_std_date;
	return standard_date;
}
string transform_month(int month)
{
	if (month == 1)
		return  "January";
	else if (month == 2)
		return "February";
	else if (month == 3)
		return "March";
	else if (month == 4)
		return"April";
	else if (month == 5)
		return"May";
	else if (month == 6)
		return"June";
	else if (month == 7)
		return "July";
	else if (month == 8)
		return "August";
	else if (month == 9)
		return "September";
	else if (month == 10)
		return"October";
	else if (month == 11)
		return"November";
	else if (month == 12)
		return"December";
}
double stringtoint(string ss)
{
	stringstream stream(ss);
	double result;
	stream >> result;
	return result;
}