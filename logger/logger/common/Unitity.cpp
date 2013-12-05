#include "Unitity.h"

void Spliter(string s,vector<string>& v,char steps)
{
	stringstream ss(s);
	string sub_str;
	while(getline(ss,sub_str,steps) )
		v.push_back(sub_str);
}