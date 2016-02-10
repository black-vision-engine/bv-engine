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

    void     ListSceneAssets         ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListProjectNames        ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListScenes              ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListPresets             ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListAssetsPaths         ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListCategoriesNames     ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     ListProjects            ( JsonSerializeObject & ser, IDeserializer & request, int eventID );

    void     ListAllFolders          ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    //void     ListResourcesInFolders  ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    //void     ListAllResources        ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     GetAssetDescriptor      ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     GetAssetThumbnail       ( JsonSerializeObject & ser, IDeserializer & request, int eventID );

    void     VideoCardsInfo          ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     GetNodeInfo             ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     GetMinimalSceneInfo     ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     GetTimeLinesInfo        ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     PerformanceInfo         ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     TreeStructureInfo       ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     CheckTimelineTime       ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     PluginInfo              ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    
    void     ListTimelineKeyframes   ( JsonSerializeObject & ser, IDeserializer & request, int eventID );
    void     MinimalTreeStructureInfo( JsonSerializeObject & ser, IDeserializer & request, int eventID );

};

} //bv
