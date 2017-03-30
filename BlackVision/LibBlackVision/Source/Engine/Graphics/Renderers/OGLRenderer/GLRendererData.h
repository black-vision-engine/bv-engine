#pragma once

#include "Engine/Graphics/State/RendererStateInstance.h"

namespace bv
{
    

//FIXME: tutaj mozna trzymac sobie stan dla wszystkich mozliwych teksture unitow, zeby ewentualnie nie trzeba bylo tego stany co tekstra przepinac - ale to taka mala optymalizacja
//FIXME: Buffer usage dla texture unitu to jest DYNAMIC_DRAW, to samo trzeba robic dla dynamicznych geometrii, ktore co klatka wymagaja wrzucenia calego kanalu atrybutow !!!!
class RendererData
{
public:

    virtual ~RendererData() {}

    // Maintain current render states to avoid redundant state changes.
    class RenderState
    {
    public:

        RendererStateInstance   m_curState;

    public:

                RenderState				();
                ~RenderState            ();

        void    Initialize              ( const RendererStateInstance & inst );

    private:

        void    InitializeAlphaState    ( AlphaStateConstPtr as );
        void    InitializeCullState     ( CullStateConstPtr cs );
        void    InitializeDepthState    ( DepthStateConstPtr ds );
        void    InitializeFillState     ( FillStateConstPtr fs );
        void    InitializeOffsetState   ( OffsetStateConstPtr os );
        void    InitializeStencilState  ( StencilStateConstPtr ss );

    public:

        AlphaStateConstPtr      CurAlphaState   () const;
        CullStateConstPtr       CurCullState    () const;
        DepthStateConstPtr      CurDepthState   () const;
        FillStateConstPtr       CurFillState    () const;
        OffsetStateConstPtr     CurOffsetState  () const;
        StencilStateConstPtr    CurStencilState () const;
    };

    RenderState m_CurrentRS;

    // Maintain current sampler states to avoid redundant state changes.
    enum
    {
        MAX_NUM_VSAMPLERS = 4,  // VSModel 3 has 4, VSModel 2 has 0.
        MAX_NUM_PSAMPLERS = 16  // PSModel 2 and PSModel 3 have 16.
    };

    class SamplerState
    {
    public:

        SamplerState ();

        //// Get the state of the currently enabled texture.  This state appears
        //// to be associated with the OpenGL texture object.  How does this
        //// relate to the sampler state?  In my opinion, OpenGL needs to have
        //// the sampler state separate from the texture object state.
        //void GetCurrent (GLenum target);

        //float mAnisotropy;
        //float mLodBias;
        //GLint mMagFilter;
        //GLint mMinFilter;
        //Float4 mBorderColor;
        //GLint mWrap[3];
    };

    SamplerState mCurrentSS[ MAX_NUM_PSAMPLERS ];

    // Capabilities (queried at run time).
    //int mMaxVShaderImages;
    //int mMaxPShaderImages;
    //int mMaxCombinedImages;
};

} //bv
