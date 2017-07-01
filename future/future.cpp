#include"date_info.h"


string re_str(string ss,char replaced_char);
string get_date(string period);
double get_standard_price(string period, vector<map<string, string>>& more_details);
void update_info(string& lines, int& trade_numb,char& trade_type,
	string& future_product, string& period, double& user_price, double& quantity);





int main()
{
	date_info days;

	string user_date;
	cout << "today date: ";
	bool date_exist = false;
	while (cin >> user_date)
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

	string file_name1 = "C:\\Users\\Air\\Desktop\\摩根\\摩根项目\\Future Trade Sample_Commodities.csv";
	ifstream is1(file_name1);
	getline(is1, lines);

	while (getline(is1, lines))
	{
		string replace;

		int trade_numb;//1000001
		char trade_type;//S B
		string future_product;//WTI
		string period;//17-jul

		double user_price;
		double quantity;
		double standard_price;

		double balance;//cha e 

		update_info(lines, trade_numb, trade_type, future_product, period, user_price, quantity);   //7 parameters 

		date contract_date;
		vector<map<string, string>> more_details;
		for (int i = 0; i < days.dates.size(); i++)
		{
			if (days.dates[i].the_date == user_date)
			{
				if (future_product == "WTI")
					more_details = days.dates[i].WTI;
				else
					more_details = days.dates[i].BRENT;
			}
		}

		

		if (trade_type == 'B')
		{
			period = get_date(period);
			standard_price = get_standard_price(period,more_details);
			balance = standard_price - user_price;
		}
		else if (trade_type == 'S')
		{
			period = get_date(period);
			standard_price = get_standard_price(period, more_details);
			balance = user_price - standard_price;
		}
		cout << "The order number is : " << trade_numb << endl;
		cout << "The profit is : " << balance*quantity<<endl;
		cout<<endl;
	}
	is1.close();

	keep_window_open();
	return 0;
}

string re_str(string ss,char replaced_char)
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
string get_date(string period)
{
	string str1;
	str1 = re_str(period, '-');
	string date;
	int year;
	string string_year;
	istringstream is(str1);
	is >> year;
	if (!is)
	{
		is.clear();
		is >> date;
		is >> string_year;
	}
	else
	{
		is >> date;
		stringstream int_to_string;
		int_to_string << year;
		int_to_string >> string_year;
	}
	return date+'-'+string_year;//standard jun-17
}
double get_standard_price(string period, vector<map<string, string>>& more_details)
{
	for (int i = 0; i < more_details.size(); i++)
	{
		if (more_details[i]["ContractMonth"] == period)
		{
			stringstream string_price(more_details[i]["Mark"]);
			double double_price;
			string_price >> double_price;
			return double_price;
		}
	}
}
void update_info(string& lines,int& trade_numb,char& trade_type,string& future_product,string& period,double& user_price, double& quantity)
{
	string replace;
	replace = re_str(lines, ',');
	istringstream is(replace);
	is >> trade_numb;
	is >> trade_type;
	is >> future_product;
	is >> period;
	is >> user_price;
	is >> quantity;
	quantity = quantity * 2500;
}