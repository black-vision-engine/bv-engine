#pragma once

#include "Engine/Events/Events.h"
#include "Serialization/Json/JsonSerializeObject.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"


#include "AsyncThumbs/ThumbsThread.h"




namespace bv
{

class BVAppLogic;
class BVProjectEditor;

class QueryHandlers
{
private:

    BVAppLogic *        m_appLogic;
    BVProjectEditor *   m_editor;

    ThumbsThread        m_thumbsThread;

public:

    explicit            QueryHandlers   ( BVAppLogic * appLogic );
                        ~QueryHandlers  ();

    void                Info            ( bv::IEventPtr evt );

private:

    void     ListSceneAssets         ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListProjectNames        ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListScenes              ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListPresets             ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListAssetsPaths         ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListCategoriesNames     ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListProjects            ( JsonSerializeObject & ser, IDeserializer * request, int eventID );

    void     ListAllFolders          ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    //void     ListResourcesInFolders  ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    //void     ListAllResources        ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetAssetDescriptor      ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetAssetThumbnail       ( JsonSerializeObject & ser, IDeserializer * request, int eventID, int socketID );
    void     GetSceneThumbnail       ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetPresetThumbnail      ( JsonSerializeObject & ser, IDeserializer * request, int eventID );

    void     VideoCardsInfo          ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetNodeInfo             ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetMinimalSceneInfo     ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetTimeLinesInfo        ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     PerformanceInfo         ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     TreeStructureInfo       ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     CheckTimelineTime       ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     PluginInfo              ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     EffectInfo              ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     LogicInfo               ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ParamInfo               ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    
    void     ListTimelineKeyframes   ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     ListTimelinesParams     ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     MinimalTreeStructureInfo( JsonSerializeObject & ser, IDeserializer * request, int eventID );

    void     GetLightsInfo           ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetCamerasInfo          ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetPMItemStats          ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    
    void     ListParamDescriptors    ( JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void     GetParamDescriptor      ( JsonSerializeObject & ser, IDeserializer * request, int eventID );


    void     RenderingPerformance    ( JsonSerializeObject & ser );
    void     RenderingPerformance    ( JsonSerializeObject & ser, model::IModelNodePtr modelNode, SceneNode * sceneNode );
};

} //bv
