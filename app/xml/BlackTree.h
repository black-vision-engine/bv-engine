#pragma once

#include "pomocnicze\auxilary.hpp"
#include "rapidxml\rapidxml.hpp" //"rapidxml.hpp"
#include "Scena.h"
#include <iostream>
#include <string>
#include <conio.h>


using namespace rapidxml;
using namespace std;


class BlackTree
{
public:
	
	Scena Scene;

	BlackTree()
	{
		////cout<<"BlackTree";
	}
	
	bool LoadFromFile(string path);
	bool ParseXml(xml_document<> &XML);
	bool CreateNode(BlackNode &parrent, xml_node<> *NewNode);
	bool CreatePlugin(BlackNode &node, xml_node<> *NewNode);
	bool ParsePlugins(BlackNode &node, xml_node<> *NewNode);
	bool ParsePluginProperties(Plugin *plugin, xml_node<> *NewNode);
	
	vector<TimeProperty>* FindProperty(Plugin *plugin,string name);
	string* FindNonLinearProperty(Plugin *plugin,string name);
	PluginPrimitive *CreatePluginPrimitive(xml_node<> *NewNode);
	PluginGeometry* CreatePluginGeometry(xml_node<> *NewNode);
	PluginTexture* CreatePluginTexture(xml_node<> *NewNode);
	PluginText* CreatePluginText(xml_node<> *NewNode);
	PluginSolid* CreatePluginSolid(xml_node<> *NewNode);
	PluginExtrude* CreatePluginExtrude(xml_node<> *NewNode);
	PluginRadialGradient* CreatePluginRadialGradient(xml_node<> *NewNode);
	PluginLinearGradient* CreatePluginLinearGradient(xml_node<> *NewNode);
};




