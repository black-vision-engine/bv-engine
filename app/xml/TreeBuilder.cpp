#include "Engine/Models/Plugins/PluginsFactory.h"
#include "TreeBuilder.h"
#include "Engine/Models/BasicNode.h"
#include "Mathematics/Transform/MatTransform.h"
#include "Engine/Models/Plugins/Simple/SimpleTransformPlugin.h"
#include "Engine/Models/Resources/TextHelpers.h"
#include <fstream>
#include <boost/algorithm/string.hpp>


namespace bv{
	// for mantainging clear output
	string AddTabs(int depth)
	{
		string temp="";
		for(int i=0;i<depth;i++)
			temp+="  ";
		return temp;
	}

	void PrintTree(BlackNode &CurrentNode,int depth=0)
	{
		//node name
		cout<<AddTabs(depth)<<"[Node "<<CurrentNode.name<<" ]"<<endl;
		//getch();
		//display all plugins
		cout<<AddTabs(depth)<<" [plugins "<<CurrentNode.plugins.size()<<"]"<<endl;
		for(unsigned int i=0;i<CurrentNode.plugins.size();i++)
		{
			//plugin name
			Plugin *pl = CurrentNode.plugins[i];
			pl->name;
			//PluginGeometry *plugin = (PluginGeometry*)CurrentNode.plugins[i];
			//cout<<plugin->name<<endl;
		
			cout<<AddTabs(depth)<<"  -"<<pl->name<<endl;
		

			//all plugin linear properties
			for(unsigned int j=0;j<CurrentNode.plugins[i]->properties.size();j++)
			{
				//property name
				cout<<AddTabs(depth)<<"     # "<<CurrentNode.plugins[i]->properties[j].name<<", size: "<<CurrentNode.plugins[i]->properties[j].timeproperty->size()<<endl;

				//all property values ( at different time values )
				vector<TimeProperty> *timevals = CurrentNode.plugins[i]->properties[j].timeproperty;
				//cout<<"udalo sie"<<endl;
				for(unsigned int h=0;h<timevals->size();h++)
				{
					cout<<AddTabs(depth)<<"         ("<<timevals->operator[](h).time<<", "<<timevals->operator[](h).value<<")"<<endl;
				}
			}

			//all plugin non linear properties
			for(unsigned int j=0;j<CurrentNode.plugins[i]->non_linear_properties.size();j++)
			{
				//property name and vlaue - note * (asterisk) to extract string value
				cout<<AddTabs(depth)<<"     # "<<CurrentNode.plugins[i]->non_linear_properties[j].name<<": '"<< *CurrentNode.plugins[i]->non_linear_properties[j].value<<"'"<<endl;
			} 



			// radialGradient - example of direct access to member variables ( solid1, solid2 )

		
		}
		for(unsigned int i=0;i<CurrentNode.children.size();i++)
		{
			PrintTree(CurrentNode.children[i],depth+1);
		}
	}


	TreeBuilder::TreeBuilder(void)
	{
	}


	TreeBuilder::~TreeBuilder(void)
	{
	}

	
	model::BasicNode*  deleteMe()
	{

		model::BasicNode * root = new model::BasicNode();

		std::wstring str    =   TextHelper::LoadUtf8FileToString( L"text_example.txt");

		Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );

