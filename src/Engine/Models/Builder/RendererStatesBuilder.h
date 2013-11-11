#pragma once


namespace bv {

namespace model {
    
    class AlphaContext;
    class CullContext;
    class DepthContext;
    class FillContext;

    class RendererContext;

} //model

class StateInstance;

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

    //FIXME: offset state and stencil state are skipped for now

public:

    static void Create  ( StateInstance * inst, const model::RendererContext * ctx ); 

    static void Assign  ( AlphaState * as, const model::AlphaContext * ac );
    static void Assign  ( CullState * cs, const model::CullContext * cc );
    static void Assign  ( DepthState * ds, const model::DepthContext * dc );
    static void Assign  ( FillState * fs, const model::FillContext * fc );

    //FIXME: offset state and stencil state are skipped for now
};

} //bv
