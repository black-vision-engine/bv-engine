#pragma once
#include "../base/XMLPlugin.h"

using namespace std;





class PluginRadialGradient:public XMLPlugin
{
public:
	PluginRadialGradient()
	{
		properties.push_back(Property(X,"X"));
		properties.push_back(Property(Y,"Y"));
		properties.push_back(Property(R1,"R1"));
		properties.push_back(Property(G1,"G1"));
		properties.push_back(Property(B1,"B1"));
		properties.push_back(Property(A1,"A1"));
		properties.push_back(Property(R2,"R2"));
		properties.push_back(Property(G2,"G2"));
		properties.push_back(Property(B2,"B2"));
		properties.push_back(Property(A2,"A2"));
		properties.push_back(Property(radius,"radius"));
	

		name="radialGradient";
		type=PLUGIN_TYPE_PIXEL_SHADER;
		
	}
	vector<TimeProperty> X;
	vector<TimeProperty> Y;
	
	vector<TimeProperty> radius;
	vector<TimeProperty> R1;
	vector<TimeProperty> G1;
	vector<TimeProperty> B1;
	vector<TimeProperty> A1;
	vector<TimeProperty> R2;
	vector<TimeProperty> G2;
	vector<TimeProperty> B2;
	vector<TimeProperty> A2;
};