#include "stdafx.h"

#include "BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/DefaultEffect.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Updaters/NodeUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/AssetTracker.h"

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectFactory.h"

#include "Engine/Models/BoundingVolume.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Plugin.h"

namespace bv {

// *******************************
// 
void                BVProjectTools::ClearSingleNode                       ( SceneNode * node, Renderer * renderer )
{
    renderer->FreeAllPDResources( static_cast< RenderableEntity * >( node->GetTransformable() ) );
    node->DeleteTransformable();
}

// *******************************
//
void                BVProjectTools::SyncSingleNode                        ( model::BasicNodePtr modelNode, SceneNode * node )
{
    model::IPluginConstPtr finalizer = modelNode->GetFinalizePlugin();
    RenderableEntity * renderable = CreateRenderableEntity( modelNode, finalizer );

    node->SetTransformable( renderable );

    if( UpdatersManager::Get().IsRegistered( modelNode.get() ) )
    {
        UpdatersManager::Get().RemoveNodeUpdater( modelNode.get() );
    }

    NodeUpdaterPtr updater  = NodeUpdater::Create( node, modelNode );
    UpdatersManager::Get().RegisterUpdater( modelNode.get(), updater );
}

// *******************************
//
SceneNode *         BVProjectTools::BuildEngineSceneNode                  ( model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping )
{
    assert( nodesMapping.find( modelNode.get() ) == nodesMapping.end() );

    SceneNode * engineNode = BuildSingleEngineNode( modelNode );

    nodesMapping[ modelNode.get() ] = engineNode;

    UpdateSceneNodeEffect( engineNode, modelNode );

    for( unsigned int i = 0; i < modelNode->GetNumChildren(); ++i )
    {
        auto modelChildNode     = modelNode->GetChild( i );
        auto engineChildNode    = BuildEngineSceneNode( modelChildNode, nodesMapping );

        engineNode->AddChildNode( engineChildNode );
    }

    return engineNode;
}

// *******************************
//
void                BVProjectTools::UpdateSceneNodeEffect                 ( SceneNode * node, model::BasicNodePtr modelNode )
{
    auto modelNodeEffect = modelNode->GetNodeEffect();
    auto nodeEffectType = NodeEffectType::NET_DEFAULT;

    if ( modelNodeEffect )
    {
        nodeEffectType = modelNodeEffect->GetType();
    }

    node->SetNodeEffect( CreateNodeEffect( nodeEffectType ) );
}

// *******************************
//
void                BVProjectTools::ReleaseUnusedResources                  ( Renderer * renderer )
{
    auto assets = AssetTracker::Instance().GetUnusedAssets();
    for( auto asset : assets )
    {
        //FIXME: for now only texture2d are used
        auto tex = std::static_pointer_cast< const Texture2D >( asset );
        assert( tex );
        renderer->DeletePDR( tex.get() );
    }
}

// *******************************
//
SceneNode *         BVProjectTools::BuildSingleEngineNode                 ( model::BasicNodePtr modelNode )
{
    model::IPluginConstPtr finalizer = modelNode->GetFinalizePlugin();

    RenderableEntity * renderable = CreateRenderableEntity( modelNode, finalizer );

    SceneNode * node        = new SceneNode( renderable );
    NodeUpdaterPtr updater  = NodeUpdater::Create( node, modelNode );
    UpdatersManager::Get().RegisterUpdater( modelNode.get(), updater );

    return node;
}

// *******************************
//
RenderableEntity *  BVProjectTools::CreateRenderableEntity                ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer )
{
    RenderableEntity * renderable = nullptr;

    if( finalizer->GetVertexAttributesChannel() )
    {
        auto renderableType = finalizer->GetVertexAttributesChannel()->GetPrimitiveType();

        RenderableEffectPtr effect = CreateDefaultEffect( finalizer );

        //RenderableArrayDataSingleVertexBuffer * rad = CreateRenderableArrayData( renderableType );
        //CreateRenderableData( &vao ); // TODO: Powinno zwracac indeksy albo vao w zaleznosci od rodzaju geometrii
        //effect = ;

        //FIXME: to powinna ogarniac jakas faktoria-manufaktura
        switch( renderableType )
        {
            case PrimitiveType::PT_TRIANGLE_STRIP:
            {
                //FIXME: it should be constructed as a proper type RenderableArrayDataArraysSingleVertexBuffer * in the first place
                //FIXME: this long type name suggests that something wrong is happening here (easier to name design required)
                RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip( modelNode, finalizer );

                if( radasvb ) 
                {
                    renderable = new TriangleStrip( radasvb, modelNode->GetBoundingVolume()->GetBoundingBox(), effect );
                }
                break;
            }
            case PrimitiveType::PT_TRIANGLES:
            case PrimitiveType::PT_TRIANGLE_MESH:
                assert( false );
            default:
                return nullptr;
        }
    }
    else
    {
        renderable = new TriangleStrip( nullptr, nullptr, nullptr );
    }


    assert( finalizer->GetTransformChannel() );
    
    return renderable;
}

