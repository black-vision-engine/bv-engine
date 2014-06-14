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


namespace bv 
{

// *****************************
//
NodeUpdater::NodeUpdater     ( RenderableEntity * renderable, SceneNode * sceneNode, const model::IModelNode * modelNode )
    : m_sceneNode( sceneNode )
    , m_modelNode( modelNode )
    , m_renderable( renderable )
{
    assert( sceneNode != nullptr );
    assert( modelNode != nullptr );
    assert( renderable != nullptr );
    assert( static_cast< void * >( renderable ) == static_cast< void * >( sceneNode->GetAnchor() ) ); //FIXME: why do we need more than one transformable per node and how to use it?

    m_timeInvariantVertexData = false;

    const model::IPlugin * finalizer = modelNode->GetFinalizePlugin();
    assert( finalizer );

    auto effect = renderable->GetRenderableEffect();

    m_transformChannel = finalizer->GetTransformChannel();
    assert( m_transformChannel != nullptr );

    if( effect )
    {
        m_vertexAttributesChannel = finalizer->GetVertexAttributesChannel();
        assert( m_vertexAttributesChannel != nullptr );

        m_timeInvariantVertexData = m_vertexAttributesChannel->IsTimeInvariant();

        m_rendererContext = finalizer->GetPixelShaderChannel()->GetRendererContext();
        assert( m_rendererContext );

        const bv::IShaderDataSource * psc = finalizer->GetPixelShaderChannel();
        const bv::IShaderDataSource * vsc = finalizer->GetVertexShaderChannel();
        const bv::IShaderDataSource * gsc = finalizer->GetGeometryShaderChannel();

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
void    NodeUpdater::DoUpdate        ()
{
    //FIXME: czy jesli node nie jest widoczne to trzeba w ogole updatowac stan - zakladam, ze nie, ale trzeba sie upewnic
    //FIXME: it is just a single bool to set, so no there is no fancy machinery for testing whehter any update is necessary 
    bool isVisible  = m_modelNode->IsVisible();
    bool isOverriden = m_modelNode->IsStateOverriden();

    m_sceneNode->SetVisible( isVisible );
    m_sceneNode->SetOverriden( isOverriden );
    m_rendererContext->SetStateChanged( m_modelNode->OverrideStateChanged() );

    //FIXME: Globalne ustawienia stanu obejmuja na teraz
    //1. Visibility
    //2. Overridden parameters
    //3. Overridden renderer state (render ctx)
    if( isVisible )
    {
        UpdateTransform();
        
        if( m_hasEffect )
        {
            if( !m_timeInvariantVertexData )
            {
                UpdateGeometry();
            }

            UpdateTexturesData();
            UpdateRendererState();

            if( isOverriden )
            {
                //FIXME: not the best way (state is set even if nothing changed)
                UpdateOverridenState();
            }
        }

        //if ( m_modelNode->OverridesParameters() )
        //{
        //    //Update overriden parameters (override values)
        //}

        //if( m_modelNode->OverridesRedererState() )
        //{
        //    //Override renderer state
        //}
    }
}

// *****************************
//
void    NodeUpdater::RegisterTexturesData   ( const IShaderDataSource * psTxData, const IShaderDataSource * vsTxData, const IShaderDataSource * gsTxData, RenderablePass * pass )
{
    RegisterTypedTexturesData( psTxData, pass->GetPixelShader() );
    RegisterTypedTexturesData( vsTxData, pass->GetVertexShader() );
    RegisterTypedTexturesData( gsTxData, pass->GetGeometryShader() );
}

// *****************************
//
bool   NodeUpdater::MustBeRegistered        ( const IShaderDataSource * shaderDataSrc, ShaderParameters * shaderParams )
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

    auto totalNumEntries = animations.size() + textures.size();

    assert( totalNumEntries == shaderParams->NumTextures() );

    return true;
}

// *****************************
//
void            NodeUpdater::RegisterTex2Params  ( const ITexturesData * texturesData, ShaderParameters * shaderParams )
{
    auto textures = texturesData->GetTextures();
    auto animations = texturesData->GetAnimations();

    if( textures.size() != 0 || animations.size() != 0 )
    {
        m_texDataMappingVec.push_back( std::make_pair( texturesData, shaderParams ) );
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
