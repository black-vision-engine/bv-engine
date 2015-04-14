#pragma once

#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

#include "Engine/Graphics/State/AlphaState.h"
#include "Engine/Graphics/State/CullState.h"
#include "Engine/Graphics/State/DepthState.h"
#include "Engine/Graphics/State/FillState.h"
#include "Engine/Graphics/State/OffsetState.h"
#include "Engine/Graphics/State/StencilState.h"

namespace bv {

namespace model {
    
    class AlphaContext;
    class CullContext;
    class DepthContext;
    class FillContext;

    class RendererContext;

} //model

class RendererStateInstance;

class RendererStatesBuilder
{
private:

    static AlphaStatePtr    CreateStateFromCtx( const model::AlphaContext * ac );
    static CullStatePtr     CreateStateFromCtx( const model::CullContext * cc );
    static DepthStatePtr    CreateStateFromCtx( const model::DepthContext * dc );
    static FillStatePtr     CreateStateFromCtx( const model::FillContext * fc );

    static AlphaStatePtr    CreateDefaultAlphaState  ();
    static CullStatePtr     CreateDefaultCullState   ();
    static DepthStatePtr    CreateDefaultDepthState  ();
    static FillStatePtr     CreateDefaultFillState   ();

    //FIXME: offset state and stencil state are skipped for now

public:


    static void Create  ( RendererStateInstance * inst, model::RendererContextConstPtr ctx ); 
    static void Create  ( RendererStateInstance * inst );

    static void Assign  ( AlphaStatePtr as, const model::AlphaContext * ac );
    static void Assign  ( CullStatePtr cs, const model::CullContext * cc );
    static void Assign  ( DepthStatePtr ds, const model::DepthContext * dc );
    static void Assign  ( FillStatePtr fs, const model::FillContext * fc );

    //FIXME: offset state and stencil state are skipped for now
};

} //bv
