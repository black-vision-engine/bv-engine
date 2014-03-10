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

        for( unsigned int i = 0; i < effect->NumPasses(); ++i )
        {
            auto inst = effect->GetPass( i )->GetStateInstance();

            assert( !inst->GetAlphaState() );
            assert( !inst->GetCullState() );
            assert( !inst->GetDepthState() );
            assert( !inst->GetFillState() );
            assert( !inst->GetOffsetState() );
            assert( !inst->GetStencilState() );

            RendererStatesBuilder::Create( inst, m_rendererContext );

            m_redererStateInstanceVec.push_back( inst );
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

} //bv
