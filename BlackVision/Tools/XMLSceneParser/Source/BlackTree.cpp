#include <math.h>
#include <boost/algorithm/string/replace.hpp>
#include "BlackTree.h"
#include "win_sock.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "structure/AssetManager.h"

bool SortByTypeFunction(XMLPlugin* A,XMLPlugin* B)
{
	return A->type < B->type;
}


bool BlackTree::LoadFromFile(string path)
{
	
	string temp = get_file_contents(path.c_str());
	
	xml_document<> doc;    // character type defaults to char
	

	char *a=new char[temp.size()+1];
	a[temp.size()]=0;
	memcpy(a,temp.c_str(),temp.size());

	doc.parse<0>(a);    // 0 means default parse flags

	ParseXml(doc);
    delete[] a;

	return true;

}	

// *********************************
//
vector<TimeProperty>* BlackTree::FindProperty(XMLPlugin *plugin,string name)
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
Property* BlackTree::FindPropertyInstance(XMLPlugin *plugin,string name)
{
	for(unsigned int i=0;i<plugin->properties.size();i++)
	{
		if(plugin->properties[i].name==name)
			return &(plugin->properties[i]);
	}
	return NULL;
}

// *********************************
//
string* BlackTree::FindNonLinearProperty(XMLPlugin *plugin,string name)
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
wstring* BlackTree::FindNonLinearPropertyWide(XMLPlugin *plugin,string name)
{
	for(unsigned int i=0;i<plugin->non_linear_properties.size();i++)
	{
		if(plugin->non_linear_properties[i].name==name)
			return plugin->non_linear_properties[i].valueWide;
	}
	return NULL;
}

// *********************************
//

std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

float CosineInterpolate(double y1,double y2,double mu)
{

   float mu2;
   float pi_val = (float)3.14159265359;
   mu2 = (1-cos((float)mu*pi_val ))/2;
   return (float)(y1*(1-mu2)+y2*mu2);
}

void SmoothKeyframe(vector<TimeProperty> *current, float t1, float t2, float v1_1,float v1_2, float v2_1,float v2_2, float v3_1,float v3_2, float smoothing_offset)
{
	{ smoothing_offset; }

    //float value1_change = v1_2-v1_1;
    //float value2_change = v2_2-v2_1;
    //float value3_change = v3_2-v3_1;

    float time_change = t2-t1;
    
    int steps = (int)time_change * 100;

    for(int i=1;i<steps;i++)
    {
        float mu = (float)i/(float)steps;
        float time = t1+ mu*time_change;

        float v1 = CosineInterpolate(v1_1,v1_2,mu);
        float v2 = CosineInterpolate(v2_1,v2_2,mu);
        float v3 = CosineInterpolate(v3_1,v3_2,mu);
        string val = to_string(v1)+","+to_string(v2)+","+to_string(v3);
        current->push_back(TimeProperty(to_string(time),val));

    }
}

