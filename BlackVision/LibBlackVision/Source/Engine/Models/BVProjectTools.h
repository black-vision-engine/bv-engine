#pragma once

#include <hash_map>

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include "Engine/Graphics/Types/Transform.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Renderer;
class SceneNode;
class RenderableEntity;
class RenderableArrayDataArraysSingleVertexBuffer;
class VertexDescriptor;


typedef std::vector< Transform >    TransformVec;

class BVProjectTools
{
private:

    typedef     RenderableArrayDataArraysSingleVertexBuffer RADASVB;

public:

    static  void                ClearSingleNode                     ( SceneNode * node, Renderer * renderer );
    static  void                SyncSingleNode                      ( model::BasicNodePtr modelNode, SceneNode * node );

    static  SceneNode *         BuildEngineSceneNode                ( model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping );

    static void                 UpdateSceneNodeEffect               ( SceneNode * node, model::BasicNodePtr modelNode );
    
    static void                 ReleaseUnusedResources              ( Renderer * renderer );

private:

    static  SceneNode *         BuildSingleEngineNode               ( model::BasicNodePtr modelNode );

    static  RenderableEntity *  CreateRenderableEntity              ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer );

    static  RenderableEffectPtr CreateDefaultEffect                 ( const model::IPluginConstPtr & finalizer );

    static  RADASVB *           CreateRenderableArrayDataTriStrip   ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer );
    static  RADASVB *           CreateRenderableArrayDataArrays     ( const std::vector< model::IConnectedComponentPtr > & ccVec, const model::IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant );

    static unsigned int         TotalNumVertices                    ( const std::vector< model::IConnectedComponentPtr > & ccVec );
    static VertexDescriptor *   CreateVertexDescriptor              ( const model::IVertexAttributesChannelDescriptor * desc );
    static void                 AddVertexDataToVBO                  ( char * data, model::IConnectedComponentPtr cc );

};

} // bv