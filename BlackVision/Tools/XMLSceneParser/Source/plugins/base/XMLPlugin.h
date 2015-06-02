#pragma once

#include <list>
#include <vector>
#include<string>
#include "Property.h"
#include "NonLinearProperty.h"
#include "TimeProperty.h"

using namespace std;

class XMLPlugin
{
public:
	XMLPlugin(){};
	PLUGIN_TYPE type;
	string name;
    string timeline;
	vector<Property> properties;
	vector<NonLinearProperty> non_linear_properties;
    virtual void DefaultPluginProperties(){};
};