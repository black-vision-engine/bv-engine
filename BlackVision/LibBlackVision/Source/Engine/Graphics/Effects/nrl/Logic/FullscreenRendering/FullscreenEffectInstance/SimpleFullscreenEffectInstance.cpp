#include "stdafx.h"

#include "SimpleFullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/FulscreenEffectVisualComponent.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/RenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"


namespace bv { namespace nrl {

// **************************
//
SimpleFullscreenEffectInstance::SimpleFullscreenEffectInstance    ( FullscreenEffectVisualComponent * visualComponent )
    : m_visualComponent( visualComponent )
{
}

// **************************
//
SimpleFullscreenEffectInstance::~SimpleFullscreenEffectInstance   ()
{
    delete m_visualComponent;
}

// **************************
//
void            SimpleFullscreenEffectInstance::Render             ( RenderContext * ctx )
{
    m_visualComponent->Render( renderer( ctx ) );
}

// **************************
//
IValuePtrVec    SimpleFullscreenEffectInstance::GetIValues         () const
{
    return m_visualComponent->GetIValues();
}

// **************************
//
void    SimpleFullscreenEffectInstance::SyncDynamicInput           ( const RenderedData & dynamicInput )
{
    auto & renderTargets = dynamicInput.GetAllRenderTargets();

    m_visualComponent->SyncRenderTargets( renderTargets );
}

// **************************
//
void    SimpleFullscreenEffectInstance::SyncState                  ( FullscreenEffectComponentStatePtr state )
{
    { state; }
    // FIXME: implement
}

// **************************
//
void    SimpleFullscreenEffectInstance::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_visualComponent->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv
