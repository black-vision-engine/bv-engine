#include "BasicNode.h"

//FIXME: node na INode

#include "System/Print.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/ConstantsMapper.h"

#include "Engine/Models/Updaters/SequenceAnimationUpdater.h"
#include "Engine/Models/Updaters/NodeUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"

#include "Engine/Graphics/Effects/DefaultEffect.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h" //FIXME: ten kod, ktory potrzebuje tego deskryptora, tekstur i animacji powinien byc w default effect lub cos, a nie tutaj - to do przerobienia koniecznie
#include "Engine/Graphics/Resources/Textures/TextureManager.h"
#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"


namespace bv { namespace model {

namespace {

const IModelNode *  FindNode( const TNodeVec & vec, const std::string & name )
{
    for( auto node : vec )
    {
        if( node->GetName() == name )
        {
            return node.get();
        }
    }

    return nullptr;
}

} //anonymous

// ********************************
//
BasicNode::BasicNode( const std::string & name,  const PluginsManager * pluginsManager )
    : m_name( name )
    , m_pluginList( nullptr )
    , m_pluginsManager( pluginsManager )
{
    if( pluginsManager == nullptr )
    {
        m_pluginsManager = &PluginsManager::DefaultInstance();
    }

}

// ********************************
//
BasicNode::~BasicNode()
{
}

// ********************************
//
const IPlugin *                 BasicNode::GetPlugin               ( const std::string & name ) const
{
    return m_pluginList->GetPlugin( name );
}

// ********************************
//
const IModelNode *              BasicNode::GetChild                ( const std::string & name ) const
{
    return FindNode( m_children, name );
}

// ********************************
//
const IModelNode *              BasicNode::GetLayer                ( const std::string & name ) const
{
    return FindNode( m_layers, name );
}

// ********************************
//
const IPluginListFinalized *    BasicNode::GetPluginList           () const
{
    return m_pluginList.get();
}

// ********************************
//
const std::string &             BasicNode::GetName                 () const
{
    return m_name;
}
                                                                 
// ********************************
//
SceneNode *                 BasicNode::BuildScene()
{
    RenderableEntity *  renderable  = CreateRenderable( m_pluginList->GetFinalizePlugin() );

    SceneNode * retNode         = new SceneNode( renderable );
    NodeUpdater * nodeUpdater   = new NodeUpdater( renderable, retNode, this );
    UpdatersManager::Get().RegisterUpdater( nodeUpdater );

    for( auto ch : m_children )
    {
        retNode->AddChildNode( ch->BuildScene() );
    }

    return retNode;
}

// ********************************
//
void            BasicNode::AddChild                 ( IModelNode * n )
{
    m_children.push_back( IModelNodePtr( n ) );
}

// ********************************
//
void            BasicNode::AddLayer                 ( IModelNode * n )
{
    m_layers.push_back( IModelNodePtr( n ) );
}

// ********************************
//
void            BasicNode::SetPlugins              ( DefaultPluginListFinalizedPtr plugins )
{
    m_pluginList = plugins;
}

// ********************************
//
void             BasicNode::NonNullPluginsListGuard ()
{
    if( !m_pluginList )
    {
        m_pluginList = DefaultPluginListFinalizedPtr( new DefaultPluginListFinalized() );
    }
}

// ********************************
//
bool            BasicNode::AddPlugin                ( IPlugin * plugin )
{
    NonNullPluginsListGuard();

    const IPlugin * prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    assert( m_pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev ) );

    if( !m_pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev ) )
    {
        return false;
    }

    m_pluginList->AttachPlugin( plugin );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin               ( IPluginPtr plugin )
{
    return AddPlugin( plugin.get() );
}

// ********************************
//
bool            BasicNode::AddPlugin               ( const std::string & uid )
{
    NonNullPluginsListGuard ();

    const IPlugin * prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
        return false;
    }

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, prev ) );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin               ( const std::string & uid, const std::string & name )
{
    NonNullPluginsListGuard ();

    const IPlugin * prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
        return false;
    }

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, name, prev ) );

    return true;
}

// ********************************
//
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids )
{
    for( auto uid : uids )
    {
        if( !AddPlugin( uid ) )
        {
            return false;
        }
    }

    return true;
}

// ********************************
//
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids, const std::vector< std::string > & names )
{
    if( uids.size() != names.size() )
    {
        return false;
    }

    for( unsigned int i = 0; i < names.size(); ++i )
    {
        if( !AddPlugin( uids[ i ], names[ i ] ) )
        {
            return false;
        }
    }

    return true;
}

// ********************************
//
void BasicNode::Update( TimeType t )
{
    for( auto l : m_layers )
        l->Update( t );

    m_pluginList->Update( t );

    for( auto ch : m_children )
        ch->Update( t );
}

// ********************************
//
bool  BasicNode::IsVisible               () const
{
    return m_visible;
}

// ********************************
//
void  BasicNode::SetVisible              ( bool visible )
{
    m_visible = visible;
}

