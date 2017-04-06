#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/RenderedData.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/FullscreenEffectComponentState.h"


namespace bv { namespace nrl {

class RenderContext;

class FullscreenEffectInstance
{
public:

                            ~FullscreenEffectInstance  ();

    virtual void            Render                      ( RenderContext * ctx ) = 0;
            void            Render                      ( RenderContext * ctx, const RenderedData & dynamicInput );

    virtual IValuePtrVec    GetIValues                  () const = 0;

protected:

    virtual void            SyncDynamicInput            ( const RenderedData & dynamicInput ) = 0;

public:

    virtual void            SyncState                   ( FullscreenEffectComponentStatePtr state ) = 0;
    virtual void            AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const = 0;

};

} // nrl
} // bv
