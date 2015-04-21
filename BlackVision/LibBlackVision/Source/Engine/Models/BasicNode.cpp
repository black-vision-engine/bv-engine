#include "BasicNode.h"

//FIXME: node na INode
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Engine/Models/Updaters/SequenceAnimationUpdater.h"
#include "Engine/Models/Updaters/NodeUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"

#include "Engine/Graphics/Effects/DefaultEffect.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h" //FIXME: ten kod, ktory potrzebuje tego deskryptora, tekstur i animacji powinien byc w default effect lub cos, a nie tutaj - to do przerobienia koniecznie
#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Models/BasicOverrideState.h"


namespace bv { namespace model {

// FIXME: hack
std::hash_map< IModelNode *, SceneNode * >    BasicNode::ms_nodesMapping;

namespace {

IModelNodePtr  FindNode( const TNodeVec & vec, const std::string & name )
{
    for( auto node : vec )
    {
        if( node->GetName() == name )
        {
            return node;
        }
    }

    return nullptr;
}

} //anonymous

// ********************************
//
BasicNode::BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
    : m_name( name )
    , m_pluginList( nullptr )
    , m_pluginsManager( pluginsManager )
    , m_overrideState( nullptr )
    , m_visible( true )
{
    if( pluginsManager == nullptr )
    {
        m_pluginsManager = &PluginsManager::DefaultInstance();
    }

    m_overrideState = new BasicOverrideState( timeEvaluator );
}

// ********************************
//
BasicNode::~BasicNode()
{
    delete m_overrideState;
}

// ********************************
//
BasicNodePtr                    BasicNode::Create                   ( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
{
    struct make_shared_enabler_BasicNode : public BasicNode
    {
        make_shared_enabler_BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
            : BasicNode( name, timeEvaluator, pluginsManager )
        {
        }
    };

    return std::make_shared<make_shared_enabler_BasicNode>( name, timeEvaluator, pluginsManager );
}    

// ********************************
//
IPluginPtr                      BasicNode::GetPlugin                ( const std::string & name ) const
{
    return m_pluginList->GetPlugin( name );
}

// ********************************
//
IFinalizePluginConstPtr BasicNode::GetFinalizePlugin        () const
{
    return m_pluginList->GetFinalizePlugin();
}

// ********************************
//
IModelNodePtr           BasicNode::GetNode                  ( const std::string & path, const std::string & separator )
{
    std::string suffix = path;

    auto name = SplitPrefix( suffix, separator );

    if( name == "" || name == GetName() )
    {
        if( suffix.size() > 0 )
        {
            return GetNode( suffix, separator );
        }
        else
        {
            return shared_from_this();
        }
    }
    else
    {
        auto child = GetChild( name );

        if( child != nullptr )
        {
            return child->GetNode( suffix );
        }
    }

    return nullptr;
}

// ********************************
//
IModelNodePtr                   BasicNode::GetChild                 ( const std::string & name )
{
    return FindNode( m_children, name );
}

// ********************************
//
const IPluginListFinalized *    BasicNode::GetPluginList            () const
{
    return m_pluginList.get();
}

// ********************************
//
IModelNodePtr                   BasicNode::DeleteNode               ( const std::string & name, Renderer * renderer )
{
    { name; }
    { renderer; }

    // FIXME: implement
    return nullptr;
}

// ********************************
//
void                            BasicNode::AddChildNode             ( IModelNodePtr modelNode )
{
    // Verify validity
    assert( modelNode != nullptr );
    assert( ms_nodesMapping.find( modelNode.get() ) == ms_nodesMapping.end() );
    assert( ms_nodesMapping.find( this ) != ms_nodesMapping.end() );

    // Create engine node corresponding to modelNode
    BasicNode * basicModelNode = static_cast< BasicNode * >( modelNode.get() );
    SceneNode * engineNode = basicModelNode->BuildScene();

    // Register created node and its mapping
    ms_nodesMapping[ basicModelNode ] = engineNode;

    // Add model node to current tree along with corresponding engine node
    AddChildToModelOnly( std::static_pointer_cast< BasicNode >( modelNode ) );
    ms_nodesMapping[ this ]->AddChildNode( engineNode );
}

// ********************************
//
void                            BasicNode::EnableOverrideStateAM    ()
{
    m_overrideState->EnableAlphaAM();
}

// ********************************
//
void                            BasicNode::EnableOverrideStateNM    ()
{
    m_overrideState->EnableAlphaNM();
}

// ********************************
//
void                            BasicNode::DisableOverrideStateAM   ()
{
    m_overrideState->DisableAlphaAM();
}

// ********************************
//
void                            BasicNode::DisableOverrideStateNM   ()
{
    m_overrideState->DisableAlphaNM();
}

// ********************************
//
bool                            BasicNode::OverrideStateChangedAM   () const
{
    return m_overrideState->ChangedAM();
}

// ********************************
//
bool                            BasicNode::OverrideStateChangedNM   () const
{
    return m_overrideState->ChangedNM();
}

// ********************************
//
void                            BasicNode::SetOverrideStateChgAM    ( bool changed )
{
    return m_overrideState->SetChangedAM( changed );
}

// ********************************
//
void                            BasicNode::SetOverrideStateChgNM    ( bool changed )
{
    return m_overrideState->SetChangedNM( changed );
}

// ********************************
//
bool                            BasicNode::IsStateOverridenAM       () const
{
    return m_overrideState->IsAlphaEnabledAM();
}

// ********************************
//
bool                            BasicNode::IsStateOverridenNM       () const
{
    return m_overrideState->IsAlphaEnabledNM();
}

// ********************************
//
IOverrideState *                BasicNode::GetOverrideState         ()
{
    return m_overrideState;
}

// ********************************
//
const std::string &             BasicNode::GetName                  () const
{
    return m_name;
}

// ********************************
//
void                            BasicNode::SetName                  ( const std::string & name )
{
    m_name = name;
}

// ********************************
//
mathematics::Rect 			    BasicNode::GetAABB			        () const
{
	mathematics::Rect r;

	auto trans = m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate( 0 );

	auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

	if( plRect )
		r.Include( *plRect );


	for( auto ch : m_children )
	{
		r.Include( ch->GetAABB( trans ) );
	}

	return r;
}

// ********************************
//
mathematics::Rect 			BasicNode::GetAABB						( const glm::mat4 & parentTransformation ) const
{
	mathematics::Rect r;

	auto trans = parentTransformation * m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate( 0 );

	auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

	if( plRect )
		r.Include( *plRect );


	for( auto ch : m_children )
	{
		r.Include( ch->GetAABB( trans ) );
	}

	return r;
}

// ********************************
//
SceneNode *                 BasicNode::BuildScene                   () 
{
    assert( ms_nodesMapping.find( this ) == ms_nodesMapping.end() );

    SceneNode * node = CreateSceneNode();

    ms_nodesMapping[ this ] = node;

    node->SetOverrideAlphaVal( GetOverrideState()->GetAlphaValue().get() );

    for( auto ch : m_children )
    {
        node->AddChildNode( ch->BuildScene() );
    }

    return node;
}

// ********************************
//
void            BasicNode::AddChildToModelOnly              ( BasicNodePtr n )
{
    m_children.push_back( n );
}

// ********************************
//
void            BasicNode::SetPlugins                       ( DefaultPluginListFinalizedPtr plugins )
{
    m_pluginList = plugins;
}

// ********************************
//
void             BasicNode::NonNullPluginsListGuard ()
{
    if( !m_pluginList )
    {
        m_pluginList = std::make_shared< DefaultPluginListFinalized >();
    }
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( IPluginPtr plugin )
{
    NonNullPluginsListGuard();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

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
bool            BasicNode::AddPlugin                        ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator )
{
    NonNullPluginsListGuard ();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
		throw ( uid + " cannot be attached to " + prev->GetTypeUid() ); //FIXME: we do not use exceptions !!!!
    }

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, prev, timeEvaluator ) );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin                    ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    NonNullPluginsListGuard ();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
        return false;
    }

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, name, prev, timeEvaluator ) );

    return true;
}

