#include "Solution.h"
#include "structure/AssetManager.h"
#include "UseLogger.h"
#include <iostream>

using namespace std;
namespace bv
{
	Solution::Solution(model::TimelineManager * timelineManager)
	{
		 this->timelineManager = timelineManager;
	}

    bool Solution::LoadSolution(std::string solution)
    {
		ifstream f(BB::AssetManager::GetSolution(solution).c_str());
		if (f.good()) {
			
		}
		else {
			f.close();
			LOG_MESSAGE( SeverityLevel::error ) << "Error LoadSolution() solution [" + BB::AssetManager::GetSolution(solution) + "] not found";
			return false;
		}

        string temp = get_file_contents(BB::AssetManager::GetSolution(solution).c_str());
	
        xml_document<>          doc;    // character type defaults to char
        char *buffer            =   new char[temp.size()+1];
        buffer[temp.size()]     =   0;

        memcpy(buffer,temp.c_str(),temp.size());
        doc.parse<0>(buffer);    
	    
        // parse solution file
        xml_node<> *Solution = doc.first_node("solution");
        xml_attribute<> *name   = Solution->first_attribute("name");   
        if(name!=NULL)
        {
            BB::AssetManager::SetCurrentProject(name->value());
        }

        if(Solution==NULL)return false;
        xml_node<> *Scenes = Solution->first_node("scenes");
        
        if(Scenes==NULL)return false;
        xml_node<> *Scene = Scenes->first_node("scene");
        while(Scene!=NULL)
        {
            string isNameOk=Scene->name();
            if(isNameOk != "scene")
            {
                Scene = Scene->next_sibling();
                continue;
            }
            xml_attribute<> *name   = Scene->first_attribute("name");    
            
            ScenesToLoad.push_back(name->value());
            Scene = Scene->next_sibling();
        }

        delete[] buffer;

        // create scenes

        auto TimeLineDefault =model::TimelineHelper::CreateDefaultTimelineImpl( "default", TimeType( 50.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
        timelineManager->AddStopEventToTimeline( TimeLineDefault, "stop1", TimeType( 0.0 ) );
        auto timeline_default = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline_default" , TimeType( 0.0 ) );
        auto timeline_default_alpha = model::TimelineHelper::CreateOffsetTimeEvaluator( "timeline_default_alpha" , TimeType( 0.0 ) );
        TimeLineDefault->AddChild( timeline_default );
        TimeLineDefault->AddChild( timeline_default_alpha );
        timelineManager->AddTimeline(TimeLineDefault);
		
		//pablito: register root timeline ?
		//timelineManager->RegisterRootTimeline

        // root node

        root = BasicNode::Create( "root",timeline_default_alpha );

        for(unsigned int i=0;i<ScenesToLoad.size();i++)
        {
             TreeBuilder *XMLTree = new TreeBuilder();

            //auto scene_1 = XMLTree->BuildTree(BB::AssetManager::GetScene("15_wizytowka.xml"),timelineManager);
            auto scene = XMLTree->BuildTree(BB::AssetManager::GetScene(ScenesToLoad[i]),timelineManager);

            root->AddChildToModelOnly(scene);
        }

        return true;
    }

	Solution::~Solution(void)
	{
	}

}
