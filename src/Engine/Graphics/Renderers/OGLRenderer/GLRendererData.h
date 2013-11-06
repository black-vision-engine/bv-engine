#pragma once

#include "Engine/Graphics/State/StateInstance.h"

namespace bv
{
    

//FIXME: tutaj mozna trzymac sobie stan dla wszystkich mozliwych teksture unitow, zeby ewentualnie nie trzeba bylo tego stany co tekstra przepinac - ale to taka mala optymalizacja
//FIXME: Buffer usage dla texture unitu to jest DYNAMIC_DRAW, to samo trzeba robic dla dynamicznych geometrii, ktore co klatka wymagaja wrzucenia calego kanalu atrybutow !!!!
class RendererData
{
public:

    // Maintain current render states to avoid redundant state changes.
    class RenderState
    {
    public:

        StateInstance   m_curState;

    public:

                RenderState				();
        void    Initialize         ( const StateInstance & inst );

    private:

        void    InitializeAlphaState    ( const AlphaState * as );
        void    InitializeCullState     ( const CullState * cs );
        void    InitializeDepthState    ( const DepthState * ds );
        void    InitializeFillState     ( const FillState * fs );
        void    InitializeOffsetState   ( const OffsetState * os );
        void    InitializeStencilState  ( const StencilState * ss );

        //// AlphaState
        //bool mAlphaBlendEnabled;
        //GLenum mAlphaSrcBlend;
        //GLenum mAlphaDstBlend;
        //bool mAlphaCompareEnabled;
        //GLenum mCompareFunction;
        //float mAlphaReference;
        //Float4 mBlendColor;

        //// CullState
        //bool mCullEnabled;
        //bool mCCWOrder;

        //// DepthState
        //bool mDepthEnabled;
        //bool mDepthWriteEnabled;
        //GLenum mDepthCompareFunction;

        //// OffsetState
        //bool mFillEnabled;
        //bool mLineEnabled;
        //bool mPointEnabled;
        //float mOffsetScale;
        //float mOffsetBias;

        //// StencilState
        //bool mStencilEnabled;
        //GLenum mStencilCompareFunction;
        //GLuint mStencilReference;
        //GLuint mStencilMask;
        //GLuint mStencilWriteMask;
        //GLenum mStencilOnFail;
        //GLenum mStencilOnZFail;
        //GLenum mStencilOnZPass;

        // WireState
        bool mWireEnabled;
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

}
