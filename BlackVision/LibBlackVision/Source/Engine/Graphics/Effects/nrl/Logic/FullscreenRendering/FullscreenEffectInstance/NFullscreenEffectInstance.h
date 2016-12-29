#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"


namespace bv { namespace nrl {

class NRenderContext;

class NFullscreenEffectInstance
{
public:

                            ~NFullscreenEffectInstance  ();

    virtual void            Render                      ( NRenderContext * ctx ) = 0;
            void            Render                      ( NRenderContext * ctx, const NRenderedData & dynamicInput );

    virtual IValuePtrVec    GetIValues                  () const = 0;

protected:

    virtual void            SyncDynamicInput            ( const NRenderedData & dynamicInput ) = 0;

public:

    virtual void            SyncState                   ( NFullscreenEffectComponentStatePtr state ) = 0;

};

} // nrl
} // bv
