#include "BasicNode.h"

#include "System/Print.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/ConstantsMapper.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Updaters/GeometryUpdater.h"
#include "Engine/Models/Updaters/TransformUpdater.h"
#include "Engine/Models/Updaters/RendererStateUpdater.h"
#include "Engine/Models/Updaters/SequenceAnimationUpdater.h"
#include "Engine/Models/Updaters/UpdatersManager.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/RenderableArrayDataElements.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/IndexBuffer.h"
#include "Engine/Graphics/Resources/Textures/TextureManager.h"
#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/TextureAnimatedSequence2D.h"

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
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
    RenderableEntity *  renderEnt   = nullptr;
    RenderableEffect *  effect      = nullptr;

    const IPlugin * finalizer = m_pluginList->GetFinalizePlugin();

    if( finalizer->GetVertexAttributesChannel() )
    {
        auto renderableType = finalizer->GetVertexAttributesChannel()->GetPrimitiveType();

        effect = CreateRenderaleEffectMockImplementationForCompleteDummies();
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
                    renderEnt = new TriangleStrip( radasvb, effect );
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
        renderEnt = new TriangleStrip( nullptr, nullptr );
    }

    bv::Transform worldTrans;

    std::vector< bv::Transform > trans;
    trans.push_back( worldTrans );

    renderEnt->SetWorldTransforms( trans );

    if ( effect ) //create only, if there is any geometry to be renderedm
    {
        //TODO: dodac liste layerow do zwracanego SceneNode
        RenderablePass * renderablePass = effect->GetPass( 0 ); //FIXME: add code to cope with more render passes
        auto pixelShader = renderablePass->GetPixelShader();

        if( finalizer->HasAnimatingTexture() ) //FIXME: this suxx, some flags should be passed here
        {
            SamplerWrappingMode wp[] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT }; 
            auto textureSampler = new TextureSampler( 0, "Animation0", bv::SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR, wp, glm::vec4( 0.f, 0.f, 1.f, 0.f ) );

            pixelShader->AddTextureSampler( textureSampler );
            TextureAnimatedSequence2D * animation = nullptr;

            unsigned int i = 0;
            for( auto tex : finalizer->GetTextures() )
            {
                auto loadedTex = bv::GTextureManager.LoadTexture( tex->m_resHandle, false );

                if( i == 0 )
                {
                    animation = new TextureAnimatedSequence2D( loadedTex->GetFormat(), loadedTex->GetType(), loadedTex->GetWidth(), loadedTex->GetHeight() );
                }

                animation->AddTexture( loadedTex );
                ++i;
            }

            assert( i > 1 );
            ShaderTextureParameters & texParams = pixelShader->Parameters()->TextureParameters();
            bool bAdded = ShaderTextureParametersAccessor::Add( texParams, animation );

            assert( bAdded );

            UpdatersManager & updatersManager = UpdatersManager::Get();

            SequenceAnimationUpdater * updater = new SequenceAnimationUpdater( animation, finalizer->QuerySequenceAnimationSource() );
            updatersManager.RegisterUpdater( updater );
        }
        else
        {
            int i = 0;
            for( auto tex : finalizer->GetTextures() )
            {
                SamplerWrappingMode wp[] = {
                                                ConstantsMapper::EngineConstant( tex->m_wrappingModeX ) 
                                            ,   ConstantsMapper::EngineConstant( tex->m_wrappingModeY )
                                            ,   SamplerWrappingMode::SWM_REPEAT // FIXME: Add 3d texture support
                                            }; 
                //FIXME: jak to kurwa przez tex->m_texName ????
                auto textureSampler = new TextureSampler(       i
                                                            ,   tex->m_texName
                                                            ,   bv::SamplerSamplingMode::SSM_MODE_2D
                                                            ,   ConstantsMapper::EngineConstant( tex->m_finteringMode )
                                                            ,   wp
                                                            ,   tex->m_texBorderColor.Evaluate( 0.f ) );
                effect->GetPass( 0 )->GetPixelShader()->AddTextureSampler( textureSampler );

                auto loadedTex = bv::GTextureManager.LoadTexture( tex->m_resHandle, false );

                ShaderTextureParameters & texParams = effect->GetPass( 0 )->GetPixelShader()->Parameters()->TextureParameters();
                bool bAdded = ShaderTextureParametersAccessor::Add( texParams, loadedTex );

                assert( bAdded );

                i++;
            }
        }

        //FIXME: Only last plugin should be used here as its output corresponds to the final transformation (list)
        //FIXME: Updater only sets proper local and world matrices for the geometry and all model transformations have already been updated at this point

        if ( true )
        {
            UpdatersManager & updatersManager = UpdatersManager::Get();

            auto transChannel = finalizer->GetTransformChannel();        
            auto vaChannel = finalizer->GetVertexAttributesChannel();
            
            assert( transChannel != nullptr );
            assert( vaChannel != nullptr );

            TransformUpdater * transformUpdater = new TransformUpdater( renderEnt, transChannel );
            updatersManager.RegisterUpdater( transformUpdater );

            if ( !vaChannel->IsTimeInvariant() )
            {
                GeometryUpdater * geometryUpdater = new GeometryUpdater( renderEnt, vaChannel );
                updatersManager.RegisterUpdater( geometryUpdater );
            }

            auto psc = finalizer->GetPixelShaderChannel();
            auto renderCtx = psc->GetRendererContext();

            assert( renderCtx );

            for( int i = 0; i < effect->NumPasses(); ++i )
            {
                auto inst = effect->GetPass( i )->GetStateInstance();

                assert( !inst->GetAlphaState() );
                assert( !inst->GetCullState() );
                assert( !inst->GetDepthState() );
                assert( !inst->GetFillState() );
                assert( !inst->GetOffsetState() );
                assert( !inst->GetStencilState() );

                RendererStatesBuilder::Create( inst, renderCtx );

                RenderStateUpdater * rendererStateUpdater = new RenderStateUpdater( inst, renderCtx );
                updatersManager.RegisterUpdater( rendererStateUpdater );
            }
        }
    }
    else
    {
        auto transChannel = finalizer->GetTransformChannel();
        assert( transChannel );

        TransformUpdater * transformUpdater = new TransformUpdater( renderEnt, transChannel );
        UpdatersManager::Get().RegisterUpdater( transformUpdater );
    }

    SceneNode * ret = new SceneNode( renderEnt );

    for( auto ch : m_children )
    {
        ret->AddChildNode( ch->BuildScene() );
    }

    return ret;
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
bool            BasicNode::AddPlugin                ( IPlugin * plugin )
{
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
bool  BasicNode::IsVisible               ( TimeType t ) const
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
PixelShader *                       BasicNode::CreatePixelShader       ()   const
{
    return CreateShader< PixelShader, IPixelShaderChannel >();
}

// ********************************
//
VertexShader *                      BasicNode::CreateVertexShader      ()   const
{
    auto vs = CreateShader< VertexShader, IVertexShaderChannel >();

    if( vs == nullptr )
    {
        vs = new PassThroughVertexShader();
    }

    return vs;
}

// ********************************
//
GeometryShader *                    BasicNode::CreateGeometryShader    () const
{
    return CreateShader< GeometryShader, IGeometryShaderChannel >();
}

// ********************************
//
RenderableEffect *                  BasicNode::CreateDefaultEffect     ( const IPlugin * finalizer ) const
{
    auto psChannel = finalizer->GetPixelShaderChannel();
    auto vsChannel = finalizer->GetVertexShaderChannel();
    auto gsChannel = finalizer->GetGeometryShaderChannel();

    assert( psChannel != nullptr );

}

// ********************************
//FIXME: reimplement someday
RenderableEffect *                  BasicNode::CreateRenderaleEffectMockImplementationForCompleteDummies   ()                                                              const
{
    RenderableEffect *  ret = new RenderableEffect();

    RenderablePass *    renderPass = new RenderablePass( CreatePixelShader(), CreateVertexShader(), CreateGeometryShader() );

    ret->AddPass( renderPass );

    return ret;
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
void                                BasicNode::RegisterShaderParameters ( const IShaderChannel * shaderChannel, ShaderParameters * shParams )
{
    for( auto value : shaderChannel->GetValues() )
    {
        GenericShaderParam * genShaderParam = ShaderParamFactory::CreateGenericParameter( value );
        assert( genShaderParam != nullptr );

        shParams->AddParameter( genShaderParam );
    }
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
