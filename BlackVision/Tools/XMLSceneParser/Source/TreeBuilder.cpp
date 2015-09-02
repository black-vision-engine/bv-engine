#include "Engine/Models/Interfaces/IOverrideState.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
//#include "Engine/Models/Resources/TextHelpers.h"
#include <boost/lexical_cast.hpp>

// default plugins 
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"

#include "Engine/Models/Plugins/PluginUtils.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Assets/Font/TextHelper.h"
//FIXME: as you can see - THIS IS A HACK
#include "../../../../Applications/BlackVision/Source/Widgets/Crawler/Crawler.h"
#include "../../../../Applications/BlackVision/Source/Widgets/Counter/Counter.h"
//#include "Helpers/RectNodeBuilder.h"


//video input
#include "../../../../Applications/BlackVision/Source/VideoInput/DefaultVideoInputResourceDescr.h"
#include "../../../../Applications/BlackVision/Source/hack_videoinput/TestVideoInput.h"

#include "Engine/Models/Plugins/Simple/DefaultCubePlugin.h"

// Log
#include "Log.h"
//#include "../BVConfig.h"

// ---------

#include "TreeBuilder.h"
#include "structure/AssetManager.h"
#include <sstream>
#pragma warning(disable:4996)

namespace bv{
	
	using namespace model;
	// for mantainging clear output
	string AddTabs(int depth)
	{
		string temp="";
		for(int i=0;i<depth;i++)
			temp+="  ";
		return temp;
	}

