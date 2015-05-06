#pragma once
#include <list>
#include <vector>
#include<string>
#include<conio.h>

class PluginLinearGradient:public XMLPlugin
{
public:
	PluginLinearGradient()
	{
		properties.push_back(Property(X1,"X1"));
		properties.push_back(Property(Y1,"Y1"));
		properties.push_back(Property(X2,"X2"));
		properties.push_back(Property(Y2,"Y2"));
		properties.push_back(Property(R1,"R1"));
		properties.push_back(Property(G1,"G1"));
		properties.push_back(Property(B1,"B1"));
		properties.push_back(Property(A1,"A1"));
		properties.push_back(Property(R2,"R2"));
		properties.push_back(Property(G2,"G2"));
		properties.push_back(Property(B2,"B2"));
		properties.push_back(Property(A2,"A2"));
		
		
		name="linearGradient";
		type=PLUGIN_TYPE_PIXEL_SHADER;
		
	}
	vector<TimeProperty> X1;
	vector<TimeProperty> Y1;
	vector<TimeProperty> Y2;
	vector<TimeProperty> X2;
	vector<TimeProperty> R1;
	vector<TimeProperty> G1;
	vector<TimeProperty> B1;
	vector<TimeProperty> A1;
	vector<TimeProperty> R2;
	vector<TimeProperty> G2;
	vector<TimeProperty> B2;
	vector<TimeProperty> A2;
	
	
	
};
