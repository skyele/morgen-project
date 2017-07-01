#include"holiday.h"

holiday::holiday()
{
	ifstream holiday_file("Holiday.csv");
	string line_in_Holiday;
	while (getline(holiday_file, line_in_Holiday))
	{
		for (int i = 0; i < line_in_Holiday.size(); i++)
			if (line_in_Holiday[i] == ',')
				line_in_Holiday = line_in_Holiday.substr(i + 1);
		string discard;   //"Monday"
		string month;
		string day;
		istringstream holiday_stream(line_in_Holiday);
		holiday_stream >> discard;
		holiday_stream >> month;
		holiday_stream >> day;
		if (month == "January")
			month = '1';
		else if (month == "February")
			month = '2';
		else if (month == "March")
			month = '3';
		else if (month == "April")
			month = '4';
		else if (month == "May")
			month = '5';
		else if (month == "June")
			month = '6';
		else if (month == "July")
			month = '7';
		else if (month == "August")
			month = '8';
		else if (month == "September")
			month = '9';
		else if (month == "October")
			month = '10';
		else if (month == "November")
			month = '11';
		else if (month == "December")
			month = '12';
		if (day == "2018")
			holiday_in_law.push_back(month + "/1/2018");
		else
			holiday_in_law.push_back(month + '/' + day + "/2017");
	}
}
