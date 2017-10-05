#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/OutputCompositeVideo.h"

#include "Services/BVServiceProvider.h"


namespace bv
{

// ***********************
//
class TEST_ACCESSOR( OutputCompositeVideo )
{
public:

    static VideoOutputsPreprocessor &      GetVideoOutputPreprocessor  ( OutputCompositeVideo * output )      { return output->m_outputsPreprocessor; }

};

// ***********************
//
class TEST_ACCESSOR( VideoOutputsPreprocessor )
{
public:

    static VideoInputChannelsData &         GetInputChannels            ( VideoOutputsPreprocessor & outPreproc ) { return outPreproc.m_inputChannels; }
    static void                             Initialize                  ( VideoOutputsPreprocessor & outPreproc, RenderContext * ctx, RenderedChannelsData * input );
};

// ***********************
//
class TEST_ACCESSOR( BVServiceProvider )
{
public:
    static void         RegisterVideoCardManager        ( videocards::VideoCardManager * manager )  { BVServiceProvider::GetInstance().RegisterVideoCardManager( manager ); }
};

// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
void        TEST_ACCESSOR( VideoOutputsPreprocessor )::Initialize       ( VideoOutputsPreprocessor & outPreproc, RenderContext * ctx, RenderedChannelsData * input )
{
    outPreproc.PostInitialize( ctx, input );
}


}	// bv