		color.AddKey(0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
		color.AddKey(3.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
		color.AddKey(5.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		color.AddKey(7.f, glm::vec4( 1.f, 1.f, 1.f, 1.f ) );

		TransformF     trns;

		FloatInterpolator xt; xt.SetWrapPostMethod( bv::WrapMethod::pingPong );
		FloatInterpolator yt; yt.SetWrapPostMethod( bv::WrapMethod::repeat );
		FloatInterpolator zt;

		xt.AddKey(0.f, -1.f);
		yt.AddKey(0.f, -5.f);
		zt.AddKey(0.f, -5.f);

		yt.AddKey(30.f, 5.f);

		trns.AddTranslation( xt, yt, zt );

		trns.AddScale( bv::InterpolatorsHelper::CreateConstValue( 1.f ), bv::InterpolatorsHelper::CreateConstValue( 1.f ), bv::InterpolatorsHelper::CreateConstValue( 1.f ) );

		auto texPlugin      =   bv::model::PluginsFactory::CreateSimpleTextPlugin( str, "../dep/Media/fonts/ARIALUNI.TTF", 64 );

        root->AddPlugin( texPlugin );

        root->AddPlugin( bv::model::PluginsFactory::CreateSimpleTransformPlugin( texPlugin, model::ParametersFactory::CreateParameter( "transformation", trns ) ) );


		return root;
	}

	void AttachSolidPlugin(model::BasicNode *EngineNode, string pluginName, vector<Property> properties, vector<NonLinearProperty> non_linear_properties)
	{
		Vec4Interpolator color; color.SetWrapPostMethod( bv::WrapMethod::pingPong );
		for(unsigned int i=0;i<properties.size();i++)
		{
			vector<TimeProperty> *timevals = properties[i].timeproperty;				
			string propertyName = properties[i].name;
			string rgba;
			if(propertyName=="RGBA")
			{
				for(unsigned int h=0;h<timevals->size();h++)
				{
					auto tF=atof(timevals->operator[](h).time.c_str());
					string temp=timevals->operator[](h).value;
					vector<string> V;
					//boost::split(V,temp,"|");
					auto rF=atof(V[0].c_str());
					auto gF=atof(V[1].c_str());
					auto bF=atof(V[2].c_str());
					auto aF=atof(V[3].c_str());
					color.AddKey(float(tF),glm::vec4(float(rF),float(gF),float(bF),float(aF)));						
				}
			}
		}
	}

	void AttachGeometryPlugin(model::BasicNode *EngineNode, string pluginName, vector<Property> properties, vector<NonLinearProperty> non_linear_properties)
	{
		TransformF     trns;
		
		FloatInterpolator xt;
		FloatInterpolator yt; 
		FloatInterpolator zt;
		FloatInterpolator rotation = bv::InterpolatorsHelper::CreateConstValue(1.f);
		FloatInterpolator xr;
		FloatInterpolator yr; 
		FloatInterpolator zr;
		FloatInterpolator xs;
		FloatInterpolator ys; 
		FloatInterpolator zs;

		xt.SetWrapPostMethod( bv::WrapMethod::pingPong );
		yt.SetWrapPostMethod( bv::WrapMethod::repeat );
		
		FloatInterpolator *Current;
		for(unsigned int i=0;i<properties.size();i++)
		{
			vector<TimeProperty> *timevals = properties[i].timeproperty;
				
			string propertyName = properties[i].name;
			if(propertyName=="posX")
				Current = &xt;
			else if(propertyName=="posY")
				Current = &yt;
			else if(propertyName=="posZ")
				Current = &zt;
			else if(propertyName=="rotX")
				Current = &xr;
			else if(propertyName=="rotY")
				Current = &yr;
			else if(propertyName=="rotZ")
				Current = &zr;
			else if(propertyName=="scaX")
				Current = &xs;
			else if(propertyName=="scaX")
				Current = &ys;
			else if(propertyName=="scaX")
				Current = &zs;

			for(unsigned int h=0;h<timevals->size();h++)
			{
				float tF=float(atof(timevals->operator[](h).time.c_str()));
				float hF=float(atof(timevals->operator[](h).value.c_str()));

				Current->AddKey(tF,hF);
			}
		}
		trns.AddTranslation( xt, yt, zt );
		trns.AddScale(xs,ys,zs);
		trns.AddRotation(rotation,xr,yr,zr);
	}
	

	void AttachTextPlugin(model::BasicNode *EngineNode, string pluginName, vector<Property> properties, vector<NonLinearProperty> non_linear_properties)
	{
		string text;
		string font_name;
		int size;
		for(unsigned int i=0;i<non_linear_properties.size();i++)
		{
				string propertyName = non_linear_properties[i].name;
				if(propertyName=="text")
					text = *non_linear_properties[i].value;
				else if(propertyName=="font")
					font_name = *non_linear_properties[i].value;
				else if(propertyName=="size")
					size = atoi(non_linear_properties[i].value->c_str());
		}

		//auto texPlugin = new model::SimpleTextPlugin( text, font_name, size );

	}

	void AttachPlugin(model::BasicNode *EngineNode, string pluginName, vector<Property> properties, vector<NonLinearProperty> non_linear_properties)
	{

	}

	void TreeBuilder::SendTree(BlackNode &CurrentNode,int depth)
	{
		//node name
		//cout<<AddTabs(depth)<<"[Node "<<CurrentNode.name<<" ]"<<endl;
		//getch();
		//display all plugins
		//cout<<AddTabs(depth)<<" [plugins "<<CurrentNode.plugins.size()<<"]"<<endl;
		
		//onNodeBegin(CurrentNode.name,depth);
		model::BasicNode * newNode = new model::BasicNode();

		for(unsigned int i=0;i<CurrentNode.plugins.size();i++)
		{
			//plugin name
			Plugin *pl = CurrentNode.plugins[i];
			//pl->name;
			//PluginGeometry *plugin = (PluginGeometry*)CurrentNode.plugins[i];
			//cout<<plugin->name<<endl;
		
			//cout<<AddTabs(depth)<<"  -"<<pl->name<<endl;
			//onPluginBegin(pl->name);

			
		
			//all plugin linear properties
			for(unsigned int j=0;j<CurrentNode.plugins[i]->properties.size();j++)
			{
				//property name
				//cout<<AddTabs(depth)<<"     # "<<CurrentNode.plugins[i]->properties[j].name<<", size: "<<CurrentNode.plugins[i]->properties[j].timeproperty->size()<<endl;

				//all property values ( at different time values )
				vector<TimeProperty> *timevals = CurrentNode.plugins[i]->properties[j].timeproperty;
				//cout<<"udalo sie"<<endl;
				for(unsigned int h=0;h<timevals->size();h++)
				{
					//cout<<AddTabs(depth)<<"         ("<<timevals->operator[](h).time<<", "<<timevals->operator[](h).value<<")"<<endl;
				}
			
				string propertyName = CurrentNode.plugins[i]->properties[j].name;
				vector<pair<float,float>> V;// = new vector<pair<float,float>>();
				for(unsigned int h=0;h<timevals->size();h++)
				{
					//cout<<AddTabs(depth)<<"         ("<<timevals->operator[](h).time<<", "<<timevals->operator[](h).value<<")"<<endl;
					float tF=float(atof(timevals->operator[](h).time.c_str()));
					float hF=float(atof(timevals->operator[](h).value.c_str()));

					V.push_back(make_pair(tF,hF));
					//onVisit(propertyName,V);
				}

			}

			//all plugin non linear properties
			for(unsigned int j=0;j<CurrentNode.plugins[i]->non_linear_properties.size();j++)
			{
				//property name and vlaue - note * (asterisk) to extract string value
				cout<<AddTabs(depth)<<"     # "<<CurrentNode.plugins[i]->non_linear_properties[j].name<<": '"<< *CurrentNode.plugins[i]->non_linear_properties[j].value<<"'"<<endl;
				string name=CurrentNode.plugins[i]->non_linear_properties[j].name;
				string value= *CurrentNode.plugins[i]->non_linear_properties[j].value;
				//onVisit(name,value);
			} 



			// radialGradient - example of direct access to member variables ( solid1, solid2 )

		
		}
		for(unsigned int i=0;i<CurrentNode.children.size();i++)
		{
			SendTree(CurrentNode.children[i],depth+1);
		}
	}	


	model::BasicNode* TreeBuilder::BuildTree(string path){
		
		model::BasicNode * root = new model::BasicNode();


		BlackTree Tree;

		Tree.LoadFromFile(path);

		cout<<"printing tree..."<<endl;
		PrintTree(Tree.Scene.MainNode);

		//SendTree(Tree.Scene.MainNode);

		return deleteMe();


	}
}