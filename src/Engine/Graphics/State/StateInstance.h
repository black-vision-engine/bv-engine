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
    const AlphaState *      m_alphaState;
    const CullState *       m_cullState;
    const DepthState *      m_depthState;
    const FillState *       m_fillState;
    const OffsetState *     m_offsetState;
    const StencilState *    m_stencilState;

public:

            StateInstance   ();
            StateInstance   ( const AlphaState * as, const CullState * cs, const DepthState * ds, const FillState * fs, const OffsetState * os, const StencilState * ss );

            ~StateInstance  ();

    const AlphaState *      GetAlphaState   () const;
    const CullState *       GetCullState    () const;
    const DepthState *      GetDepthState   () const;
    const FillState *       GetFillState    () const;
    const OffsetState *     GetOffsetState  () const;
    const StencilState *    GetStencilState () const;

    void                    SetState( const AlphaState * as );
    void                    SetState( const CullState * cs );
    void                    SetState( const DepthState * ds );
    void                    SetState( const FillState * fs );
    void                    SetState( const OffsetState * os );
    void                    SetState( const StencilState * ss );

    void                    DeleteStates    ();

};

}
