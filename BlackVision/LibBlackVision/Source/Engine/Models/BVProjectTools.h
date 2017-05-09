#pragma once

#include <hash_map>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Models/SceneModel.h"

#include "Engine/Graphics/Types/Transform.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class BVProject;
class Renderer;
class SceneNode;
class RenderableEntity;
class RenderableArrayDataArraysSingleVertexBuffer;
class VertexDescriptor;
class Scene;
class UniformBuffer;
class AssetTracker;

namespace audio {
    class AudioEntity;
}

typedef std::vector< model::Transform >                                                    TransformVec;


enum ParamOwner : int
{
    PO_Plugin,
    PO_GlobalEffect,
    PO_Resource,
    PO_NodeLogic,
    PO_Node,
    //PO_Light,

    PO_Total
};

struct TimelineParamInfo
{
    std::string             nodePath;
    std::string             pluginName;
    std::string             paramSubName;
    model::IParameterPtr    param;
    ParamOwner              paramOwner;
};

typedef std::map< model::ITimeEvaluatorPtr, std::vector< TimelineParamInfo > >   ParamsOfTimelinesMap;

class BVProjectTools
{
private:

    typedef     RenderableArrayDataArraysSingleVertexBuffer RADASVB;

public:

    static  void                ClearSingleNode                     ( SceneNode * node, Renderer * renderer );
    static  void                SyncSingleNode                      ( model::BasicNodePtr modelNode, SceneNode * node );

    static  SceneNode *         BuildEngineSceneNode                ( model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping );
    static  RenderableEntity *  BuildRenderableFromComponent        ( model::IConnectedComponentPtr cc, PrimitiveType type );

    static void                 UpdateSceneNodeEffect               ( SceneNode * node, model::BasicNodePtr modelNode );
    
    static void                 ReleaseUnusedResources              ( Renderer * renderer, AssetTracker * assetTracker );

    static ParamsOfTimelinesMap GetParamsOfTimelines                ( model::BasicNodePtr modelNode, model::ITimeEvaluatorPtr sceneTimeline );

    static  std::pair< model::BasicNodePtr, Float32 >   NodeIntersection    ( model::BasicNodePtr modelNode, glm::mat4 & parentInverseTrans, glm::vec3 & rayPoint, glm::vec3 & rayDir );

    static  Scene *             AddEngineScene                      ( BVProject * project,  model::SceneModelPtr modelScene, model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping, UInt32 idx );

private:

    static  SceneNode *         BuildSingleEngineNode               ( model::BasicNodePtr modelNode );

    static  RenderableEntity *  CreateRenderableEntity              ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer );
    
    static audio::AudioEntity * CreateAudioEntity                   ( const model::IPluginConstPtr & finalizer );

    static  RenderableEffectPtr CreateDefaultEffect                 ( const model::IPluginConstPtr & finalizer );

    static  RADASVB *           CreateRenderableArrayDataTriStrip   ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer );
    static  RADASVB *           CreateRenderableArrayDataArrays     ( const std::vector< model::IConnectedComponentPtr > & ccVec, const model::IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant );

    static unsigned int         TotalNumVertices                    ( const std::vector< model::IConnectedComponentPtr > & ccVec );
    static VertexDescriptor *   CreateVertexDescriptor              ( const model::IVertexAttributesChannelDescriptor * desc );
    static void                 AddVertexDataToVBO                  ( char * data, model::IConnectedComponentPtr cc );

    static void                 UpdateEffectAssetData               ( SceneNode * node, model::BasicNodePtr modelNode );

};

} // bv
