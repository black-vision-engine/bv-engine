#include "NodeUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Interfaces/IShaderDataSource.h"
#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectDefault.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectWireframe.h"

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffects/NodeMaskNodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffects/AlphaMaskNodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffects/WireframeNodeEffect.h"


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
        }
    }

    m_hasEffect = effect != nullptr;
}

// *****************************
//
NodeUpdater::~NodeUpdater    ()
{
}

// *****************************
//
void    NodeUpdater::DoUpdate               ()
{
    //FIXME: czy jesli node nie jest widoczne to trzeba w ogole updatowac stan - zakladam, ze nie, ale trzeba sie upewnic
    //FIXME: it is just a single bool to set, so no there is no fancy machinery for testing whehter any update is necessary 
    if( m_modelNode->IsVisible() )
    {
        m_sceneNode->SetVisible( true );

        // Add, when all mechanisms are implemented
        UpdateNodeEffect();
        UpdateTransform();

        if( m_hasEffect )
        {
            if( !m_timeInvariantVertexData )
            {
                UpdateGeometry();
            }

            UpdateTexturesData();
            UpdateRendererState();
        }
    }
    else
    {
        m_sceneNode->SetVisible( false );
    }
}

// *****************************
// FIXME: change effects if required or assert that they cannot be changed in runtime
void    NodeUpdater::UpdateNodeEffect       ()
{
    auto name = m_modelNode->GetName();
    auto nodeEffect = m_modelNode->GetNodeEffect();

    if( nodeEffect )
    {
        switch( nodeEffect->GetType() )
        {
            case NodeEffectType::NET_DEFAULT:
            {
                auto defaultEffect = std::static_pointer_cast< model::ModelNodeEffectDefault >( nodeEffect );

                auto sceneNodeEffect = m_sceneNode->GetNodeEffect();

                if ( !sceneNodeEffect || sceneNodeEffect->GetType() != NodeEffect::Type::T_DEFAULT )
                {
                    sceneNodeEffect = std::make_shared< NodeEffect >( NodeEffect::Type::T_DEFAULT );
                    m_sceneNode->SetNodeEffect( sceneNodeEffect );
                }
                break;
            }
            case NodeEffectType::NET_ALPHA_MASK:
            {
                auto alphaMaskEffect = std::static_pointer_cast< model::ModelNodeEffectAlphaMask >( nodeEffect );
                auto paramAlpha = alphaMaskEffect->GetParamAlpha();

                auto sceneNodeEffect = m_sceneNode->GetNodeEffect();

                if ( !sceneNodeEffect || sceneNodeEffect->GetType() != NodeEffect::Type::T_ALPHA_MASK )
                {
                    sceneNodeEffect = std::make_shared< AlphaMaskNodeEffect >();
                    m_sceneNode->SetNodeEffect( sceneNodeEffect );
                }

                auto alphaVal = std::static_pointer_cast< ValueFloat >( sceneNodeEffect->GetValue( paramAlpha->GetName() ) );

                if ( alphaVal != nullptr )
                {
                    alphaVal->SetValue( alphaMaskEffect->GetAlpha() );
                }

                break;
            }
            case NodeEffectType::NET_NODE_MASK:
            {
                auto nodeMaskEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( nodeEffect );

                auto paramBgIdx = nodeMaskEffect->GetParamBgIdx();
                auto paramFgIdx = nodeMaskEffect->GetParamFgIdx();
                auto paramAlpha = nodeMaskEffect->GetParamAlpha();

                auto sceneNodeEffect = m_sceneNode->GetNodeEffect();

                if ( !sceneNodeEffect || sceneNodeEffect->GetType() != NodeEffect::Type::T_NODE_MASK )
                {
                    sceneNodeEffect = std::make_shared< NodeMaskNodeEffect >();
                    m_sceneNode->SetNodeEffect( sceneNodeEffect );
                }

                auto bgIdxVal = std::static_pointer_cast< ValueInt >( sceneNodeEffect->GetValue( paramBgIdx->GetName() ) );
                auto fgIdxVal = std::static_pointer_cast< ValueInt >( sceneNodeEffect->GetValue( paramFgIdx->GetName() ) );
                auto alphaVal = std::static_pointer_cast< ValueFloat >( sceneNodeEffect->GetValue( paramAlpha->GetName() ) );

                if ( bgIdxVal != nullptr && fgIdxVal != nullptr && alphaVal != nullptr )
                {
                    bgIdxVal->SetValue( nodeMaskEffect->GetBackgroundChildIdx() );
                    fgIdxVal->SetValue( nodeMaskEffect->GetForegroundChildIdx() );
                    alphaVal->SetValue( nodeMaskEffect->GetAlpha() );
                }

                break;
            }
            case NodeEffectType::NET_WIREFRAME:
            {
                auto nodeMaskEffect = std::static_pointer_cast< model::ModelNodeEffectNodeMask >( nodeEffect );
                auto sceneNodeEffect = m_sceneNode->GetNodeEffect();

                if ( !sceneNodeEffect || sceneNodeEffect->GetType() != NodeEffect::Type::T_WIREFRAME )
                {
                    sceneNodeEffect = std::make_shared< WireframeNodeEffect >();
                    m_sceneNode->SetNodeEffect( sceneNodeEffect );
                }
                break;
            }
            default:
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
		m_texDataUpdateID.push_back( 0 );
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

} //bv
