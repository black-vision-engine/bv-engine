#pragma once

#include "helpers/auxiliary.hpp"
#include "rapidxml.hpp" //"rapidxml.hpp"
#include "Scena.h"
#include <iostream>
#include <algorithm>  
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
	
	bool                    LoadFromFile                    (string path);
	bool                    ParseXml                        (xml_document<> &XML);

    //timeline stuff 
    bool                    CreateTimeline                  (xml_node<> *NewTimeline);

    // classes stuff
    bool                    CreateClasses                   (xml_node<> *NewClass);
    

    // node stuff
	bool                    CreateNode                      (BlackNode &parrent, xml_node<> *NewNode);
    bool                    CreateNode                      (BlackNode &parrent, xml_node<> *NewNode,string node_name);
	bool                    CreatePlugin                    (BlackNode &node, xml_node<> *NewNode);
	bool                    ParsePlugins                    (BlackNode &node, xml_node<> *NewNode);
	bool                    ParsePluginProperties           (XMLPlugin *plugin, xml_node<> *NewNode);
	
	vector<TimeProperty>*   FindProperty                    (XMLPlugin *plugin,string name);
    Property*               FindPropertyInstance            (XMLPlugin *plugin,string name);
	wstring*                FindNonLinearPropertyWide       (XMLPlugin *plugin,string name);
	string*                 FindNonLinearProperty           (XMLPlugin *plugin,string name);
	PluginRectangle *       CreatePluginRectangle           (xml_node<> *NewNode);
	PluginRing *            CreatePluginRing                (xml_node<> *NewNode);
	PluginTransform*         CreatePluginTransform           (xml_node<> *NewNode);
	PluginTexture*          CreatePluginTexture             (xml_node<> *NewNode);
    PluginInstance*         CreatePluginInstance            (BlackNode* node, xml_node<> *NewNode);
	PluginSequence*         CreatePluginSequence            (xml_node<> *NewNode);
	PluginCrawl*			CreatePluginCrawl	            (xml_node<> *NewNode);
	PluginCounter*			CreatePluginCounter	            (xml_node<> *NewNode);
    PluginMask*             CreatePluginMask                (xml_node<> *NewNode);
	PluginText*             CreatePluginText                (xml_node<> *NewNode);
    PluginTimer*            CreatePluginTimer               (xml_node<> *NewNode);
	PluginPrism*			CreatePluginPrism			 	(xml_node<> *NewNode);
    PluginAlpha*            CreatePluginAlpha               (xml_node<> *NewNode);
	PluginSolid*            CreatePluginSolid               (xml_node<> *NewNode);
	PluginExtrude*          CreatePluginExtrude             (xml_node<> *NewNode);
	PluginRadialGradient*   CreatePluginRadialGradient      (xml_node<> *NewNode);
	PluginLinearGradient*   CreatePluginLinearGradient      (xml_node<> *NewNode);
    PluginReplicator*       CreatePluginReplicator          (xml_node<> *NewNode);
    PluginVideoInput*       CreatePluginVideoInput          (xml_node<> *NewNode);
    PluginCube*             CreatePluginCube                (xml_node<> *NewNode);
    
    vector<PluginReplicator*> ActiveReplicators;


	
};

bool SortByTypeFunction(XMLPlugin* A,XMLPlugin* B);




