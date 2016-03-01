#include "stdafx.h"

#include "BlurPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

namespace bv {

// *********************************
//
BlurPreFullscreenEffectLogic::BlurPreFullscreenEffectLogic              ()
{
}

// *********************************
//
BlurPreFullscreenEffectLogic::~BlurPreFullscreenEffectLogic             ()
{
}

// *********************************
//
void                        BlurPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    { outputs; }
    { node; }
    //auto blurSizeVal = node->GetNodeEffect()->GetValue( "blurSize" );
    //auto normalizeVal = node->GetNodeEffect()->GetValue( "normalize" );
    //auto blurKernelTypeVal = node->GetNodeEffect()->GetValue( "blurKernelType" );

//    auto blurKernelTypeValue = QueryTypedValue< ValueIntPtr >( blurKernelTypeVal )->GetValue();
    //auto blurSizeValue = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();
 //   auto normalizeFlagValue = QueryTypedValue< ValueIntPtr >( normalizeVal )->GetValue() > 0 ? true : false;

//    auto renderer       = ctx->GetRenderer();
    
    auto mainRT = disableBoundRT( ctx );
    auto outRT  = outputs[ 0 ];

    enable( ctx, outRT );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    disableBoundRT( ctx );

    enable( ctx, mainRT );

//    if( blurSizeValue < 1.f )
//    {
//        logic->DrawNode( node, ctx );
//    }
//    else
//    {
//        auto rtAllocator    = ctx->GetRenderTargetAllocator();
//
//        auto mainTarget = disableBoundRT( ctx );
//
////        auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
//        auto hBluredRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
//
//        RenderToRenderTarget( ctx, outputs[ 0 ], node );
//
//        enable( ctx, hBluredRenderTarget );
//        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//
//        //ApplyBlurEffect( renderer, foregroundRt, blurSizeValue, false, normalizeFlagValue, blurKernelTypeValue );
//
//        //rtAllocator->Free();
//        //rtAllocator->Free();
//
//        //disableBoundRT( ctx );
//
//        enable( ctx, mainTarget );
//
//        //ApplyBlurEffect( renderer, hBluredRenderTarget, blurSizeValue, true, normalizeFlagValue, blurKernelTypeValue );
//    }
}

// *********************************
//
std::vector< IValuePtr >    BlurPreFullscreenEffectLogic::GetValues     () const
{
    return std::vector< IValuePtr >();
    //std::vector< IValuePtr > res( 4 );
    //res[ 0 ] = m_blurSize;
    //res[ 1 ] = m_isVertical;
    //res[ 2 ] = m_normalize;
    //res[ 3 ] = m_blurKernelTypeVal;

    //return res;
}

// *********************************
//
unsigned int                BlurPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
bool                        BlurPreFullscreenEffectLogic::IsFSERequired () const
{
    //auto bs = m_blurSize->GetValue();

    return true;
}

} // bv