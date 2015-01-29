#pragma once

#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

namespace bv {

namespace model {
    
    class AlphaContext;
    class CullContext;
    class DepthContext;
    class FillContext;

    class RendererContext;

} //model

class RendererStateInstance;

class AlphaState;
class CullState;
class DepthState;
class FillState;
class OffsetState;
class StencilState;

class RendererStatesBuilder
{
private:

    static AlphaState *    CreateStateFromCtx( const model::AlphaContext * ac );
    static CullState *     CreateStateFromCtx( const model::CullContext * cc );
    static DepthState *    CreateStateFromCtx( const model::DepthContext * dc );
    static FillState *     CreateStateFromCtx( const model::FillContext * fc );

    static AlphaState *    CreateDefaultAlphaState  ();
    static CullState *     CreateDefaultCullState   ();
    static DepthState *    CreateDefaultDepthState  ();
    static FillState *     CreateDefaultFillState   ();

    //FIXME: offset state and stencil state are skipped for now

public:


    static void Create  ( RendererStateInstance * inst, model::RendererContextConstPtr ctx ); 
    static void Create  ( RendererStateInstance * inst );

    static void Assign  ( AlphaState * as, const model::AlphaContext * ac );
    static void Assign  ( CullState * cs, const model::CullContext * cc );
    static void Assign  ( DepthState * ds, const model::DepthContext * dc );
    static void Assign  ( FillState * fs, const model::FillContext * fc );

    //FIXME: offset state and stencil state are skipped for now
};

} //bv