// ********************************
//
RenderableEntity *                  BasicNode::CreateRenderable         ( const IPlugin * finalizer ) const
{
    RenderableEntity * renderable = nullptr;

    if( finalizer->GetVertexAttributesChannel() )
    {
        auto renderableType = finalizer->GetVertexAttributesChannel()->GetPrimitiveType();

        RenderableEffect * effect = CreateDefaultEffect( finalizer );

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
                RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip();

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
std::vector< bv::Transform >        BasicNode::CreateTransformVec      ( const IPlugin * finalizer ) const
{
    auto tc = finalizer->GetTransformChannel();
    assert( tc );

    auto numTransforms = tc->GetTransformValues().size();
    assert( numTransforms > 0 );

    std::vector< bv::Transform > worldTransformVec;

    for( unsigned int i = 0; i < numTransforms; ++i )
    {
        bv::Transform worldTrans;
        worldTransformVec.push_back( worldTrans );
    }

    return worldTransformVec;
}

// ********************************
//
bool                                BasicNode::CreateRenderableData     (/* VertexArray ** vao*/ ) const
{
    if( m_pluginList->NumPlugins() == 0 )
    {
        return false;
    }

    auto components = m_pluginList->GetFinalizePlugin()->GetVertexAttributesChannel()->GetComponents();

    if( components.empty() )
    {
        return nullptr;
    }

    auto lastComponent = components.back();

    auto vertNum    = lastComponent->GetNumVertices();

    auto attribChannels = lastComponent->GetAttributeChannels();

    if( attribChannels.empty() )
    {
        return nullptr;
    }

    VertexArray ** vao = { nullptr };
    assert( false );
    *vao = new VertexArray();

    int channelLoc = 0;

    for( auto attrCh : attribChannels )
    {
        auto desc       = attrCh->GetDescriptor();
        
        VertexDescriptor*   vd = VertexDescriptor::Create( 1, channelLoc++, desc->GetType(), desc->GetSemantic(), (int)desc->GetSemantic());
        VertexBuffer*       vb = new VertexBuffer( vertNum, desc->GetEntrySize() );

        vb->WriteToBuffer( attrCh->GetData(), attrCh->GetNumEntries() * desc->GetEntrySize() );

        //(*vao)->AddEntry( vb, vd );
    }

    return true;
}

// ********************************
//
RenderableEffect *                  BasicNode::CreateDefaultEffect     ( const IPlugin * finalizer ) const
{
    auto psChannel = finalizer->GetPixelShaderChannel();
    auto vsChannel = finalizer->GetVertexShaderChannel();
    auto gsChannel = finalizer->GetGeometryShaderChannel();

    assert( psChannel != nullptr );
    assert( vsChannel != nullptr );

    return new DefaultEffect( psChannel, vsChannel, gsChannel ); 
}

// ********************************
//
RenderableArrayDataSingleVertexBuffer * BasicNode::CreateRenderableArrayData( PrimitiveType type ) const
{
    if( m_pluginList->NumPlugins() == 0 )
    {
        return nullptr;
    }

    auto vaChannel = m_pluginList->GetFinalizePlugin()->GetVertexAttributesChannel();

    auto components = vaChannel->GetComponents();
    auto geomDesc = vaChannel->GetDescriptor();

    if( components.empty() )
    {
        return nullptr;
    }

    switch( type )
    {
        case PrimitiveType::PT_TRIANGLE_STRIP:
            return CreateRenderableArrayDataArrays( components, geomDesc, vaChannel->IsTimeInvariant() );
        case PrimitiveType::PT_TRIANGLES:
        case PrimitiveType::PT_TRIANGLE_MESH:
            //FIXME: implement
            assert( false );
        default:
            return nullptr;
    }
}

// ********************************
//
RenderableArrayDataArraysSingleVertexBuffer *   BasicNode::CreateRenderableArrayDataTriStrip   () const
{
    if( m_pluginList->NumPlugins() == 0 )
    {
        return nullptr;
    }

    auto vaChannel = m_pluginList->GetFinalizePlugin()->GetVertexAttributesChannel();

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
RenderableArrayDataArraysSingleVertexBuffer * BasicNode::CreateRenderableArrayDataArrays( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant ) const
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
void                            BasicNode::AddVertexDataToVBO              ( char * data, IConnectedComponent * cc ) const
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

// ********************************
//
VertexDescriptor *                  BasicNode::CreateVertexDescriptor          ( const IVertexAttributesChannelDescriptor * desc ) const
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
unsigned int                        BasicNode::TotalNumVertices       ( const std::vector< IConnectedComponent * > & ccVec ) const
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
unsigned int                        BasicNode::TotalSize             ( const std::vector< IConnectedComponent * > & ccVec, const IVertexAttributesChannelDescriptor * desc ) const
{
    return TotalNumVertices( ccVec ) * desc->SingleVertexEntrySize();
}


// ********************************
//
void            BasicNode::Print                    ( std::ostream& out, int tabs ) const
{
    out << "------------------NODE-------------------- : " << this << debug::EndLine(tabs);
    //out << "Transform plugins: " << m_transformPlugins.size() << debug::EndLine(tabs + 1);
    //for(auto tp : m_transformPlugins)
    //    tp->Print(out, tabs + 1);
    
    //out << debug::EndLine(tabs) << "Pixel Shader plugin: " << m_pshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_pshaderPlugin != nullptr)
    //    m_pshaderPlugin->Print(out, tabs + 1);

    //out << debug::EndLine(tabs) << "Vertex Shader plugin: " << m_vshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_vshaderPlugin != nullptr)
    //    m_vshaderPlugin->Print(out, tabs + 1);

    //out << debug::EndLine(tabs) << "Geometry Shader plugin: " << m_gshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_gshaderPlugin != nullptr)
    //    m_gshaderPlugin->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "------------------PARENT OF---------------- : " << m_children.size() << debug::EndLine( tabs + 1 );
    for( auto ch : m_children )
    {
        ch->Print( out, tabs + 1 );
        out << debug::EndLine( tabs + 1 );
    }
}

} // model
} // bv