// ********************************
//
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator )
{
    for( auto uid : uids )
    {
        if( !AddPlugin( uid, timeEvaluator ) )
        {
            return false;
        }
    }

    return true;
}

// ********************************
//
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator )
{
    if( uids.size() != names.size() )
    {
        return false;
    }

    for( unsigned int i = 0; i < names.size(); ++i )
    {
        if( !AddPlugin( uids[ i ], names[ i ], timeEvaluator ) )
        {
            return false;
        }
    }

    return true;
}

// ********************************
//
void			BasicNode::SetLogic					( INodeLogicPtr logic )
{
	m_nodeLogic = logic;
}

// ********************************
//
void BasicNode::Update( TimeType t )
{
    if( IsVisible() )
    {
        m_overrideState->Update( t );

        m_pluginList->Update( t );

		if( m_nodeLogic )
			m_nodeLogic->Update( t );

        for( auto ch : m_children )
            ch->Update( t );
    }
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
SceneNode *                         BasicNode::CreateSceneNode          () const
{
    IPluginConstPtr finalizer = GetFinalizePlugin();

    RenderableEntity * renderable = CreateRenderable( finalizer );

    SceneNode * node        = new SceneNode( renderable );
    NodeUpdaterPtr updater  = NodeUpdater::Create( node, shared_from_this() );
    UpdatersManager::Get().RegisterUpdater( updater );

    return node;
}

// ********************************
//
RenderableEntity *                  BasicNode::CreateRenderable         ( IPluginConstPtr finalizer ) const
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
std::vector< bv::Transform >        BasicNode::CreateTransformVec      ( IPluginConstPtr finalizer ) const
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

    auto components = GetFinalizePlugin()->GetVertexAttributesChannel()->GetComponents();

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

    // int channelLoc = 0;

    for( auto attrCh : attribChannels )
    {
        auto desc       = attrCh->GetDescriptor();
        
        // FIXME: WTF - why vd is created here
        // VertexDescriptor *   vd = VertexDescriptor::Create( 1, channelLoc++, desc->GetType(), desc->GetSemantic(), (int)desc->GetSemantic());
        VertexBuffer *       vb = new VertexBuffer( vertNum, desc->GetEntrySize() );

        vb->WriteToBuffer( attrCh->GetData(), attrCh->GetNumEntries() * desc->GetEntrySize() );

        //(*vao)->AddEntry( vb, vd );
    }

    return true;
}

