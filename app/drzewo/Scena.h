#pragma once

#include <list>
#include <vector>
#include<string>
#include<conio.h>
using namespace std;

class TimeProperty{
public:
	TimeProperty(){};
	TimeProperty(string _time,string _value){time=_time;value=_value;};
	string time;
	string value;


};

class Property
{
public:
	Property(vector<TimeProperty> &timeprop,string _name)
	{
		timeproperty = &timeprop;
		name=_name;
	}
	vector<TimeProperty> *timeproperty;
	string name;
};

class NonLinearProperty
{
public:
	NonLinearProperty(string &_value,string _name)
	{
		value=&_value;
		name=_name;
	}
	string *value;
	string name;
};

class Plugin
{
public:
	Plugin(){};

	string name;
	vector<Property> properties;
	vector<NonLinearProperty> non_linear_properties;
};

class BlackNode{
public:
	BlackNode(){};
	
	string name;
	vector<BlackNode> children;
	vector<Plugin*> plugins;

	
};




class PluginGeometry:public Plugin
{
public:
	PluginGeometry()
	{
		properties.push_back(Property(posX,"posX"));
		properties.push_back(Property(posY,"posY"));
		properties.push_back(Property(posZ,"posZ"));
		properties.push_back(Property(rotX,"rotX"));
		properties.push_back(Property(rotY,"rotY"));
		properties.push_back(Property(rotZ,"rotZ"));
		properties.push_back(Property(scaX,"scaX"));
		properties.push_back(Property(scaY,"scaY"));
		properties.push_back(Property(scaZ,"scaZ"));
		name="geometry";
	}

	vector<TimeProperty> posX;
	vector<TimeProperty> posY;
	vector<TimeProperty> posZ;
	vector<TimeProperty> rotX;
	vector<TimeProperty> rotY;
	vector<TimeProperty> rotZ;
	vector<TimeProperty> scaX;
	vector<TimeProperty> scaY;
	vector<TimeProperty> scaZ;
};
class PluginSolid:public Plugin
{
public:
	PluginSolid()
	{
		properties.push_back(Property(R,"RGBA"));
		properties.push_back(Property(R,"R"));
		properties.push_back(Property(G,"G"));
		properties.push_back(Property(B,"B"));
		properties.push_back(Property(A,"A"));
		name="solid";

	}

	vector<TimeProperty> R;
	vector<TimeProperty> G;
	vector<TimeProperty> B;
	vector<TimeProperty> A;
};

class PluginTexture:public Plugin
{
public:
	PluginTexture()
	{
		Path="";
		properties.push_back(Property(posX,"posX"));
		properties.push_back(Property(posY,"posY"));
		properties.push_back(Property(rotX,"rotX"));
		properties.push_back(Property(rotY,"rotY"));
		properties.push_back(Property(scaX,"scaX"));
		properties.push_back(Property(scaY,"scaY"));
		non_linear_properties.push_back(NonLinearProperty(Path,"path"));
		name="texture";

	}

	string Path;
	vector<TimeProperty> posX;
	vector<TimeProperty> posY;
	vector<TimeProperty> rotX;
	vector<TimeProperty> rotY;
	vector<TimeProperty> scaX;
	vector<TimeProperty> scaY;

};

class PluginText:public Plugin
{
public:
	PluginText()
	{
		Text="";
		Size="";
		Font="";
		
		non_linear_properties.push_back(NonLinearProperty(Text,"text"));
		non_linear_properties.push_back(NonLinearProperty(Text,"size"));
		non_linear_properties.push_back(NonLinearProperty(Text,"font"));
		name="text";

	}

	string Text;
	string Size;
	string Font;
	
};

class PluginPrimitive:public Plugin
{
public:
	PluginPrimitive()
	{
		type="";
		properties.push_back(Property(width,"width"));
		properties.push_back(Property(height,"height"));
		non_linear_properties.push_back(NonLinearProperty(type,"type"));
		name="primitive";
	}

	string type;
	vector<TimeProperty> width;
	vector<TimeProperty> height;


};

class PluginExtrude:public Plugin
{
public:
	PluginExtrude()
	{
		
		properties.push_back(Property(depth,"depth"));
		name="extrude";

	}
	vector<TimeProperty> depth;

};

class PluginRadialGradient:public Plugin
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

class PluginLinearGradient:public Plugin
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








class Scena{

public:

	Scena(){MainNode.name = "Root";};
	BlackNode MainNode;
	BlackNode* FindNode(BlackNode *LookFor,string name);
	BlackNode* FindNode(string name){
		return FindNode(&MainNode,name);
	}
};