// ********************************
//
RenderableEffectPtr     BVProjectTools::CreateDefaultEffect               ( const model::IPluginConstPtr & finalizer )
{
    auto psChannel      = finalizer->GetPixelShaderChannel();
    auto vsChannel      = finalizer->GetVertexShaderChannel();
    auto gsChannel      = finalizer->GetGeometryShaderChannel();

    assert( psChannel != nullptr );
    assert( vsChannel != nullptr );

    return std::make_shared< DefaultEffect >( psChannel.get(), vsChannel.get(), gsChannel.get() ); 
}

// *******************************
//
BVProjectTools::RADASVB * BVProjectTools::CreateRenderableArrayDataTriStrip ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer )
{
    assert( modelNode->GetNumPlugins() > 0 );

    if( modelNode->GetNumPlugins() == 0 )
    {
        return nullptr;
    }

    auto vaChannel = finalizer->GetVertexAttributesChannel();

    if( vaChannel == nullptr )
    {
        return nullptr;
    }

    auto components = vaChannel->GetComponents();
    auto geomDesc = vaChannel->GetDescriptor();

    //if( components.empty() )
    //{
    //    return nullptr;
    //}
    
    return CreateRenderableArrayDataArrays( components, geomDesc, vaChannel->IsTimeInvariant() );
}

// ********************************
//
BVProjectTools::RADASVB * BVProjectTools::CreateRenderableArrayDataArrays   ( const std::vector< model::IConnectedComponentPtr > & ccVec, const model::IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant )
{
    //FIXME: a bit of hackery because memory layout may be different than what this constructor suggests (this time it is not)
    //FIXME: this code should be moved to some utility classes from this poor BasicNode (not so basic right now)
    //FIXME: check that plugin's channel signature is the same for all connected components

    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_STATIC;

    if ( !isTimeInvariant )
    {
        vbSemantic = DataBuffer::Semantic::S_DYNAMIC;
    }

    VertexBuffer * vertexBuffer         = new VertexBuffer( TotalNumVertices( ccVec ), desc->SingleVertexEntrySize(), vbSemantic );
    VertexDescriptor * vertexDescriptor = CreateVertexDescriptor( desc );

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vertexBuffer, vertexDescriptor );
    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    char * vbData = vertexBuffer->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P

    unsigned int currentOffset = 0;

    for( auto cc : ccVec )
    {
        assert( !cc->GetAttributeChannels().empty() );

        vao->AddCCEntry( cc->GetNumVertices() );

        AddVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * desc->SingleVertexEntrySize();
    }

    return rad;
}

// ********************************
//
unsigned int            BVProjectTools::TotalNumVertices                  ( const std::vector< model::IConnectedComponentPtr > & ccVec )
{
    unsigned int totalNumVertices = 0;

    for( auto cc : ccVec )
    {
        totalNumVertices += cc->GetNumVertices();
    }

    return totalNumVertices;
}