// ********************************
//
RenderableEffectPtr                  BasicNode::CreateDefaultEffect     ( IPluginConstPtr finalizer ) const
{
    auto psChannel      = finalizer->GetPixelShaderChannel();
    auto vsChannel      = finalizer->GetVertexShaderChannel();
    auto gsChannel      = finalizer->GetGeometryShaderChannel();

    assert( psChannel != nullptr );
    assert( vsChannel != nullptr );

    return std::make_shared<DefaultEffect>( psChannel.get(), vsChannel.get(), gsChannel.get() ); 
}


// ********************************
//
std::string                         BasicNode::SplitPrefix              ( std::string & str, const std::string & separator ) const
{
    assert( separator.length() == 1 );

    std::vector< std::string > ret;

    //FIXME: _SCL_SECURE_NO_WARNINGS is defined due to this fuckin line (or fuckin VS)
    boost::split( ret, str, boost::is_any_of( separator ) );

    if( ret.size() == 0 )
    {
        return "";
    }
    else if ( ret.size() == 1 )
    {
        str = "";

        return ret[ 0 ];
    }

    str = boost::algorithm::join( std::vector< std::string >( ret.begin() + 1, ret.end() ), separator );

    return ret[ 0 ];
}

// ********************************
//
RenderableArrayDataSingleVertexBuffer * BasicNode::CreateRenderableArrayData( PrimitiveType type ) const
{
    if( m_pluginList->NumPlugins() == 0 )
    {
        return nullptr;
    }

    auto vaChannel = GetFinalizePlugin()->GetVertexAttributesChannel();

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

    auto vaChannel = GetFinalizePlugin()->GetVertexAttributesChannel();

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
RenderableArrayDataArraysSingleVertexBuffer * BasicNode::CreateRenderableArrayDataArrays( const std::vector< IConnectedComponentPtr > & ccVec, const IVertexAttributesChannelDescriptor * desc, bool isTimeInvariant ) const
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
void                            BasicNode::AddVertexDataToVBO              ( char * data, IConnectedComponentPtr cc ) const
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
unsigned int                        BasicNode::TotalNumVertices       ( const std::vector< IConnectedComponentPtr > & ccVec ) const
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
unsigned int                        BasicNode::TotalSize             ( const std::vector< IConnectedComponentPtr > & ccVec, const IVertexAttributesChannelDescriptor * desc ) const
{
    return TotalNumVertices( ccVec ) * desc->SingleVertexEntrySize();
}

// ********************************
//
void            BasicNode::Print                    ( std::ostream& , int ) const
{}

} // model
} // bv
