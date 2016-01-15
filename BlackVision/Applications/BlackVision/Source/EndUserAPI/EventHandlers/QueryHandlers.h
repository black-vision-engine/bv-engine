#pragma once

#include "Engine/Events/Events.h"
#include "Serialization/Json/JsonSerializeObject.h"

namespace bv
{

class BVAppLogic;

class QueryHandlers
{
private:

    BVAppLogic *        m_appLogic;

public:

    explicit            QueryHandlers   ( BVAppLogic * appLogic );
                        ~QueryHandlers  ();

    void                Info            ( bv::IEventPtr evt );

private:

    void     ListSceneAssets         ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     ListProjectNames        ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     ListScenes              ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     ListAssetsPaths         ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     ListCategoriesNames     ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     ListProjects            ( JsonSerializeObject & ser, const std::string & request, int eventID );

    void     ListAllFolders          ( JsonSerializeObject & ser, const std::string & request, int eventID );
    //void     ListResourcesInFolders  ( JsonSerializeObject & ser, const std::string & request, int eventID );
    //void     ListAllResources        ( JsonSerializeObject & ser, const std::string & request, int eventID );

    void     VideoCardsInfo          ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     GetNodeInfo             ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     GetMinimalSceneInfo     ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     GetTimeLinesInfo        ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     PerformanceInfo         ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     TreeStructureInfo       ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     CheckTimelineTime       ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     PluginInfo              ( JsonSerializeObject & ser, const std::string & request, int eventID );
    
    void     ListTimelineKeyframes   ( JsonSerializeObject & ser, const std::string & request, int eventID );
    void     MinimalTreeStructureInfo( JsonSerializeObject & ser, const std::string & request, int eventID );

};

} //bv
