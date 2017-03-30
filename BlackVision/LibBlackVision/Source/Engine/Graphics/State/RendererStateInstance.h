#pragma once

#include "Engine/Graphics/State/AlphaState.h"
#include "Engine/Graphics/State/CullState.h"
#include "Engine/Graphics/State/DepthState.h"
#include "Engine/Graphics/State/FillState.h"
#include "Engine/Graphics/State/OffsetState.h"
#include "Engine/Graphics/State/StencilState.h"


namespace bv {

class RendererStateInstance
{
private:

    // Global state.
    AlphaStatePtr      m_alphaState;
    CullStatePtr       m_cullState;
    DepthStatePtr      m_depthState;
    FillStatePtr       m_fillState;
    OffsetStatePtr     m_offsetState;
    StencilStatePtr    m_stencilState;

public:

            RendererStateInstance   ();
            RendererStateInstance   ( AlphaStatePtr as, CullStatePtr cs, DepthStatePtr ds, FillStatePtr fs, OffsetStatePtr os, StencilStatePtr ss );

            ~RendererStateInstance  ();

    AlphaStateConstPtr      GetAlphaState   () const;
    CullStateConstPtr       GetCullState    () const;
    DepthStateConstPtr      GetDepthState   () const;
    FillStateConstPtr       GetFillState    () const;
    OffsetStateConstPtr     GetOffsetState  () const;
    StencilStateConstPtr    GetStencilState () const;

    void                    SetState( AlphaStatePtr as );
    void                    SetState( CullStatePtr cs );
    void                    SetState( DepthStatePtr ds );
    void                    SetState( FillStatePtr fs );
    void                    SetState( OffsetStatePtr os );
    void                    SetState( StencilStatePtr ss );

    void                    AssignState( AlphaStateConstPtr as );
    void                    AssignState( CullStateConstPtr cs );
    void                    AssignState( DepthStateConstPtr ds );
    void                    AssignState( FillStateConstPtr fs );
    void                    AssignState( OffsetStateConstPtr os );
    void                    AssignState( StencilStateConstPtr ss );

    void                    SetStateIgnoreEmptyEntries( const RendererStateInstance & inst );

    friend class RenderStateAccessor;
};

class RenderStateAccessor
{
public:

    static AlphaStatePtr &      AccessAlphaState    ( RendererStateInstance * inst );
    static CullStatePtr &       AccessCullState     ( RendererStateInstance * inst );
    static DepthStatePtr &      AccessDepthState    ( RendererStateInstance * inst );
    static FillStatePtr &       AccessFillState     ( RendererStateInstance * inst );
    static OffsetStatePtr &     AccessOffsetState   ( RendererStateInstance * inst );
    static StencilStatePtr &    AccessStencilState  ( RendererStateInstance * inst );
};

}
 // bv