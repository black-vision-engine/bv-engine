#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoTypes.h"

#include "Engine/Graphics/Effects/Logic/State/OutputState.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Output.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoOutputsHandler.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoOutputsPreprocessor.h"


namespace bv { 

class FrameDataHandler;

class OutputCompositeVideo : public Output
{
    FRIEND_TEST_ACCESSOR( OutputCompositeVideo )
private:

    unsigned int                m_mainWidth;
    unsigned int                m_mainHeight;

    VideoOutputsPreprocessor    m_outputsPreprocessor;
    VideoOutputsHandler         m_outputsHandler;
    
public:

                                    OutputCompositeVideo    ( unsigned int width, unsigned int height );
                                    ~OutputCompositeVideo   ();

    virtual void                    ProcessFrameData        ( RenderContext * ctx, RenderedChannelsData * input ) override;

public:

    static  OutputCompositeVideo * Create                   ( unsigned int width, unsigned int height, OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );

};


} //bv
