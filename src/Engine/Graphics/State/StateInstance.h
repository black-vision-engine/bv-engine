#pragma once

#include "Engine/Graphics/State/AlphaState.h"
#include "Engine/Graphics/State/CullState.h"
#include "Engine/Graphics/State/DepthState.h"
#include "Engine/Graphics/State/FillState.h"
#include "Engine/Graphics/State/OffsetState.h"
#include "Engine/Graphics/State/StencilState.h"


namespace bv {

class StateInstance
{
private:

    // Global state.
    AlphaState *      m_alphaState;
    CullState *       m_cullState;
    DepthState *      m_depthState;
    FillState *       m_fillState;
    OffsetState *     m_offsetState;
    StencilState *    m_stencilState;

public:

            StateInstance   ();
            StateInstance   ( AlphaState * as, CullState * cs, DepthState * ds, FillState * fs, OffsetState * os, StencilState * ss );

            ~StateInstance  ();

    const AlphaState *      GetAlphaState   () const;
    const CullState *       GetCullState    () const;
    const DepthState *      GetDepthState   () const;
    const FillState *       GetFillState    () const;
    const OffsetState *     GetOffsetState  () const;
    const StencilState *    GetStencilState () const;

    void                    SetState( AlphaState * as );
    void                    SetState( CullState * cs );
    void                    SetState( DepthState * ds );
    void                    SetState( FillState * fs );
    void                    SetState( OffsetState * os );
    void                    SetState( StencilState * ss );

    void                    SetStateIgnoreEmptyEntries( const StateInstance & inst );

    void                    DeleteStates    ();

    friend class RenderStateAccessor;
};

class RenderStateAccessor
{
public:

    static AlphaState *     AccessAlphaState( StateInstance * inst );
    static CullState *      AccessCullState( StateInstance * inst );
    static DepthState *     AccessDepthState( StateInstance * inst );
    static FillState *      AccessFillState( StateInstance * inst );
    static OffsetState *    AccessOffsetState( StateInstance * inst );
    static StencilState *   AccessStencilState( StateInstance * inst );
};

}