// ********************************
//
VertexDescriptor *      BVProjectTools::CreateVertexDescriptor            ( const model::IVertexAttributesChannelDescriptor * desc )
{
    VertexDescriptor * vertexDescriptor = new VertexDescriptor( desc->GetNumVertexChannels() );

    unsigned int attributeOffset = 0;

    for( unsigned int i = 0; i < desc->GetNumVertexChannels(); ++i )
    {
        auto * channelDesc = desc->GetAttrChannelDescriptor( i );

        //FIXME: default channel location just copied from model channel ordering (maybe it should be a permutation or something)
        vertexDescriptor->SetAttribute( i, i, attributeOffset, channelDesc->GetType(), channelDesc->GetSemantic() );
        attributeOffset += channelDesc->GetEntrySize();     
    }

    vertexDescriptor->SetStride( attributeOffset );

    return vertexDescriptor;
}

// ********************************
//
void                    BVProjectTools::AddVertexDataToVBO                    ( char * data, model::IConnectedComponentPtr cc )
{
    unsigned int numVertices = cc->GetNumVertices();
    unsigned int offset = 0;

    for( unsigned int i = 0; i < numVertices; ++i )
    {
        for( auto vach : cc->GetAttributeChannels() )
        {
            assert( vach->GetNumEntries() == numVertices );

            auto eltSize = vach->GetDescriptor()->GetEntrySize();
            const char * eltData = vach->GetData();

            memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

            offset += eltSize;
        }
    }
}

namespace {

// ***********************
//
void GetParamsOfTimelinesRecursively                ( const std::string & nodePath, model::BasicNodePtr modelNode, ParamsOfTimelinesMap & map )
{
    auto params = modelNode->GetParameters();
    
    // Adding plugin parameters to map.
    TimelineParamInfo timelineParamInfo;
    timelineParamInfo.nodePath = nodePath;

    auto pluginListFinalize = modelNode->GetPlugins();
    for( UInt32 i = 0; i < pluginListFinalize->NumPlugins(); ++i )
    {
        auto plugin = pluginListFinalize->GetPlugin( i );
        timelineParamInfo.pluginName = plugin->GetName();

        // Begin with plugin model parameters.
        timelineParamInfo.paramOwner = ParamOwner::PO_Plugin;

        for( auto & param : plugin->GetParameters() )
        {
            timelineParamInfo.param = param;

            auto te = param->GetTimeEvaluator();
            assert( map.find( te ) != map.end() ); // a little bit of defensive programming
            map[ te ].push_back( timelineParamInfo );
        }

        // Add plugin resource parameters.
        timelineParamInfo.paramOwner = ParamOwner::PO_Resource;

        auto basePlugin = std::static_pointer_cast< model::BasePlugin< model::IPlugin > >( plugin );
        auto assets = basePlugin->GetLAssets();

        for( auto & asset : assets )
        {
            timelineParamInfo.paramSubName = asset.name;

            for( auto & param : asset.rsm->GetParameters() )
            {
                timelineParamInfo.param = param;

                auto te = param->GetTimeEvaluator();
                assert( map.find( te ) != map.end() ); // a little bit of defensive programming
                map[ te ].push_back( timelineParamInfo );
            }
        }
    }

    // Add node effect parameters.
    timelineParamInfo.paramOwner = ParamOwner::PO_GlobalEffect;

    auto effect = modelNode->GetNodeEffect();

    if( effect != nullptr )
    {
        for( auto param : effect->GetParameters() )
        {
            auto te = param->GetTimeEvaluator();
            assert( map.find( te ) != map.end() ); // a little bit of defensive programming
            map[ te ].push_back( timelineParamInfo );
        }
    }

    // Process node children.
    for( unsigned int i = 0; i < modelNode->GetNumChildren(); i++ )
    {
        std::string childNodePath = nodePath + "/#" + SerializationHelper::T2String( i );
        GetParamsOfTimelinesRecursively( childNodePath, modelNode->GetChild( i ), map );
    }
}

} // anonymous

// ***********************
//
ParamsOfTimelinesMap BVProjectTools::GetParamsOfTimelines                ( model::BasicNodePtr modelNode, model::ITimeEvaluatorPtr sceneTimeline )
{
    ParamsOfTimelinesMap ret;
    std::string nodePath = modelNode->GetName();

    ret[ sceneTimeline ];
    for( auto timeline : sceneTimeline->GetChildren() )
    {
        ret[ timeline ];
    }

    GetParamsOfTimelinesRecursively( nodePath, modelNode, ret );

    return ret;
}


} //bv
