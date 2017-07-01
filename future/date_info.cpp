#include "date_info.h"

date_info::date_info()
{
	string current_product;
	ifstream marks("days.txt");
	string line_in_marks;
	while (getline(marks, line_in_marks))
	{
		for (int i = 0; i < line_in_marks.size(); i++)
		{
			if (line_in_marks[i] == '"' || line_in_marks[i] == ',' || line_in_marks[i] == '}' || line_in_marks[i] == ':')
				line_in_marks[i] = ' ';
		}
		istringstream marks_stringstream(line_in_marks);
		string chunk_in_stringstream;
		map<string, string> temp_map;
		while (marks_stringstream >> chunk_in_stringstream)
		{
			if (chunk_in_stringstream[0] == '2')
			{
				date temp_date(chunk_in_stringstream);
				dates.push_back(temp_date);
				break;
			}
			if (chunk_in_stringstream == "WTI" || chunk_in_stringstream == "BRENT")
			{
				current_product = chunk_in_stringstream;
				break;
			}
			string detail;
			marks_stringstream >> detail;
			temp_map[chunk_in_stringstream] = detail;

		}
		if (temp_map.size() == 0)
			continue;
		if (current_product == "WTI")
		{
			dates[dates.size() - 1].WTI.push_back(temp_map);
			continue;
		}
		else
		{
			dates[dates.size() - 1].BRENT.push_back(temp_map);
			continue;
		}
	}
}