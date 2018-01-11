#include "stdafx.h"

#include "NodeUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Models/ModelState.h"
#include "Engine/Models/NodeEffects/ModelNodeEffect.h"


#include "Memory/MemoryLeaks.h"


namespace bv 
{

// *****************************
//
NodeUpdaterPtr NodeUpdater::Create( SceneNode * sceneNode, model::IModelNodeConstPtr modelNode )
{
    struct make_shared_enabler_NodeUpdater : public NodeUpdater
    {
        make_shared_enabler_NodeUpdater( SceneNode * sceneNode, model::IModelNodeConstPtr modelNode )
            : NodeUpdater( sceneNode, modelNode )
        {
        }
    };

    return std::make_shared<make_shared_enabler_NodeUpdater>( sceneNode, modelNode );
}

// *****************************
//
NodeUpdater::NodeUpdater     ( SceneNode * sceneNode, model::IModelNodeConstPtr modelNode )
    : m_sceneNode( sceneNode )
    , m_modelNode( modelNode )
    , m_renderable( nullptr )
    , m_rendererContext( nullptr )
    , m_attributesUpdateID( 0 )
    , m_topologyUpdateID( 0 )
    , m_audio( nullptr )
{
    assert( sceneNode != nullptr );
    assert( modelNode != nullptr );
    
    m_renderable = static_cast< RenderableEntity* >( sceneNode->GetTransformable() );
    assert( m_renderable != nullptr );

    m_timeInvariantVertexData = false;

    model::IPluginConstPtr finalizer = modelNode->GetFinalizePlugin();
    assert( finalizer );

    auto effect = m_renderable->GetRenderableEffect();

    m_transformChannel = finalizer->GetTransformChannel();
    assert( m_transformChannel != nullptr );
    m_transformStatedValue = model::IStatedValuePtr( new model::SimpleState< glm::mat4 >() );

    m_audio = sceneNode->GetAudio();
    m_audioChannel = finalizer->GetAudioChannel();

    if( effect )
    {
        m_vertexAttributesChannel = finalizer->GetVertexAttributesChannel();
        assert( m_vertexAttributesChannel != nullptr );

        m_timeInvariantVertexData = m_vertexAttributesChannel->IsTimeInvariant();

        m_rendererContext = finalizer->GetPixelShaderChannel()->GetRendererContext();
        assert( m_rendererContext );

        auto psc = finalizer->GetPixelShaderChannel();
        auto vsc = finalizer->GetVertexShaderChannel();
        auto gsc = finalizer->GetGeometryShaderChannel();

        //FIXME: tutaj jak widac jest petla po numpasses, ale m_rendererContext, psc, vsc i gsc sa tylko pobierane raz (czyli dziala tylko dla 1 passa) - jak bedzie wiecej passow, trzeba to poprawic
        for( unsigned int i = 0; i < effect->NumPasses(); ++i )
        {
            auto pass = effect->GetPass( i );

            auto inst = pass->GetStateInstance();

            assert( !inst->GetAlphaState() );
            assert( !inst->GetCullState() );
            assert( !inst->GetDepthState() );
            assert( !inst->GetFillState() );
            assert( !inst->GetOffsetState() );
            assert( !inst->GetStencilState() );

            RendererStatesBuilder::Create( inst, m_rendererContext );

            m_redererStateInstanceVec.push_back( inst );

            RegisterTexturesData( psc, vsc, gsc, pass );

            RegisterShaderParams( vsc, pass->GetVertexShader() );
            RegisterShaderParams( psc, pass->GetPixelShader() );
            RegisterShaderParams( gsc, pass->GetGeometryShader() );
        
        }
    }

    m_isDrawable = effect != nullptr;
}

// *****************************
//
NodeUpdater::~NodeUpdater    ()
{
}

// *****************************
//
void    NodeUpdater::DoUpdate               ( EngineResources & resources )
{
    //FIXME: czy jesli node nie jest widoczne to trzeba w ogole updatowac stan - zakladam, ze nie, ale trzeba sie upewnic
    //FIXME: it is just a single bool to set, so no there is no fancy machinery for testing whehter any update is necessary 
    if( m_modelNode->IsVisible() )
    {
        m_sceneNode->SetVisible( true );

        // Add, when all mechanisms are implemented
        UpdateTransform();

        if( m_isDrawable )
        {
            UpdateVACPtr();

            if( !m_timeInvariantVertexData )
            {
                UpdateGeometry();
            }

            UpdateNodeEffect();
            UpdateShaderParams();

            UpdateTexturesData( resources );
            UpdateRendererState();
        }
        else
        {
            UpdateNodeEffect();
        }

        UpdateAudio();
    }
    else
    {
        m_sceneNode->SetVisible( false );
    }
}

// *****************************
//
void    NodeUpdater::UpdateNodeEffect       ()
{
    auto nodeEffect = m_modelNode->GetNodeEffect();
    if( nodeEffect )
    {
        auto sceneNodeEffect = m_sceneNode->GetNodeEffect();

        if( sceneNodeEffect )
        {
            if( nodeEffect->IsEnabled() )
            {
                m_sceneNode->EnableNodeEffect();

                for( auto & val : nodeEffect->GetValues() )
                {
                    auto vSrc = sceneNodeEffect->GetValue( val->GetName() );
                    if( vSrc )
                        UpdateValue( val, vSrc );
                    else if( val->GetName() != model::ModelNodeEffect::EFFECT_ENABLED_PARAM_NAME )
                        LOG_MESSAGE( SeverityLevel::warning ) << "Cannot find value [" << val->GetName() << "] in effect state.";
                        
                }
            }
            else
            {
                m_sceneNode->DisableNodeEffect();
            }
        }
        else
        {
            // FIXME: nrl transition
            assert( false );
        }
    }
}

// *****************************
//
void    NodeUpdater::RegisterTexturesData   ( IShaderDataSourceConstPtr psTxData, IShaderDataSourceConstPtr vsTxData, IShaderDataSourceConstPtr gsTxData, RenderablePass * pass )
{
    RegisterTypedTexturesData( psTxData, pass->GetPixelShader() );
    RegisterTypedTexturesData( vsTxData, pass->GetVertexShader() );
    RegisterTypedTexturesData( gsTxData, pass->GetGeometryShader() );
}

// *****************************
//
bool   NodeUpdater::MustBeRegistered        ( IShaderDataSourceConstPtr shaderDataSrc, ShaderParameters * shaderParams )
{
    if( shaderDataSrc == nullptr || shaderParams == nullptr )
    {
        return false;
    }

    auto txData = shaderDataSrc->GetTexturesData();

    if( txData == nullptr )
    {
        return false;
    }

    auto textures = txData->GetTextures();
    auto animations = txData->GetAnimations();

    if( textures.size() == 0 && animations.size() == 0 )
    {
        return false;
    }

    assert( ( animations.size() + textures.size() ) == shaderParams->NumTextures() );

    return true;
}

// *****************************
//
void            NodeUpdater::RegisterTex2Params  ( ITexturesDataConstPtr texturesData, ShaderParameters * shaderParams )
{
    auto textures = texturesData->GetTextures();
    auto animations = texturesData->GetAnimations();

    if( textures.size() != 0 || animations.size() != 0 )
    {
        m_texDataMappingVec.push_back( std::make_pair( texturesData, shaderParams ) );

        std::vector< UInt64 > updateIDs( textures.size() + animations.size(), 0 );
        m_texDataUpdateID.push_back( updateIDs );
    }
/*
    auto textures = texturesData->GetTextures();
    auto animations = texturesData->GetAnimations();

    //FIXME: make sure that textures and animations from model are passed in this exact order (textures first and animations next)
    unsigned int j = 0;
    for( unsigned int i = 0; i < textures.size(); ++i, ++j )
    {
        auto tex2D = static_cast< Texture2DImpl * >( shaderParams->GetTexture( j ) );
        m_texMappingVec.push_back( std::make_pair( textures[ i ], tex2D ) );
    }

    for( unsigned int i = 0; i < animations.size(); ++i, ++j )
    {
        auto tex2DSeq = static_cast< Texture2DSequenceImpl * >( shaderParams->GetTexture( j ) );
        m_animMappingVec.push_back( std::make_pair( animations[ i ], tex2DSeq ) );
    }
*/
}

// *****************************
//
void            NodeUpdater::RegisterShaderParams			( IValueSetConstPtr values, Shader * shader )
{
    if( values && shader )
    {
        auto shaderParams = shader->GetParameters();
        for( UInt32 i = 0; i < shaderParams->NumParameters(); ++i )
        {
            auto dest = shaderParams->AccessParam( i );
            auto source = values->GetValue( dest->Name() );
            if( source && source->GetType() == dest->Type() )
            {
                m_paramsMappingVec.push_back( Value2ShaderParam( source, dest ) );
            }
        }
    }
}

// *****************************
//
void	NodeUpdater::UpdateValue			( IValueConstPtr source, IValuePtr dest )
{
    assert( source && source->GetType() == dest->GetType() );

    switch( source->GetType() )
    {
        case ParamType::PT_INT:
            UpdateTypedValue< ValueIntPtr >( source, dest );
            break;
        case ParamType::PT_FLOAT1:
            UpdateTypedValue< ValueFloatPtr >( source, dest );
            break;
        case ParamType::PT_FLOAT2:
            UpdateTypedValue< ValueVec2Ptr >( source, dest );
            break;
        case ParamType::PT_FLOAT3:
            UpdateTypedValue< ValueVec3Ptr >( source, dest );
            break;
        case ParamType::PT_FLOAT4:
            UpdateTypedValue< ValueVec4Ptr >( source, dest );
            break;
        case ParamType::PT_MAT2:
            UpdateTypedValue< ValueMat2Ptr >( source, dest );
            break;
        case ParamType::PT_MAT3:
            UpdateTypedValue< ValueMat3Ptr >( source, dest );
            break;
        case ParamType::PT_MAT4:
            UpdateTypedValue< ValueMat4Ptr >( source, dest );
            break;
		case ParamType::PT_BOOL:
			UpdateTypedValue< ValueBoolPtr >( source, dest );
			break;
        default:
            assert( false );
    }
}

// ***********************
//
void NodeUpdater::UpdateVACPtr()
{
    auto finalizer = m_modelNode->GetFinalizePlugin();
    auto vertexAttributtesChannel = finalizer->GetVertexAttributesChannel();
    m_vertexAttributesChannel = vertexAttributtesChannel;

    assert( m_vertexAttributesChannel != nullptr );

    m_timeInvariantVertexData = m_vertexAttributesChannel->IsTimeInvariant();
}

// ***********************
//
void NodeUpdater::UpdateBoundingBox()
{
    auto node = Cast< const model::BasicNode * >( m_modelNode.get() );

    auto bv = node->GetBoundingVolume().get();
    assert( bv );

    m_sceneNode->SetBoundingBox( bv->GetBoundingBox() );
}

} //bv
