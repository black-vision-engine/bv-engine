#pragma once

#include <list>
#include <vector>
#include<string>

using namespace std;

class NonLinearProperty
{
public:
	NonLinearProperty(string &_value,string _name)
	{
		value=&_value;
		name=_name;
	}
	NonLinearProperty(wstring &_value,string _name)
	{
		valueWide=&_value;
		name=_name;
	}
	string *value;
	wstring *valueWide;
	string name;
};