void AddSmoothing(vector<TimeProperty> *current)
{
    //return;
    for(unsigned int i=0;i<current->size();i++)
    {
        if(i>0)
        {
            if(current->operator[](i).smoothing!="0")
            {
                string point    =	current->operator[](i-1).value.c_str();

                std::vector<std::wstring> tok;
                split(tok, point, boost::is_any_of(L","));
                float x1=0,y1=0,z1=0;
                int count=0;
                for(std::vector<std::wstring>::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
                {
                       wstring s = *tok_iter;
                       if(count == 0)
                       {
                           x1 = boost::lexical_cast<float>(s);
                       }
                       if(count == 1)
                       {
                          y1 = boost::lexical_cast<float>(s);
                       }
                       if(count == 2)
                       {
                           z1 = boost::lexical_cast<float>(s);
                       }
                       count++;
                }

                string point2    =	current->operator[](i).value.c_str();

                std::vector<std::wstring> tok2;
                split(tok2, point2, boost::is_any_of(L","));
                float x2=0,y2=0,z2=0;
                count=0;
                for(std::vector<std::wstring>::iterator tok_iter = tok2.begin(); tok_iter != tok2.end(); ++tok_iter)
                {
                       wstring s = *tok_iter;
                       if(count == 0)
                       {
                           x2 = boost::lexical_cast<float>(s);
                       }
                       if(count == 1)
                       {
                          y2 = boost::lexical_cast<float>(s);
                       }
                       if(count == 2)
                       {
                           z2 = boost::lexical_cast<float>(s);
                       }
                       count++;
                }

                float t1 = (float)atof(current->operator[](i-1).time.c_str());
                float t2 = (float)atof(current->operator[](i).time.c_str());

                float smoothing = (float)atof(current->operator[](i).smoothing.c_str());
                SmoothKeyframe(current, t1,t2,x1,x2,y1,y2,z1,z2,smoothing);
            }
        }
    }

}

bool BlackTree::ParsePluginProperties(XMLPlugin *plugin, xml_node<> *NewNode)
{
	
    vector<TimeProperty> *current=NULL;

	xml_node<> *CurrentNode = NewNode->first_node("property");
    if(CurrentNode == NULL)
    {
        plugin->DefaultPluginProperties();
    }
	while(CurrentNode!=NULL)
	{
			xml_attribute<> *attr = CurrentNode->first_attribute("name");
            xml_attribute<> *attr_timeline = CurrentNode->first_attribute("timeline");
			string name=attr->value();
			//cout<<"property name..."<<name<<endl;

            


			current = FindProperty(plugin,name);
            if(attr_timeline!=NULL)
            {
                string tm = attr_timeline->value();
                Property *prop = FindPropertyInstance(plugin,name);
                if(prop!=NULL)
                {
                    for(int i=0;i<ActiveReplicators.size();i++)
                    {
                         
                         boost::replace_all(tm, "$COUNT" , to_string(ActiveReplicators[i]->Iteration));
                         
                    }
                    prop->timeline = tm;
                }
            }

			if(current!=NULL)
			{
				xml_node<> *val = CurrentNode->first_node("timeval");
				
				while(val!=NULL)
				{
					xml_attribute<> *value = val->first_attribute("value");
					xml_attribute<> *time = val->first_attribute("time");
                    xml_attribute<> *smoothing_xml = val->first_attribute("smoothing");
                    
					string v=value->value();
					string t=time->value();
                    string smoothing="";
                    if(smoothing_xml!=NULL)
                    {
                        smoothing=smoothing_xml->value();
                    }
                   
                    if(v.find("$")!=std::string::npos || t.find("$")!=std::string::npos )
                    {
                        for(int i=0;i<ActiveReplicators.size();i++)
                        {
                            for(int j=0;j<ActiveReplicators[i]->Offsets.size();j++)
                            {
                                boost::replace_all(v, "$"+ActiveReplicators[i]->Offsets[j].OffsetName , to_string(ActiveReplicators[i]->Offsets[j].currentValue));
                                boost::replace_all(t, "$"+ActiveReplicators[i]->Offsets[j].OffsetName , to_string(ActiveReplicators[i]->Offsets[j].currentValue));
                            }
                        }
                    }
                    if(smoothing=="")
					    current->push_back(TimeProperty(t,v));
                    else
                        current->push_back(TimeProperty(t,v,smoothing));
					val = val->next_sibling();

                    AddSmoothing(current);
				}
			}else{

				if((plugin->name=="text" || plugin->name=="timer") && name=="text" || name=="promo_msg")
				{
					wstring* temp = FindNonLinearPropertyWide(plugin,name);
					if(temp!=NULL)
					{
					
						xml_attribute<> *value = CurrentNode->first_attribute("value");
						wstring str = s2ws( value->value());
						*temp = str;

					}
				}else{
					string* temp = FindNonLinearProperty(plugin,name);
					if(temp!=NULL)
					{
					
						xml_attribute<> *value = CurrentNode->first_attribute("value");
                        string v= value->value();
                        
                    if(v.find("$")!=std::string::npos )
                    {
                        for(int i=0;i<ActiveReplicators.size();i++)
                        {
                            for(int j=0;j<ActiveReplicators[i]->Offsets.size();j++)
                            {
                                boost::replace_all(v, "$"+ActiveReplicators[i]->Offsets[j].OffsetName , to_string(ActiveReplicators[i]->Offsets[j].currentValue));
                              
                            }
                        }
                        
                    }
					    *temp = v;

					}
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

	xml_node<> *CurrentNode = NewNode->first_node("plugin");
	{CurrentNode;}




	return plugin;
}

// *********************************
//
PluginVideoInput* BlackTree::CreatePluginVideoInput(xml_node<> *NewNode)
{
	PluginVideoInput *plugin = new PluginVideoInput();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginCube* BlackTree::CreatePluginCube(xml_node<> *NewNode)
{
	PluginCube *plugin = new PluginCube();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginTransform *BlackTree::CreatePluginTransform(xml_node<> *NewNode)
{
	PluginTransform *plugin = new PluginTransform();;
	//cout<<"parsing..."<<endl;
	ParsePluginProperties(plugin,NewNode);
	//cout<<"parsed..."<<endl;
	return plugin;
}
// *********************************
//
PluginTexture* BlackTree::CreatePluginTexture(xml_node<> *NewNode)
{
	PluginTexture *plugin = new PluginTexture();
    ParsePluginProperties(plugin,NewNode);
    xml_attribute<> *attr = NewNode->first_attribute("path");
    if(attr!=NULL)
    {
        string* temp = FindNonLinearProperty(plugin,"path");
	    *temp = attr->value();
    }
	
    
	return plugin;
}

// *********************************
//
PluginTimer* BlackTree::CreatePluginTimer(xml_node<> *NewNode)
{
	PluginTimer *plugin = new PluginTimer();
    ParsePluginProperties(plugin,NewNode);
    xml_attribute<> *attr = NewNode->first_attribute("path");
    if(attr!=NULL)
    {
        string* temp = FindNonLinearProperty(plugin,"path");
	    *temp = attr->value();
    }
	
    
	return plugin;
}

// *********************************
//
PluginSequence* BlackTree::CreatePluginSequence(xml_node<> *NewNode)
{
	PluginSequence *plugin = new PluginSequence();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginCrawl* BlackTree::CreatePluginCrawl(xml_node<> *NewNode)
{
	PluginCrawl *plugin = new PluginCrawl();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginMask* BlackTree::CreatePluginMask(xml_node<> *NewNode)
{
	PluginMask *plugin = new PluginMask();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginRectangle *BlackTree::CreatePluginRectangle(xml_node<> *NewNode)
{
	PluginRectangle *plugin = new PluginRectangle();
    ParsePluginProperties(plugin,NewNode);

    xml_attribute<> *attr = NewNode->first_attribute("preset");
    if(attr!=NULL)
    {
        string val = attr->value();
        if(val =="HD")
        {
            string* temp = FindNonLinearProperty(plugin,"width");
	        *temp = "1920";
            temp = FindNonLinearProperty(plugin,"height");
	        *temp = "1080";
        }
        if(val =="autosize")
        {
            string* temp = FindNonLinearProperty(plugin,"autosize");
	        *temp = "true";
        }
    }
	
	return plugin;
}


// *********************************
//
PluginPrism *BlackTree::CreatePluginPrism(xml_node<> *NewNode)
{
	PluginPrism *plugin = new PluginPrism();
    ParsePluginProperties(plugin,NewNode);
	return plugin;
}


// *********************************
//
PluginRing *BlackTree::CreatePluginRing(xml_node<> *NewNode)
{
	PluginRing *plugin = new PluginRing();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************


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
PluginInstance* BlackTree::CreatePluginInstance(BlackNode *node,xml_node<> *NewNode)
{
	PluginInstance *plugin = new PluginInstance();
	ParsePluginProperties(plugin,NewNode);

    for(unsigned int i=0;i<Scene.Classes.size();i++)
    {
        if(plugin->class_name == Scene.Classes[i]->name)
        {
            node->children.push_back((Scene.Classes[i]->node));
        }
    }
	return plugin;
}

// *********************************
//
PluginAlpha* BlackTree::CreatePluginAlpha(xml_node<> *NewNode)
{
	PluginAlpha *plugin = new PluginAlpha();
	ParsePluginProperties(plugin,NewNode);
	return plugin;
}

// *********************************
//
PluginSolid *BlackTree::CreatePluginSolid(xml_node<> *NewNode)
{
	PluginSolid *plugin = new PluginSolid();
	ParsePluginProperties(plugin,NewNode);
    xml_attribute<> *attr_surface = NewNode->first_attribute("surface");
    if(attr_surface!=NULL)
    {
        string val = attr_surface->value();
        plugin->surface = val; 
        SurfaceDescriptor* surface = bv::BB::AssetManager::GetSurface(val);
        vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
        current->push_back(TimeProperty("0",to_string(surface->color_main.R)+","+to_string(surface->color_main.G)+","+to_string(surface->color_main.B)+","+to_string(surface->color_main.A)));
    }
    xml_attribute<> *attr = NewNode->first_attribute("color");
    if(attr!=NULL)
    {
        string val = attr->value();
        if(val =="white")
        {
            vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
			current->push_back(TimeProperty("0","1,1,1,1"));
        }
         if(val =="gray")
        {
            vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
			current->push_back(TimeProperty("0","0.63,0.66,0.67,1"));
        }
        if(val =="red")
        {
            vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
			current->push_back(TimeProperty("0","1,0,0,1"));
        }
        if(val =="verva_red")
        {
            vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
			current->push_back(TimeProperty("0","0.9,0.1,0.16,1"));
        }
         if(val =="blue")
        {
            vector<TimeProperty> *current = FindProperty(plugin,"RGBA");
			current->push_back(TimeProperty("0","0.21,0.37,0.65,1"));
        }
       
    }

	return plugin;
}

// *********************************
//

PluginReplicator *BlackTree::CreatePluginReplicator(xml_node<> *NewNode)
{
	PluginReplicator *plugin = new PluginReplicator();
	ParsePluginProperties(plugin,NewNode);
    plugin->count = atoi(plugin->s_count.c_str());
    
    xml_node<> *offsets = NewNode->first_node("offsets");

    if(offsets!=NULL)
    {
        xml_node<> *CurrentOffset = offsets->first_node("offset");
	    while(CurrentOffset!=NULL)
        {
             PluginOffset offset;
             xml_node<> *properties = CurrentOffset->first_node("property");
             while(properties!=NULL)
             {
                 xml_attribute<> *attr_name = properties->first_attribute("name");
                 xml_attribute<> *attr_value = properties->first_attribute("value");
                 if(attr_name==NULL||attr_value==NULL)
                 {
                     properties = properties->next_sibling();
                     CurrentOffset = CurrentOffset->next_sibling();
                     continue;
                 }
                 string s_name = attr_name->value();
                 string s_value = attr_value->value();
                 if(s_name=="offset_name")
                 {
                     offset.OffsetName = s_value;
                 }else if(s_name=="start")
                 {
                     offset.Start = (float)atof(s_value.c_str());
                     offset.currentValue = (float)atof(s_value.c_str());
                 }else if(s_name=="offset")
                 {
                     offset.OffsetValue = (float)atof(s_value.c_str());
                 }
                 properties = properties->next_sibling();
             }
             plugin->Offsets.push_back(offset);
             plugin->Iteration=0;
             CurrentOffset = CurrentOffset->next_sibling();
        }
    }

    ActiveReplicators.push_back(plugin);

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
	
	XMLPlugin *plugin;

	xml_attribute<> *attr = NewNode->first_attribute("name");
	string type=attr->value();

    xml_attribute<> *timeline = NewNode->first_attribute("timeline");
    


	//cout<<"create plugin..."<<type<<endl;
	//getch();



	if(type=="geometry")
	{
		plugin = CreatePluginTransform(NewNode);
	}else if(type=="video_input")
	{
		plugin = CreatePluginVideoInput(NewNode);
	}else if(type=="cube")
	{
		plugin = CreatePluginCube(NewNode);
	}else if(type=="solid")
	{
		plugin = CreatePluginSolid(NewNode);
	}else if(type=="instance")
	{
		plugin = CreatePluginInstance(&node,NewNode);
	}else if(type=="texture")
	{
		plugin = CreatePluginTexture(NewNode);
	}else if(type=="crawl")
	{
		plugin = CreatePluginCrawl(NewNode);	
	}else if(type=="sequence")
	{
		plugin = CreatePluginSequence(NewNode);	
	}else if(type=="text")
	{
		plugin = CreatePluginText(NewNode);
	}else if(type=="timer")
	{
		plugin = CreatePluginTimer(NewNode);
	}else if(type=="alpha")
	{
		plugin = CreatePluginAlpha(NewNode);
	}else if(type=="rectangle")
	{
		plugin = CreatePluginRectangle(NewNode);
	}else if(type=="prism")
	{
		plugin = CreatePluginPrism(NewNode);
	}else if(type=="mask")
	{
		plugin = CreatePluginMask(NewNode);
	}else if(type=="ring")
	{
		plugin = CreatePluginRing(NewNode);
	}else if(type=="extrude")
	{
		plugin = CreatePluginExtrude(NewNode);
	}else if(type=="radialGradient")
	{
		plugin = CreatePluginRadialGradient(NewNode);
	}else if(type=="linearGradient")
	{
		plugin = CreatePluginLinearGradient(NewNode);
	}else if(type=="replicator")
	{
		plugin = CreatePluginReplicator(NewNode);
	}else{
		//cout<<endl<<"!! ERROR - dont know how to parse plugin: "<<type<<endl<<endl;
		return false;
	}
	
    if(timeline==NULL)
    {
        plugin->timeline="default";
    }else{
        
               string tm = timeline->value() ;
                    for(int i=0;i<ActiveReplicators.size();i++)
                    {
                         
                         boost::replace_all(tm, "$COUNT" , to_string(ActiveReplicators[i]->Iteration));
                         
                    }
                   
                
        plugin->timeline=tm;
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
			xml_attribute<> *attr = CurrentNode->first_attribute("name");
			{attr;}
			//cout<< "adding plugin : "<<attr->value()<<endl;

			CreatePlugin(node, CurrentNode);
			//cout<<"added"<<endl;
			CurrentNode = CurrentNode->next_sibling();
			
		}
		//sort plugins by type
		
		std::sort (node.plugins.begin(), node.plugins.end(), SortByTypeFunction);


	return true;
}


// *********************************
//
bool BlackTree::CreateNode(BlackNode &parrent, xml_node<> *NewNode, string node_name="")
{
    xml_attribute<> *nazwa = NewNode->first_attribute("name");
    xml_attribute<> *ommit = NewNode->first_attribute("ommit");
    if(ommit!=NULL)
    {
        string om = ommit->value();
        if(om =="true")
        {
            return false;
        }
    }
   
	Scene.NodeCount++;
	

	

	BlackNode node;

    xml_attribute<> *blending = NewNode->first_attribute("blend_mode");
    if(blending!=NULL)
    {
        string blend = blending->value();
        if(blend =="add")
        {
            node.blend_mode=BLEND_MODES::ADD;
        }
    }
    xml_attribute<> *visible = NewNode->first_attribute("visible");
    if(visible!=NULL)
    {
        string visibility = visible->value();
        if(visibility=="true")
            node.visible = true;
        else
            node.visible = false;
    }

    if(node_name=="")
	    node.name=nazwa->value();
    else
    {
        node.name=node_name;
    }




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
			xml_attribute<> *attr = CurrentNode->first_attribute("name");

			////cout<< " children name is : "<<attr->value()<<endl;
            bool ommit=false;
            if(attr!=NULL)
            {
                string node_name    =   attr->value();
                for(unsigned int j=0;j<ActiveReplicators.size();j++)
                {
                    if(ActiveReplicators[j]->target_name == node_name)
                    {
                        for(int i=0;i<ActiveReplicators[j]->count;i++)
                        {
                            // check if node is needed to ommit
                            string indexes = ActiveReplicators[j]->ommit_indexes;
                            std::vector<std::string> tok;
                            split(tok, indexes, boost::is_any_of(L","));
                            
                            bool ommit = false;
                            for(std::vector<std::string>::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
                            {
                               
                                string s = *tok_iter;
                                if(s=="")continue;
                                int temp = boost::lexical_cast<int>(s)-1;
                                if(temp==i)
                                {
                                    ommit = true;
                                    break;
                                }
                      
                            }
                            ActiveReplicators[j]->Iteration = i;
                            if(!ommit)
                            {
                                CreateNode(node, CurrentNode,node_name+to_string(i));
                            }
                            
                            for(int w=0;w<ActiveReplicators[j]->Offsets.size();w++)
                            {
                                ActiveReplicators[j]->Offsets[w].currentValue = ActiveReplicators[j]->Offsets[w].currentValue + ActiveReplicators[j]->Offsets[w].OffsetValue;
                                
                            } 
                        }
                        CurrentNode = CurrentNode->next_sibling();
                        ommit = true;
                        continue;
                    }
                }
            }
            if(ommit)continue;
			CreateNode(node, CurrentNode,"");
			CurrentNode = CurrentNode->next_sibling();
		}
	}

    // remove possible replicators
    for(unsigned int i=0;i<node.plugins.size();i++)
    {
        if(node.plugins[i]->name=="replicator")
        {
            PluginReplicator *replicator = (PluginReplicator*)node.plugins[i];
            for(unsigned int j=0;j<ActiveReplicators.size();j++)
            {
                if(ActiveReplicators[j]==replicator)
                {
                    ActiveReplicators.erase(ActiveReplicators.begin()+j);
                }
            }
        }else{

        }
    }

	//todo: hack ¿eby dzia³a³o bez nodea grupuj¹cego który nie posiada pluginów
	if(parrent.name=="Root")
		parrent = node;
	else
		parrent.children.push_back(node);
	//cout<<"--parsing done"<<endl;
	return true;
}

// *********************************
//
bool BlackTree::CreateTimeline(xml_node<> *NewTimeline)
{

	xml_attribute<> *nazwa = NewTimeline->first_attribute("name");
    xml_attribute<> *type = NewTimeline->first_attribute("type");
    xml_attribute<> *autoplay = NewTimeline->first_attribute("autoplay");
    xml_attribute<> *duration = NewTimeline->first_attribute("duration");
	

	BlackTimeline *timeline = new BlackTimeline();
	timeline->name=nazwa->value();
    if(type!=NULL)
    {
        timeline->type = type->value();
    }
    if(autoplay!=NULL)
    {
        timeline->autoplay = autoplay->value();
    }
    if(duration!=NULL)
    {
        timeline->duration = duration->value();
    }
	//cout<< "---Parsing node : "<<node.name<<endl;

	xml_node<> *keyframes = NewTimeline->first_node("keyframes");

    if(keyframes!=NULL)
    {
        xml_node<> *CurrentKeyframe = keyframes->first_node("keyframe");
	    while(CurrentKeyframe!=NULL)
        {
            
            xml_attribute<> *attr_name = CurrentKeyframe->first_attribute("name");
            xml_attribute<> *attr_type = CurrentKeyframe->first_attribute("type");
            xml_attribute<> *attr_time = CurrentKeyframe->first_attribute("time");
            xml_attribute<> *attr_time2 = CurrentKeyframe->first_attribute("time2");
            
            if(attr_name == NULL || attr_type==NULL || attr_time==NULL)
            {
                CurrentKeyframe = CurrentKeyframe->next_sibling();
                continue;
            }

            BlackKeyframe * keyframe = new BlackKeyframe();
            keyframe->name = attr_name->value();
            keyframe->time = (float)atof(attr_time->value());
            if(attr_time2 != NULL)
            {
                keyframe->time2 = (float)atof(attr_time2->value());
            }
            keyframe->type = attr_type->value();

            timeline->keyframes.push_back(keyframe);

            CurrentKeyframe = CurrentKeyframe->next_sibling();
        }
    }
    Scene.Timelines.push_back(timeline);
    

	return true;
}


// *********************************
//
bool BlackTree::CreateClasses(xml_node<> *NewClass)
{

	xml_attribute<> *nazwa = NewClass->first_attribute("name");
	if(nazwa==NULL)return false;

	BlackClass *blackClass = new BlackClass();
	blackClass->name=nazwa->value();
    
	xml_node<> *CurrentNode = NewClass->first_node("node");

	if(CurrentNode!=NULL)
	{
        CreateNode(blackClass->node, CurrentNode,"");
	}

    Scene.Classes.push_back(blackClass);
    
	return true;
}

// *********************************
//
bool BlackTree::ParseXml(xml_document<> &doc)
{

	////cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

     // parse timelines
    xml_node<> *Scene = doc.first_node("scene");
    if(Scene == NULL) 
        return false;

    xml_node<> *Metatags = Scene->first_node("metatags");
    if(Metatags!=NULL)
	{
		xml_node<> *CurrentMeta = Metatags->first_node("meta");
		while(CurrentMeta!=NULL)
		{
			xml_attribute<> *name   = CurrentMeta->first_attribute("name");
            xml_attribute<> *value  = CurrentMeta->first_attribute("value");
            string name_s             = name->value();
            string value_s            = value->value();
            this->Scene.MetaTags.push_back(new KeyValue(name_s,value_s));

            CurrentMeta = CurrentMeta->next_sibling();
		}
	}

    this->Scene.ParseMetaTags();


    xml_node<> *Timelines = Scene->first_node("timelines");
    if(Timelines!=NULL)
	{
		xml_node<> *CurrentTimeline = Timelines->first_node("timeline");
		while(CurrentTimeline!=NULL)
		{
			xml_attribute<> *attr = CurrentTimeline->first_attribute("name");
			{attr;}
			////cout<< " children name is : "<<attr->value()<<endl;

			CreateTimeline(CurrentTimeline);
			CurrentTimeline = CurrentTimeline->next_sibling();
		}
	}

    xml_node<> *Classes = Scene->first_node("classes");
    if(Classes!=NULL)
	{
		xml_node<> *CurrentClass = Classes->first_node("class");
		while(CurrentClass!=NULL)
		{
			CreateClasses(CurrentClass);
			CurrentClass = CurrentClass->next_sibling();
		}
	}

	xml_node<> *Root = Scene->first_node("nodes");
	
	xml_node<> *CurrentNode = Root->first_node("node");

	while(CurrentNode!=NULL)
	{
		xml_attribute<> *attr = CurrentNode->first_attribute("name");
		{attr;}
		////cout<< " name is : "<<attr->value()<<endl;

		CreateNode(this->Scene.MainNode, CurrentNode,"");
		
		CurrentNode = CurrentNode->next_sibling();
	}

	return true;
}
