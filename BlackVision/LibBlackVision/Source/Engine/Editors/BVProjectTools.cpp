#include "stdafx.h"

#include "Engine/Editors/BVProject.h"
#include "BVProjectTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/SceneGraph/Lines.h"

#include "Engine/Graphics/Effects/DefaultEffect.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Updaters/NodeUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Editors/AssetsLoading/AssetTracker.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffectFactory.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Engine/Models/BoundingVolume.h"



#include "Memory/MemoryLeaks.h"



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

    auto audio = CreateAudioEntity( finalizer );
    node->SetAudio( audio );

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

    engineNode->SetBoundingBox( modelNode->GetBoundingVolume()->GetBoundingBox() );

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
RenderableEntity *  BVProjectTools::BuildRenderableFromComponent        ( model::IConnectedComponentPtr cc, PrimitiveType type )
{
    auto attrChan = cc->GetAttributeChannels()[ 0 ];

    auto compDesc = std::dynamic_pointer_cast< const model::AttributeChannelDescriptor >( attrChan->GetDescriptor() );

    auto vaDesc = new model::VertexAttributesChannelDescriptor();
    vaDesc->AddAttrChannelDesc( compDesc );

    std::vector< model::IConnectedComponentPtr > components; components.push_back( cc );

    RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataArrays( components, vaDesc, false );

    assert( radasvb );

    delete vaDesc; //not used anymore

    if( type == PrimitiveType::PT_TRIANGLE_STRIP )
        return new TriangleStrip( radasvb, nullptr );
    else if( type == PrimitiveType::PT_LINES )
        return new Lines( radasvb, nullptr, 2.f );
    else
    {
        assert( false );
        return nullptr;
    }
}

// *******************************
//
Scene *             BVProjectTools::AddEngineScene                          ( BVProject * project, model::SceneModelPtr modelScene, model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping, UInt32 idx )
{
    auto scene = new Scene();
    
    auto root = BuildEngineSceneNode( modelNode, nodesMapping );
    scene->SetRoot( root );

    project->AddEngineScene( SceneUPtr( scene ), idx );

    return scene;
}

// *******************************
//
void                BVProjectTools::UpdateSceneNodeEffect                 ( SceneNode * node, model::BasicNodePtr modelNode )
{
    auto modelNodeEffect = modelNode->GetNodeEffect();

    // FIXME: nrl - make sure that this code works as expected
    if( modelNodeEffect )
    {
        auto nNodeEffectType = modelNodeEffect->GetType();

        node->SetNodeEffect( CreateNodeEffect( nNodeEffectType ) );
    }
    else
    {
        auto nodeEffectType = NodeEffectType::NET_DEFAULT;
        node->SetNodeEffect( CreateNodeEffect( nodeEffectType ) );
    }
}
// *******************************
//
void                BVProjectTools::ReleaseUnusedResources                  ( Renderer * renderer, AssetTracker * assetTracker )
{
    auto assetUIDs = assetTracker->GetUnusedAssetUIDs();
    for( auto assetUID : assetUIDs )
    {
        //FIXME: for now only texture2d are used
        auto tex = GTexture2DCache.GetTexture( assetUID );
        if( tex )
        {
            renderer->DeletePDR( tex.get() );
        }
    }
}

// *******************************
//
SceneNode *         BVProjectTools::BuildSingleEngineNode                 ( model::BasicNodePtr modelNode )
{
    model::IPluginConstPtr finalizer = modelNode->GetFinalizePlugin();

    RenderableEntity * renderable = CreateRenderableEntity( modelNode, finalizer );
    
    SceneNode * node        = new SceneNode( renderable );

    auto audio = CreateAudioEntity( finalizer );
    node->SetAudio( audio );

    NodeUpdaterPtr updater  = NodeUpdater::Create( node, modelNode );
    UpdatersManager::Get().RegisterUpdater( modelNode.get(), updater );

    return node;
}

namespace
{
    void    MakeReversible( glm::mat4 & matrix )
    {
        // If scale component of matrix is 0, replace it with minimal float number near 0.

        if( matrix[ 0 ][ 0 ] == 0.0 )
            matrix[ 0 ][ 0 ] = std::numeric_limits< float >::epsilon();
        if( matrix[ 1 ][ 1 ] == 0.0 )
            matrix[ 1 ][ 1 ] = std::numeric_limits< float >::epsilon();
        if( matrix[ 2 ][ 2 ] == 0.0 )
            matrix[ 2 ][ 2 ] = std::numeric_limits< float >::epsilon();
    }

}   //  annonymous


// ***********************
//
std::pair< model::BasicNodePtr, Float32 >   BVProjectTools::NodeIntersection    ( model::BasicNodePtr modelNode, glm::mat4 & parentInverseTrans, glm::vec3 & rayPoint, glm::vec3 & rayDir )
{
    glm::mat4 transform;
    auto transPlugin = modelNode->GetPlugin( "transform" );

    if( transPlugin != nullptr )
    {
        auto transformParam = modelNode->GetFinalizePlugin()->GetParamTransform();
        
        if( transformParam != nullptr )
            transform = transformParam->Evaluate();
        else
            transform = glm::mat4( 1 ); //  Identity matrix
    }

    MakeReversible( transform );
    glm::mat4 inverseTransform = glm::inverse( transform );

    inverseTransform = inverseTransform * parentInverseTrans;
    glm::vec3 rayLocalDir = glm::normalize( glm::vec3( inverseTransform * glm::vec4( rayDir, 0.0f ) ) );
    glm::vec3 rayLocalPos = glm::vec3( inverseTransform * glm::vec4( rayPoint, 1.0f ) );

    std::pair< model::BasicNodePtr, Float32 > result = std::make_pair< model::BasicNodePtr, Float32 >( nullptr, std::numeric_limits< Float32 >::infinity() );

    Float32 distance = -std::numeric_limits< Float32 >::infinity();
    const mathematics::Box * boundingBox;

    bool rayPointInside = false;
    auto boundingVolume = modelNode->GetBoundingVolume();
    if( boundingVolume != nullptr )
    {
        boundingBox = boundingVolume->GetBoundingBox();
        distance = boundingBox->RayIntersection( rayLocalPos, rayLocalDir, rayPointInside );

		distance = glm::length( transform * glm::vec4( distance * rayLocalDir, 0.0f ) );
    }

    if( distance >= 0.0 || rayPointInside )
    {
        result.second = distance;
        result.first = modelNode;
    }


    for( UInt32 i = 0; i < modelNode->GetNumChildren(); ++i )
    {
        auto childNode = modelNode->GetChild( i );

        if( childNode->IsVisible() )
        {
            auto newPair = NodeIntersection( childNode, inverseTransform, rayPoint, rayDir );
            if( newPair.second <= result.second && result.second >= 0 && newPair.second != std::numeric_limits< float >::infinity() )
            {
                result.first = newPair.first;
                result.second = newPair.second;
            }
        }
    }

    return result;
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
                    renderable = new TriangleStrip( radasvb, effect );
                }
                break;
            }
            case PrimitiveType::PT_TRIANGLES:
            {
                RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip( modelNode, finalizer );

                if( radasvb )
                {
                    renderable = new Triangles( radasvb, effect );
                }
                break;
            }
            case PrimitiveType::PT_LINES:
            {
                RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip( modelNode, finalizer );

                if( radasvb )
                {
                    renderable = new Lines( radasvb, effect, 3.f ); // FIXME this width should be stored in VAO somehow
                }
                break;
            }
            case PrimitiveType::PT_TRIANGLE_MESH:
                assert( false );
            default:
                return nullptr;
        }
    }
    else
    {
        renderable = new TriangleStrip( nullptr, nullptr );
    }


    assert( finalizer->GetTransformChannel() );
    
    return renderable;
}

// ********************************
//
audio::AudioEntity *    BVProjectTools::CreateAudioEntity                 ( const model::IPluginConstPtr & finalizer )
{
    auto audioChannel = finalizer->GetAudioChannel();
    if( audioChannel )
    {
        return new audio::AudioEntity( audioChannel->GetFrequency(), audioChannel->GetFormat() );
    }

    return nullptr;
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

    char * vbData = vertexBuffer->Data(); //FIXME: THIS SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!!

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
        auto channelDesc = desc->GetAttrChannelDescriptor( i );

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

        auto basePlugin = std::static_pointer_cast< model::BasePlugin >( plugin );
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
    timelineParamInfo.pluginName = "";
    timelineParamInfo.paramSubName = "";

    auto effect = modelNode->GetNodeEffect();

    if( effect != nullptr )
    {
        for( auto param : effect->GetParameters() )
        {
            timelineParamInfo.param = param;

            auto te = param->GetTimeEvaluator();
            assert( map.find( te ) != map.end() ); // a little bit of defensive programming
            map[ te ].push_back( timelineParamInfo );
        }
    }

    timelineParamInfo.paramOwner = ParamOwner::PO_NodeLogic;

    auto logic = modelNode->GetLogic();
    if( logic != nullptr )
    {
        for( auto param : logic->GetParameters() )
        {
            timelineParamInfo.param = param;

            auto te = param->GetTimeEvaluator();
            assert( map.find( te ) != map.end() ); // a little bit of defensive programming
            map[ te ].push_back( timelineParamInfo );
        }
    }

    // Process node children.
    for( unsigned int i = 0; i < modelNode->GetNumChildren(); i++ )
    {
        std::string childNodePath = nodePath + "/#" + Convert::T2String( i );
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

// ***********************
//
void                 BVProjectTools::UpdateEffectAssetData               ( SceneNode * node, model::BasicNodePtr modelNode )
{
    auto effect = node->GetNodeEffect();
    auto modelEffect = modelNode->GetNodeEffect();

    if( modelEffect != nullptr )
    {
        assert( modelEffect->NumRequiredAssets() == modelEffect->GetAssets().size() );
        for( auto ad : modelEffect->GetAssets() )
        {
            if( ad->GetUID() == TextureAssetDesc::UID() )
            {
                //FIXME: nrl update
                //auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( ad );
                //auto defTexDesc = model::DefaultTextureDescriptor::LoadTexture( txAssetDescr, "" );

                //defTexDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

                //effect->AddTexture( defTexDesc );
            }
            else
            {
                assert( !"Only textures assets can be added to FSE." );
            }
        }
    }
}

} //bv
