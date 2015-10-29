#include "BVSceneTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/DefaultEffect.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Updaters/NodeUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffects/NodeMaskNodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffects/AlphaMaskNodeEffect.h"


namespace bv {

// *******************************
// 
void                BVSceneTools::ClearSingleNode                       ( SceneNode * node, Renderer * renderer )
{
    renderer->FreeAllPDResources( static_cast< RenderableEntity * >( node->GetTransformable() ) );
    node->DeleteTransformable();
}

// *******************************
//
void                BVSceneTools::SyncSingleNode                        ( model::BasicNodePtr modelNode, SceneNode * node )
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
SceneNode *         BVSceneTools::BuildEngineSceneNode                  ( model::BasicNodePtr modelNode, std::hash_map< model::IModelNode *, SceneNode * > & nodesMapping )
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
void                BVSceneTools::UpdateSceneNodeEffect                 ( SceneNode * node, model::BasicNodePtr modelNode )
{
    auto modelNodeEffect = modelNode->GetNodeEffect();

    if ( !modelNodeEffect || modelNodeEffect->GetType() == NodeEffectType::NET_DEFAULT )
    {
        auto sceneNodeDefaultEffect = std::make_shared< NodeEffect >( NodeEffect::Type::T_DEFAULT );
        node->SetNodeEffect( sceneNodeDefaultEffect );
    }
    else if( modelNodeEffect->GetType() == NodeEffectType::NET_ALPHA_MASK )
    {
        auto sceneNodeAMEffect = std::make_shared< AlphaMaskNodeEffect >();
        node->SetNodeEffect( sceneNodeAMEffect );
    }
    else if( modelNodeEffect->GetType() == NodeEffectType::NET_NODE_MASK )
    {
        auto sceneNodeNMEffect = std::make_shared< NodeMaskNodeEffect >();
        node->SetNodeEffect( sceneNodeNMEffect );
    }
    else
    {
        //Did you forget to implement an additional effect
        assert( false );
    }
}

// *******************************
//
SceneNode *         BVSceneTools::BuildSingleEngineNode                 ( model::BasicNodePtr modelNode )
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
RenderableEntity *  BVSceneTools::CreateRenderableEntity                ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer )
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

    auto worldTransformVec = CreateTransformVec( finalizer );

    renderable->SetWorldTransforms( worldTransformVec );

    return renderable;
}

// ********************************
//
RenderableEffectPtr     BVSceneTools::CreateDefaultEffect               ( const model::IPluginConstPtr & finalizer )
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
TransformVec            BVSceneTools::CreateTransformVec                ( const model::IPluginConstPtr & finalizer )
{
    auto tc = finalizer->GetTransformChannel();
    assert( tc );

    auto numTransforms = tc->GetTransformValues().size();
    assert( numTransforms > 0 );

    std::vector< Transform > worldTransformVec;

    for( unsigned int i = 0; i < numTransforms; ++i )
    {
        Transform worldTrans;
        worldTransformVec.push_back( worldTrans );
    }

    return worldTransformVec;
}

// *******************************
//
BVSceneTools::RADASVB * BVSceneTools::CreateRenderableArrayDataTriStrip ( model::BasicNodePtr modelNode, const model::IPluginConstPtr & finalizer )
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

    if( components.empty() )
    {
        return nullptr;
    }
    
    return CreateRenderableArrayDataArrays( components, geomDesc, vaChannel->IsTimeInvariant() );
}

// ********************************
//
BVSceneTools::RADASVB * BVSceneTools::CreateRenderableArrayDataArrays   ( const std::vector< model::IConnectedComponentPtr > & ccVec, const model::IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant )
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
unsigned int            BVSceneTools::TotalNumVertices                  ( const std::vector< model::IConnectedComponentPtr > & ccVec )
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
VertexDescriptor *      BVSceneTools::CreateVertexDescriptor            ( const model::IVertexAttributesChannelDescriptor * desc )
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
void                    BVSceneTools::AddVertexDataToVBO                    ( char * data, model::IConnectedComponentPtr cc )
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

} //bv
