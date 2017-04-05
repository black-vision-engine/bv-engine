#include "stdafx.h"

#include "NSimpleFullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFulscreenEffectVisualComponent.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"


namespace bv { namespace nrl {

// **************************
//
NSimpleFullscreenEffectInstance::NSimpleFullscreenEffectInstance    ( NFullscreenEffectVisualComponent * visualComponent )
    : m_visualComponent( visualComponent )
{
}

// **************************
//
NSimpleFullscreenEffectInstance::~NSimpleFullscreenEffectInstance   ()
{
    delete m_visualComponent;
}

// **************************
//
void            NSimpleFullscreenEffectInstance::Render             ( NRenderContext * ctx )
{
    m_visualComponent->Render( renderer( ctx ) );
}

// **************************
//
IValuePtrVec    NSimpleFullscreenEffectInstance::GetIValues         () const
{
    return m_visualComponent->GetIValues();
}

// **************************
//
void    NSimpleFullscreenEffectInstance::SyncDynamicInput           ( const NRenderedData & dynamicInput )
{
    auto & renderTargets = dynamicInput.GetAllRenderTargets();

    m_visualComponent->SyncRenderTargets( renderTargets );
}

// **************************
//
void    NSimpleFullscreenEffectInstance::SyncState                  ( NFullscreenEffectComponentStatePtr state )
{
    { state; }
    // FIXME: implement
}

// **************************
//
void    NSimpleFullscreenEffectInstance::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_visualComponent->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv
