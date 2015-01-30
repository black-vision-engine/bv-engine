#include "BlackTree.h"

// *********************************
//
bool BlackTree::LoadFromFile(string path)
{
	
	string temp = get_file_contents(path.c_str());
	
	xml_document<> doc;    // character type defaults to char
	

	char *a=new char[temp.size()+1];
	a[temp.size()]=0;
	memcpy(a,temp.c_str(),temp.size());

	doc.parse<0>(a);    // 0 means default parse flags

	ParseXml(doc);

	return true;

}	

// *********************************
//
vector<TimeProperty>* BlackTree::FindProperty(Plugin *plugin,string name)
{
	for(unsigned int i=0;i<plugin->properties.size();i++)
	{
		if(plugin->properties[i].name==name)
			return plugin->properties[i].timeproperty;
	}
	return NULL;
}

// *********************************
//
string* BlackTree::FindNonLinearProperty(Plugin *plugin,string name)
{
	for(unsigned int i=0;i<plugin->non_linear_properties.size();i++)
	{
		if(plugin->non_linear_properties[i].name==name)
			return plugin->non_linear_properties[i].value;
	}
	return NULL;
}

// *********************************
//
bool BlackTree::ParsePluginProperties(Plugin *plugin, xml_node<> *NewNode)
{
		vector<TimeProperty> *current=NULL;


	//cout<<"Adding "<<plugin->name<<" plugin..."<<endl;
	//getch();

	xml_node<> *CurrentNode = NewNode->first_node("property");
	while(CurrentNode!=NULL)
	{
			xml_attribute<> *attr = CurrentNode->first_attribute("name");
			string name=attr->value();
			//cout<<"property name..."<<name<<endl;

			current = FindProperty(plugin,name);


			if(current!=NULL)
			{
				xml_node<> *val = CurrentNode->first_node("timeval");
				
				while(val!=NULL)
				{
					xml_attribute<> *value = val->first_attribute("value");
					xml_attribute<> *time = val->first_attribute("time");
					string v=value->value();
					string t=time->value();

					current->push_back(TimeProperty(t,v));
					val = val->next_sibling();
				}
			}else{
				string* temp = FindNonLinearProperty(plugin,name);
				if(temp!=NULL)
				{
					
					xml_attribute<> *value = CurrentNode->first_attribute("value");
					*temp =(value->value());

				}

			}



			CurrentNode = CurrentNode->next_sibling();

	}
	return true;
}

// *********************************
//
PluginLinearGradient* BlackTree::CreatePluginLinearGradient(xml_node<> *NewNode)
{
	PluginLinearGradient *plugin = new PluginLinearGradient();
	ParsePluginProperties(plugin,NewNode);





	return plugin;
}

// *********************************
//
PluginRadialGradient* BlackTree::CreatePluginRadialGradient(xml_node<> *NewNode)
{
	PluginRadialGradient *plugin = new PluginRadialGradient();
	ParsePluginProperties(plugin,NewNode);

	xml_node<> * CurrentNode = NewNode->first_node("plugin");
    { CurrentNode; } // FIXME: suppress unused warning

	return plugin;
}

// *********************************
//
PluginGeometry *BlackTree::CreatePluginGeometry(xml_node<> *NewNode)
{
	PluginGeometry *plugin = new PluginGeometry();;
	//cout<<"parsing..."<<endl;
	ParsePluginProperties(plugin,NewNode);
	//cout<<"parsed..."<<endl;
	return plugin;
}

