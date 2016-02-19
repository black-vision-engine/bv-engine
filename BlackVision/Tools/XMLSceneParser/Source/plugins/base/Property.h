#pragma once

#include <list>
#include <vector>
#include<string>
#include "TimeProperty.h"

using namespace std;

class Property
{
public:
	Property(vector<TimeProperty> &timeprop,string _name)
	{
		timeproperty = &timeprop;
		name=_name;
        timeline="";
	}
	vector<TimeProperty> *timeproperty;
	string name;
    string timeline;
};