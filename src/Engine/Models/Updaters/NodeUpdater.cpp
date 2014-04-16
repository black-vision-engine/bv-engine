#include "NodeUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"


namespace bv 
{

// *****************************
//
NodeUpdater::NodeUpdater     ( RenderableEntity * renderable, SceneNode * sceneNode, const model::IModelNode * modelNode )
    : m_sceneNode( sceneNode )
    , m_modelNode( modelNode )
    , m_renderable( renderable )
    , m_psTexturesData( nullptr )
    , m_vsTexturesData( nullptr )
    , m_gsTexturesData( nullptr )
    , m_psShaderParameters( nullptr )
    , m_vsShaderParameters( nullptr )
    , m_gsShaderParameters( nullptr )
{
    assert( sceneNode != nullptr );
    assert( modelNode != nullptr );
    assert( renderable != nullptr );
    assert( static_cast< void * >( renderable ) == static_cast< void * >( sceneNode->GetAnchor() ) ); //FIXME: why do we need more than one transformable per node and how to use it?

    m_timeInvariantVertexData = false;

    const model::IPlugin * finalizer = modelNode->GetPluginList()->GetFinalizePlugin();
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

        auto psc = finalizer->GetPixelShaderChannel();
        auto vsc = finalizer->GetVertexShaderChannel();
        auto gsc = finalizer->GetGeometryShaderChannel();

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

            RegisterTexturesData( &psc, pass->GetPixelShader()->GetParameters() );
            RegisterTexturesData( &vsc, pass->GetVertexShader()->GetParameters() );
            RegisterTexturesData( &vsc, pass->GetGeometryShader()->GetParameters() );
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
    bool isVisible = m_modelNode->IsVisible();

    m_sceneNode->SetVisible( isVisible );

    if( isVisible )
    {
        UpdateTransform();
        
        if( m_hasEffect )
        {
            if( !m_timeInvariantVertexData )
            {
                UpdateGeometry();
            }

            UpdateRendererState();
        }
    }
}

// *****************************
//
void    NodeUpdater::RegisterTexturesData( const IShaderDataSource *& texturesData, ShaderParameters *& shaderParameters )
{
    if( texturesData )
    {
        
    }
}

} //bv
