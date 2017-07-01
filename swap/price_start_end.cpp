#include"price_start_end.h"

string re_str(string ss, char replaced_char);		
price_start_end::price_start_end()
{
	ifstream price_start_end_file("ExpiryDetails.csv");
	string line_in_price_file;
	getline(price_start_end_file, line_in_price_file);
	while (getline(price_start_end_file, line_in_price_file))
	{
		string product;
		string contract_month;
		string first_trading_date;
		string last_trading_date;
		line_in_price_file = re_str(line_in_price_file, ',');
		istringstream line_in_file_stream(line_in_price_file);
		line_in_file_stream >> product;
		line_in_file_stream >> contract_month;
		line_in_file_stream >> first_trading_date;
		line_in_file_stream >> last_trading_date;

		month_info temp_month_info(contract_month);
		temp_month_info.st_en.push_back(first_trading_date);
		temp_month_info.st_en.push_back(last_trading_date);
		if (product == "WTI")
			WTI_in_month.push_back(temp_month_info);
		else
			BRENT_in_month.push_back(temp_month_info);
	}
}