// *********************************
//
PluginPrimitive *BlackTree::CreatePluginPrimitive(xml_node<> *NewNode)
{
	PluginPrimitive *plugin = new PluginPrimitive();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginTexture* BlackTree::CreatePluginTexture(xml_node<> *NewNode)
{
	PluginTexture *plugin = new PluginTexture();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginText* BlackTree::CreatePluginText(xml_node<> *NewNode)
{
	PluginText *plugin = new PluginText();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginSolid *BlackTree::CreatePluginSolid(xml_node<> *NewNode)
{
	PluginSolid *plugin = new PluginSolid();
	ParsePluginProperties(plugin,NewNode);

	return plugin;
}

// *********************************
//
PluginExtrude* BlackTree::CreatePluginExtrude(xml_node<> *NewNode)
{
	PluginExtrude *plugin = new PluginExtrude();
	ParsePluginProperties(plugin,NewNode);

	return plugin;
}

// *********************************
//
bool BlackTree::CreatePlugin(BlackNode &node, xml_node<> *NewNode)
{
	
	Plugin *plugin;

	xml_attribute<> *attr = NewNode->first_attribute("name");
	string type=attr->value();

	//cout<<"create plugin..."<<type<<endl;
	//getch();

	if(type=="geometry")
	{
		plugin = CreatePluginGeometry(NewNode);
	}else if(type=="solid")
	{
		plugin = CreatePluginSolid(NewNode);
	}else if(type=="texture")
	{
		plugin = CreatePluginTexture(NewNode);
	}else if(type=="text")
	{
		plugin = CreatePluginText(NewNode);
	}else if(type=="primitive")
	{
		plugin = CreatePluginPrimitive(NewNode);
	}else if(type=="extrude")
	{
		plugin = CreatePluginExtrude(NewNode);
	}else if(type=="radialGradient")
	{
		plugin = CreatePluginRadialGradient(NewNode);
	}else if(type=="linearGradient")
	{
		plugin = CreatePluginLinearGradient(NewNode);
	}else{
		//cout<<endl<<"!! ERROR - dont know how to parse plugin: "<<type<<endl<<endl;
		return false;
	}
	

	node.plugins.push_back(plugin);
	return true;
}

// *********************************
//
bool BlackTree::ParsePlugins(BlackNode &node, xml_node<> *NewNode)
{
	//cout << "---parsing plugins..."<<endl;

	xml_node<> *CurrentNode = NewNode->first_node("plugin");
		while(CurrentNode!=NULL)
		{
			// xml_attribute<> *attr = CurrentNode->first_attribute("name");

			//cout<< "adding plugin : "<<attr->value()<<endl;

			CreatePlugin(node, CurrentNode);
			//cout<<"added"<<endl;
			CurrentNode = CurrentNode->next_sibling();
			
		}


	return true;
}

// *********************************
//
bool BlackTree::CreateNode(BlackNode &parrent, xml_node<> *NewNode)
{
	xml_attribute<> *nazwa = NewNode->first_attribute("name");
	

	BlackNode node;
	node.name=nazwa->value();
	//cout<< "---Parsing node : "<<node.name<<endl;

	xml_node<> *plugins = NewNode->first_node("plugins");

	if(plugins!=NULL)
		ParsePlugins(node,plugins);

	//getch();
	


	// dodaje dzieci
	xml_node<> *Root = NewNode->first_node("nodes");
	if(Root!=NULL)
	{
		xml_node<> *CurrentNode = Root->first_node("node");
		while(CurrentNode!=NULL)
		{
			// xml_attribute<> *attr = CurrentNode->first_attribute("name");

			////cout<< " children name is : "<<attr->value()<<endl;

			CreateNode(node, CurrentNode);
			CurrentNode = CurrentNode->next_sibling();
		}
	}

	parrent.children.push_back(node);
	//cout<<"--parsing done"<<endl;
	return true;
}

// *********************************
//
bool BlackTree::ParseXml(xml_document<> &doc)
{

	////cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

	xml_node<> *Root = doc.first_node("nodes");
	
	xml_node<> *CurrentNode = Root->first_node("node");

	while(CurrentNode!=NULL)
	{
		// xml_attribute<> *attr = CurrentNode->first_attribute("name");

		////cout<< " name is : "<<attr->value()<<endl;

		CreateNode(Scene.MainNode, CurrentNode);
		CurrentNode = CurrentNode->next_sibling();
	}

	return true;
}