	//**********************************
	void PrintTree(BlackNode &CurrentNode,int depth=0)
	{
		//node name
		cout<<AddTabs(depth)<<"[Node "<<CurrentNode.name<<" ]"<<endl;
		//display all plugins
		cout<<AddTabs(depth)<<" [plugins "<<CurrentNode.plugins.size()<<"]"<<endl;
		for(unsigned int i=0;i<CurrentNode.plugins.size();i++)
		{
			//plugin name
			XMLPlugin *pl = CurrentNode.plugins[i];
			pl->name;
		
			cout<<AddTabs(depth)<<"  -"<<pl->name<<endl;
		
			//all plugin linear properties
			for(unsigned int j=0;j<CurrentNode.plugins[i]->properties.size();j++)
			{
				//property name
				cout<<AddTabs(depth)<<"     # "<<CurrentNode.plugins[i]->properties[j].name<<", size: "<<CurrentNode.plugins[i]->properties[j].timeproperty->size()<<endl;

				//all property values ( at different time values )
				vector<TimeProperty> *timevals = CurrentNode.plugins[i]->properties[j].timeproperty;
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
		}
		for(unsigned int i=0;i<CurrentNode.children.size();i++)
		{
			PrintTree(CurrentNode.children[i],depth+1);
		}
	}

    int TreeBuilder::TotalNodesCount=0;
	//**********************************
	TreeBuilder::TreeBuilder(void)
	{

		NodesLoaded =0;
	}


	//**********************************
	TreeBuilder::~TreeBuilder(void)
	{
	}

	//**********************************
	void TreeBuilder::PrepareNode(void)
	{

	}




	//**********************************
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss, item, delim)) 
		{
			elems.push_back(item);
		}
		return elems;
	}

    //**********************************
    ITimeEvaluatorPtr TreeBuilder::GetTimeline(string name)
    {
        ITimeEvaluatorPtr offset_timeline = timeline_default;
        auto timeline = timelineManager->GetTimeline(Tree.Scene.Meta.SceneName+"*"+name);
        if(timeline!=NULL)
        {
            offset_timeline = timeline->GetChild(Tree.Scene.Meta.SceneName+"*"+name);
            if(offset_timeline == NULL)offset_timeline = timeline_default;
        }
        return offset_timeline;
    }

	//**********************************
	bool TreeBuilder::AttachSolidPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
       
        
        bool result = node->AddPlugin( "DEFAULT_COLOR", "solid color", GetTimeline(plugin->timeline) ); 
        
        if(!result) return false;

        for(unsigned int i=0;i<plugin->properties.size();i++)
		{
			vector<TimeProperty> *timevals  = plugin->properties[i].timeproperty;				
			string propertyName             = plugin->properties[i].name;
			string rgba;
			if(propertyName		==	"RGBA")
			{
				for(unsigned int h=0;h<timevals->size();h++)
				{
					
					auto tF		=	atof(timevals->operator[](h).time.c_str());
					string temp	=	timevals->operator[](h).value;

					vector<string> V;
															
					V = split(temp,',',V);
					float rF	=	(float)	atof(V[0].c_str());
					float gF	=	(float)	atof(V[1].c_str());
					float bF	=	(float)	atof(V[2].c_str());
					float aF	=	(float)	atof(V[3].c_str());
                    result = result && SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( tF ), glm::vec4( rF,gF,bF, aF ) );
					    
				}
			}
		}

	
        return true;
	}

    //**********************************
    //
    bool TreeBuilder::AttachAlphaPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
        auto state = node->GetOverrideState();
        auto alpha = state->GetAlphaParam();

        alpha->SetTimeEvaluator(GetTimeline(plugin->timeline));

        for(unsigned int i=0;i<plugin->properties.size();i++)
		{
            vector<TimeProperty> *timevals  = plugin->properties[i].timeproperty;				
			string propertyName             = plugin->properties[i].name;
            if(propertyName		==	"alpha")
			{
                for(unsigned int h=0;h<timevals->size();h++)
				{
					
					auto tF		        =	atof(timevals->operator[](h).time.c_str());
					string temp	        =	timevals->operator[](h).value;
                    float alpha_value   =   (float)atof(temp.c_str());
                    
					SetParameter( alpha, (bv::TimeType)tF, alpha_value );
                }
            }
             
        }
        //todo: byc mo¿e przesun¹æ w miejsce odpalania timelinu
        node->EnableOverrideStateAM();

        return true;
	}

	//**********************************
    bool TreeBuilder::AttachSequencePlugin(model::BasicNodePtr node,XMLPlugin* plugin) 
	{
        PluginSequence *seq		= (PluginSequence*)plugin;
        bool result = node->AddPlugin( "DEFAULT_ANIMATION", "animation", GetTimeline(plugin->timeline) ); 
		 
        //float frame_count = (float)atof(seq->FrameCount.c_str());
        //float frame_begin = (float)atof(seq->FrameBegin.c_str());

        bv::model::DefaultAnimationPlugin* sequencePlugin = (bv::model::DefaultAnimationPlugin*)(node->GetPlugin( "animation" ).get());

        for(unsigned int i=0;i<plugin->properties.size();i++)
		{
            vector<TimeProperty> *timevals  = plugin->properties[i].timeproperty;				
			string propertyName             = plugin->properties[i].name;
            if(propertyName		==	"frames_maping")
			{
                for(unsigned int h=0;h<timevals->size();h++)
				{
					
					auto tF		        =	atof(timevals->operator[](h).time.c_str());
					string temp	        =	timevals->operator[](h).value;
                    float value         =   (float)atof(temp.c_str());
                    
                   SetParameter( sequencePlugin->GetParameter( "frameNum" ), TimeType( tF ), value );
                }
            }
             
        }

        sequencePlugin->GetParameter( "frameNum" )->SetTimeEvaluator( GetTimeline(plugin->timeline) );

        
        std::string animation_directory = BB::AssetManager::GetSequence(seq->Path);
        result = result &&  model::LoadAnimation( node->GetPlugin( "animation" ), animation_directory, "*."+seq->Extension );
        sequencePlugin->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        sequencePlugin->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;

        if(ProcessedNode->blend_mode==BLEND_MODES::ADD)
        {
            node->GetPlugin( "animation" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
            node->GetPlugin( "animation" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        }else{
			node->GetPlugin( "animation" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
            node->GetPlugin( "animation" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
		}

        return true;
	}

	//**********************************
	bool TreeBuilder::AttachTexturePlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		PluginTexture *texture		= (PluginTexture*)plugin;
        bool result = node->AddPlugin( "DEFAULT_TEXTURE", "texture", GetTimeline(plugin->timeline) ); 


        bv::model::DefaultTexturePlugin* texturePlugin =  (bv::model::DefaultTexturePlugin*)node->GetPlugin( "texture" ).get();
        texturePlugin->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        texturePlugin->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;

        string texturePath = BB::AssetManager::GetTexture(texture->Path );
             
        result = result && model::LoadTexture( node->GetPlugin( "texture" ), texturePath);
        
        for(unsigned int i=0;i<plugin->properties.size();i++)
		{
			vector<TimeProperty> *timevals = plugin->properties[i].timeproperty;
            string propertyName = plugin->properties[i].name;
            for(unsigned int h=0;h<timevals->size();h++)
			{
				
				float tF	    =	(float)	atof(timevals->operator[](h).time.c_str());
				float aF    =	(float) atof(timevals->operator[](h).value.c_str());
                if(propertyName=="alpha")
                {
                    SetParameter( node->GetPlugin( "texture" )->GetParameter( "alpha" ), tF,aF );
                }
            }
        }

		if(ProcessedNode->blend_mode==BLEND_MODES::ADD)
        {
            node->GetPlugin( "texture" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
            node->GetPlugin( "texture" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        }else{
			node->GetPlugin( "texture" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
            node->GetPlugin( "texture" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
		}

        return result;
        
	}


    //**********************************
	bool TreeBuilder::AttachVideoInputPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		PluginVideoInput *texture		= (PluginVideoInput*)plugin;
        {texture;}
        bool result = node->AddPlugin( "DEFAULT_VIDEOINPUT", "video input", GetTimeline(plugin->timeline) ); 


       auto vi = node->GetPlugin( "video input" );
       result &= vi->LoadResource( AssetDescConstPtr( new model::DefaultVideoInputResourceDescr( bv::BB::AssetManager::VideoInput->GetTexture(), bv::BB::AssetManager::VideoInput ) ) );
             
       
        return result;
        
	}
    //**********************************
    bool SetTransformParams(IParameterPtr transform, XMLPlugin* plugin, bool ZeroParam=true)
    {
        bool result = true;
        for(unsigned int i=0;i<plugin->properties.size();i++)
		{
			vector<TimeProperty> *timevals = plugin->properties[i].timeproperty;
            string propertyName = plugin->properties[i].name;
            string timelineName = plugin->properties[i].timeline;
            if(timelineName!="")
            {
                //transform->
            }
            for(unsigned int h=0;h<timevals->size();h++)
			{
				
				float tF	    =	(float)	atof(timevals->operator[](h).time.c_str());
				string point    =	timevals->operator[](h).value.c_str();

                std::vector<std::wstring> tok;
                split(tok, point, boost::is_any_of(L","));
                float x=0,y=0,z=0,angle=0;
                int count=0;
                for(std::vector<std::wstring>::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
                {
                       wstring s = *tok_iter;
                       if(count == 0)
                       {
                           x = boost::lexical_cast<float>(s);
                       }
                       if(count == 1)
                       {
                          y = boost::lexical_cast<float>(s);
                       }
                       if(count == 2)
                       {
                           z = boost::lexical_cast<float>(s);
                       }
					   if(count == 3)
                       {
                           angle = boost::lexical_cast<float>(s);
                       }
                       count++;
                }
                if(propertyName=="position")
                {
                    if(ZeroParam)
                        result = result && SetParameterTranslation (transform, 0, tF, glm::vec3(x,y,z));
                    else
                        result = result && SetParameterTranslation (transform, tF, glm::vec3(x,y,z));
                }else if(propertyName=="rotation")
                {
                     if(ZeroParam)
                     {
                         result = result && SetParameterRotation ( transform,0, tF, glm::vec3( x,y,z ), angle );
                     }
                     else
                     {
                         result = result && SetParameterRotation ( transform, tF, glm::vec3( x,y,z ), angle );
                         
                     }
                }else if(propertyName=="scaling")
                {
                    if(ZeroParam)
                        result = result && SetParameterScale (transform, 0, tF, glm::vec3(x,y,z));
                    else
                        result = result && SetParameterScale (transform, tF, glm::vec3(x,y,z));
                }else if(propertyName=="weight")
                {
                    if(ZeroParam)
                        result = SetParameterCenterMass( transform, 0, tF, glm::vec3( x,y,z ) );
                    else
                        result = result && SetParameterCenterMass (transform, tF, glm::vec3(x,y,z));
                }
                
            }
        }
        return result;
    }

	//**********************************
	bool TreeBuilder::AttachTransformPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{

        bool result = node->AddPlugin( "DEFAULT_TRANSFORM", "transform", GetTimeline(plugin->timeline) ); 

        IParameterPtr transform = node->GetPlugin("transform")->GetParameter("simple_transform");

        SetTransformParams(transform, plugin);

        return result;
	}
	
	//**********************************
	bool TreeBuilder::AttachRectanglePlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		// cast to proper object
		PluginRectangle *rect = (PluginRectangle*)plugin;
		
		// get dimensions
		float width = (float)atof(rect->width.c_str());
		float height = (float)atof(rect->height.c_str());

        if(!node->AddPlugin( "DEFAULT_RECTANGLE", "rectangle", GetTimeline(plugin->timeline) )){ return false; }
        
        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), 0.f, ((float)height)/540.0f );
        SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), 0.f, ((float)width)/540.0f );

        return true;
	}
    //**********************************
	bool TreeBuilder::AttachCubePlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		// cast to proper object
		PluginCube *rect = (PluginCube*)plugin;
		
		// get dimensions
		int tessalation = (int)atoi(rect->Tesselation.c_str());

        if(!node->AddPlugin( "DEFAULT_CUBE", "cube", GetTimeline(plugin->timeline) )){ return false; }
          auto plugin_cube = node->GetPlugin( "cube" );

          for(unsigned int h=0;h<rect->Bevel.size();h++)
		{

            TimeType time =  (TimeType)((float)atof(rect->Bevel[h].time.c_str()));

            float value = (float)atof(rect->Bevel[h].value.c_str());

            model::SetParameter( plugin_cube->GetParameter( model::DefaultCube::PN::BEVEL ),time, value);
        }
          model::SetParameter( plugin_cube->GetParameter( model::DefaultCube::PN::TESSELATION ), 0.f, tessalation );
        return true;
	}

	//**********************************
	bool TreeBuilder::AttachPrismPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		// cast to proper object
		PluginPrism *prism = (PluginPrism*)plugin;
		
		// get dimensions
		float vertices_count = (float)atof(prism->vertices_number.c_str());
		
        if(!node->AddPlugin( "DEFAULT_PRISM", "prism", GetTimeline(plugin->timeline) )){ return false; }
		SetParameter( node->GetPlugin( "prism" )->GetParameter( "n" ), 0.f, vertices_count );
        
        return true;
	}

    //**********************************
	bool TreeBuilder::AttachMaskPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		// cast to proper object
		PluginMask *rect = (PluginMask*)plugin;

        string maskPath = rect->MaskFilePath;

        if(!node->AddPlugin( "DEFAULT_ALPHA_MASK", "alpha_mask", GetTimeline(plugin->timeline) )){ return false; }
        
		bool success = model::LoadTexture( node->GetPlugin( "alpha_mask" ), BB::AssetManager::GetTexture(maskPath) );
		{ success; }
		assert( success );

        node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" )->SetTimeEvaluator( GetTimeline(plugin->timeline)  );
        
        node->GetPlugin( "alpha_mask" )->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;

        SetTransformParams(node->GetPlugin( "alpha_mask" )->GetParameter( "txAlphaMat" ), plugin,false);
     

        node->GetPlugin( "alpha_mask" )->Update( TimeType( 0.f ) ); //Regenerate all necessary geometry and channels

        return true;
	}

	//**********************************
	bool TreeBuilder::AttachRingPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{
		{ plugin; node;}
        return true;
	}


	//**********************************
	bool TreeBuilder::AttachPlugin(model::BasicNodePtr node, XMLPlugin* plugin)
		{
			// static & ugly

		string pluginName=plugin->name;

		if(pluginName=="rectangle")
		{
			//cout<<"loading rectangle plugin"<<endl;
			AttachRectanglePlugin(node,plugin);
		}else if(pluginName=="cube")
		{
			//cout<<"loading rectangle plugin"<<endl;
			AttachCubePlugin(node,plugin);
		}	
        /*
		else if(pluginName=="ring")
		{
			//cout<<"loading ring plugin"<<endl;
			AttachRingPlugin(node,plugin);
		}*/else if(pluginName=="geometry")
		{
			//cout<<"loading geometry plugin"<<endl;
			AttachTransformPlugin(node,plugin);
		}else if(pluginName=="alpha")
		{
			//cout<<"loading solid color plugin"<<endl;
			AttachAlphaPlugin(node,plugin);
		}else if(pluginName=="prism")
		{
			//cout<<"loading solid color plugin"<<endl;
			AttachPrismPlugin(node,plugin);
		}else if(pluginName=="mask")
		{
			//cout<<"loading solid color plugin"<<endl;
			AttachMaskPlugin(node,plugin);
		}else if(pluginName=="solid")
		{
			//cout<<"loading solid color plugin"<<endl;
			AttachSolidPlugin(node,plugin);
		}else if(pluginName=="texture")
		{
			//cout<<"loading texture plugin"<<endl;
			AttachTexturePlugin(node,plugin);
		}else if(pluginName=="video_input")
		{
			//cout<<"loading texture plugin"<<endl;
			AttachVideoInputPlugin(node,plugin);
		}else if(pluginName=="crawl")
		{
			//cout<<"loading text plugin"<<endl;
			AttachCrawlPlugin(node,plugin);
		}else if(pluginName=="counter")
		{
			//cout<<"loading text plugin"<<endl;
			AttachCounterPlugin(node,plugin);
		}else if(pluginName=="text")
		{
			//cout<<"loading text plugin"<<endl;
			AttachTextPlugin(node,plugin);
		}else if(pluginName=="timer")
		{
			//cout<<"loading text plugin"<<endl;
			AttachTimerPlugin(node,plugin);
		}else if(pluginName=="sequence")
		{
			//cout<<"loading text plugin"<<endl;
			AttachSequencePlugin(node,plugin);
		}		
        return false;
	}


	//**********************************
	bool TreeBuilder::AttachTextPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{		 
		PluginText * text	= (PluginText*)plugin;
		std::wstring str	= text->Text;
        std::string font = BB::AssetManager::GetFont(text->Font);
        //todo: unhack 1.25
		float size = (float)atof(text->Size.c_str());// *1.25f;
		UInt32 outlineSize = (UInt32)atoi(text->OutlineSize.c_str());
		UInt32 blurSize = (UInt32)atoi(text->BlurSize.c_str());
		string outlineColor = text->OutlineColor_RGBA;

        bool result = node->AddPlugin( "DEFAULT_TEXT", "text", GetTimeline(plugin->timeline) ); 
        
        if(!result) return false;
        bv::model::DefaultTextPlugin * txt =  (bv::model::DefaultTextPlugin*)node->GetPlugin( "text" ).get();
		

		//outline color
		vector<string> V;
															
		V = split(outlineColor,',',V);
		float rF	=	(float)	atof(V[0].c_str());
		float gF	=	(float)	atof(V[1].c_str());
		float bF	=	(float)	atof(V[2].c_str());
		float aF	=	(float)	atof(V[3].c_str());


		result = result && SetParameter( node->GetPlugin( "text" )->GetParameter( "outlineColor" ), TimeType( 0.0 ), glm::vec4( rF,gF,bF,aF) );

        //todo: unhack 1.25 size
        result = result && SetParameter( txt->GetParameter( "fontSize" ), TimeType( 0.0 ), size );
        result = result && model::LoadFont( node->GetPlugin( "text" ), font, (UInt32) size, blurSize, outlineSize, false );

		


        if(text->Align=="center")
        {
            SetParameter(txt->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center )) ;
        }else if(text->Align=="right")
        {
             SetParameter(txt->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Right )) ;
        }
        if(text->max_size!="null")
        {
            float max_size = (float)atof(text->max_size.c_str());
            SetParameter( node->GetPlugin( "text" )->GetParameter( "maxTextLenght" ), TimeType( 0.0 ), max_size);
            

        }

        //SetParameter( txt->GetParameter( "spacing" ), TimeType( 0.0 ), 0.005f*size / 28.f*0 );
        SetParameter( txt->GetParameter( "spacing" ), TimeType( 0.0 ), 0.0f );
        
        //FIXME: another set text hack
        model::DefaultTextPlugin::SetText( node->GetPlugin( "text" ), text->Text ); 
       
        bv::model::DefaultColorPlugin* color =  (bv::model::DefaultColorPlugin*)node->GetPlugin( "solid color" ).get();

		color->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        txt->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        txt->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;
        color->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;
        txt->GetPixelShaderChannel()->GetRendererContext()->cullCtx->enabled = true;
        color->GetPixelShaderChannel()->GetRendererContext()->cullCtx->enabled = true;

        return true;
	}

	//**********************************
	bool TreeBuilder::AttachCrawlPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{		 
		PluginCrawl * crawl	= (PluginCrawl*)plugin;
        std::string font = BB::AssetManager::GetFont(crawl->font_name);
        //todo: unhack 1.25
		//float size = (float)atof(crawl->size.c_str());
		float interspace = (float)atof(crawl->interspace.c_str());
		float speed = (float)atof(crawl->speed.c_str());
		int	freq = (int)atoi(crawl->promo_freq.c_str());
		int count = (int)atoi(crawl->count.c_str());
		bool autostart=false;
		if(crawl->autostart=="true")autostart=true;

		auto crawler = widgets::Crawler::Create( node.get(), mathematics::Rect::Create( -2.2f, -1.f, 2.2f, 1.f ) );

		node->SetLogic(crawler);
		

		crawler->SetSpeed( speed );
		crawler->SetPromoFrequency(freq);
		crawler->SetPromoMessage(crawl->promo_msg);
		crawler->SetInterspace( interspace );
		for(int i=0;i<count;i++)
		{

			model::BasicNodePtr separatorNode = BasicNode::Create("crawl_separator_img_"+to_string(i),timeline_default_alpha);
			AttachTransformPlugin(separatorNode,new PluginTransform());
			
			PluginRectangle* rectanglePlugin = new PluginRectangle();
			//rectanglePlugin->autosize = false;
            rectanglePlugin->width = "309";
            rectanglePlugin->height = "80";
			AttachRectanglePlugin(separatorNode,rectanglePlugin);
			
			PluginTexture *texturePlugin = new PluginTexture();
			texturePlugin->Path=crawl->separator_image;

			AttachTexturePlugin(separatorNode,texturePlugin);

			model::BasicNodePtr textNode = BasicNode::Create("Text",timeline_default_alpha);
			
			PluginTransform* geometryPlugin = new PluginTransform();
			geometryPlugin->position.push_back(TimeProperty("0",crawl->text_offset));
			AttachTransformPlugin(textNode,geometryPlugin);

			PluginSolid* solidPlugin = new PluginSolid();
			//solidPlugin->surface="white";
			solidPlugin->RGBA.push_back(TimeProperty("0",crawl->text_color));
			AttachSolidPlugin(textNode,solidPlugin);

			PluginText *textPlugin = new PluginText();
			textPlugin->Font = crawl->font_name;
			textPlugin->Align = "left";
			textPlugin->Text = L"asdas asd asda sdas dasd asd as asd Testowy Crawl "+to_wstring(i)+L" end.";
			textPlugin->Size = crawl->size;
			AttachTextPlugin(textNode,textPlugin);

			separatorNode->AddChildToModelOnly(textNode);

			crawler->AddNext(separatorNode);
		}

	    crawler->Finalize();

	    if(autostart)
		    crawler->Start();

        return true;
	}

	//**********************************
	bool TreeBuilder::AttachCounterPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{		 
		PluginCounter * counter_meta	= (PluginCounter*)plugin;

		{counter_meta;}
      
		auto counter = widgets::WidgetCounter::Create( node.get(),GetTimeline(plugin->timeline));
		node->SetLogic(counter);

		 auto counter_param = counter->GetValueParam();

		for(unsigned int i=0;i<plugin->properties.size();i++)
		{
            vector<TimeProperty> *timevals  = plugin->properties[i].timeproperty;				
			string propertyName             = plugin->properties[i].name;
            if(propertyName		==	"values")
			{
                for(unsigned int h=0;h<timevals->size();h++)
				{
					
					auto tF		        =	atof(timevals->operator[](h).time.c_str());
					string temp	        =	timevals->operator[](h).value;
                    float counter_value   =   (float)atof(temp.c_str());
                    
					SetParameter( counter_param, (bv::TimeType)tF, counter_value );
                }
            }
             
        }


		 //

        //alpha->SetTimeEvaluator(GetTimeline(plugin->timeline));

        return true;
	}


    //**********************************
	bool TreeBuilder::AttachTimerPlugin(model::BasicNodePtr node,XMLPlugin* plugin)
	{		 
		PluginTimer * text	= (PluginTimer*)plugin;
		std::wstring str	= text->Text;
        std::string font = BB::AssetManager::GetFont(text->Font);
        //todo: unhack 1.25
		float size	= (float)atof(text->Size.c_str())*1.25f;

        bool result = node->AddPlugin( "DEFAULT_TIMER", "timer", GetTimeline(plugin->timeline) ); 
        
        if(!result) return false;
        bv::model::DefaultTimerPlugin * timer =  (bv::model::DefaultTimerPlugin*)node->GetPlugin( "timer" ).get();

        //todo: unhack 1.25 size
        result = result && SetParameter( timer->GetParameter( "fontSize" ), TimeType( 0.0 ), size );
        result = result && model::LoadFont( node->GetPlugin( "timer" ),font, (UInt32) size, 0, 0, false  );

        if(text->Align=="center")
        {
            SetParameter(timer->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Center )) ;
        }else if(text->Align=="right")
        {
             SetParameter(timer->GetParameter( "alignment" ), TimeType( 0.0 ), float( TextAlignmentType::Right )) ;
        }

        if(text->ShowMS=="true")
        {
            SetParameter(timer->GetParameter( "precision" ), TimeType( 0.0 ), float( 1.f)) ;
        }else
        {
            SetParameter(timer->GetParameter( "precision" ), TimeType( 0.0 ), float( 0.f)) ;
        }

        SetParameter( timer->GetParameter( "spacing" ), TimeType( 0.0 ), 0.005f*size / 28.f*0 );
      
        bv::model::DefaultColorPlugin * color =  (bv::model::DefaultColorPlugin*)node->GetPlugin( "solid color" ).get();
        color->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        timer->GetPixelShaderChannel()->GetRendererContext()->alphaCtx->blendEnabled = true;
        timer->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;
        color->GetPixelShaderChannel()->GetRendererContext()->depthCtx->enabled = false;
        timer->GetPixelShaderChannel()->GetRendererContext()->cullCtx->enabled = true;
        color->GetPixelShaderChannel()->GetRendererContext()->cullCtx->enabled = true;

        return true;
	}

	bool TreeBuilder::AfterPlugins(model::BasicNodePtr node,BlackNode &CurrentNode)
    {
        for(unsigned int i=0;i<CurrentNode.plugins.size();i++)
		{	
            if(CurrentNode.plugins[i]->name=="rectangle")
            {
                PluginRectangle* rect = (PluginRectangle*)(CurrentNode.plugins[i]);
                if(rect->autosize=="true")
                {
            
                    auto plugin_rect = node->GetPlugin( "rectangle" );
                    auto plugin_texture =   std::dynamic_pointer_cast< model::DefaultTexturePlugin >(node->GetPlugin( "texture" ));

                    if(plugin_rect!=nullptr && plugin_texture!=nullptr)
                    {
                         float width   = (float)plugin_texture->GetTextureWidth();
                         float height  = (float)plugin_texture->GetTextureHeight();
                        
                         SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ),  height/540 );
                         SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),   width/540 );
                    }
                }
            }
        }


        // plugin rect
        return true;
    }

	//**********************************
	model::BasicNodePtr TreeBuilder::SendTree(BlackNode &CurrentNode,int depth)
	{
		PrepareNode();
        TotalNodesCount++;
		NodesLoaded++;
        printf("[%.0f] Loading node...%d/%d (%s)                            \r",100.f*(float)NodesLoaded/(float)Tree.Scene.NodeCount,NodesLoaded,Tree.Scene.NodeCount,CurrentNode.name.c_str());

        auto newNode = BasicNode::Create(CurrentNode.name,timeline_default_alpha);
        
        newNode->SetVisible(CurrentNode.visible);
        ProcessedNode = &CurrentNode;
		// attaches each plugin to node
		for(unsigned int i=0;i<CurrentNode.plugins.size();i++)
		{		
			AttachPlugin(newNode,CurrentNode.plugins[i]);
		}
        AfterPlugins(newNode,CurrentNode);

		// goes through node's children
		for(unsigned int i=0;i<CurrentNode.children.size();i++)
		{
			newNode->AddChildToModelOnly(SendTree(CurrentNode.children[i],depth+1));
		}
		return newNode;
	}	

    bool FileExists (const std::string& name) 
    {
        ifstream f(name.c_str());
        if (f.good()) {
            f.close();
            return true;
        } else {
            f.close();
            return false;
        }   
    }


	//**********************************
	//
    bool TreeBuilder::GenerateTimelines()
    {
        for(int i=0;i<Tree.Scene.Timelines.size();i++)
        {
            TimelineWrapMethod WrapMethod = TimelineWrapMethod::TWM_CLAMP;

            if( Tree.Scene.Timelines[i]->type=="loop")
            {
                WrapMethod = TimelineWrapMethod::TWM_REPEAT;
            }
            float duration = (float)atof(Tree.Scene.Timelines[i]->duration.c_str());
            DefaultTimelinePtr timeline = timelineManager->CreateDefaultTimelineImpl( Tree.Scene.Meta.SceneName+"*"+ Tree.Scene.Timelines[i]->name, TimeType( duration ), WrapMethod, WrapMethod );
            auto timeline_offset = timelineManager->CreateOffsetTimeEvaluator( Tree.Scene.Meta.SceneName+"*"+Tree.Scene.Timelines[i]->name , TimeType( 0.0 ) );
            timeline->AddChild(timeline_offset);

            for(int j=0;j<Tree.Scene.Timelines[i]->keyframes.size();j++)
            {
                BlackKeyframe *keyframe = Tree.Scene.Timelines[i]->keyframes[j];

                if(keyframe->type=="stop")
                {
                    timelineManager->AddStopEventToTimeline( timeline, keyframe->name, TimeType( keyframe->time ) );
                }
                if(keyframe->type=="goto")
                {
                    
                    timelineManager->AddLoopJumpEventToTimeline(timeline,keyframe->name, TimeType( keyframe->time ),999999, TimeType( keyframe->time2 ));
                }
            }
            timelineManager->AddTimeline(timeline);
            if(Tree.Scene.Timelines[i]->autoplay=="true")
                timeline->Play();
            
        }
        return true;
    }

    //**********************************
	//
	model::BasicNodePtr TreeBuilder::BuildTree(string path, model::TimelineManager * timelineManager)
	{
		
		printf("Loading scene from file: %s\n",path.c_str());

        this->timelineManager = timelineManager;

		auto timeline1 = this->timelineManager->GetTimeline("default");
        timeline_default = timeline1->GetChild("timeline_default");
        timeline_default_alpha = this->timelineManager->GetTimeline("default")->GetChild("timeline_default_alpha");

        

        if(FileExists(path))
        {
            Log::A("Load","info","Loading scene file "+path);
		    Tree.LoadFromFile(path);
        }else{
            model::BasicNodePtr newNode = BasicNode::Create("root",timeline_default_alpha);
            bool result = newNode->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeline_default ); 
            assert( result );
			{ result; }
			Log::A("Load","error","scene file "+path+" does not exist.");
            return newNode;
        }
        
        GenerateTimelines();
        if(Tree.Scene.Meta.DefaultTextureDirectory!="")
        {
            BB::AssetManager::SetCurrentDirectory(Tree.Scene.Meta.DefaultTextureDirectory);
        }

		BasicNodePtr root = SendTree(Tree.Scene.MainNode);
		Log::A("Load","info","XML scene has been parsed and loaded into engine");
        printf("Scene loaded\n");
        printf("Total nodes : %d\n",TotalNodesCount);
		return root;

	}
}
