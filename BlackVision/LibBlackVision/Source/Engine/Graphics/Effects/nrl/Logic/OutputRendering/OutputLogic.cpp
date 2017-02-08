#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic                                ()
{
}

// *********************************
//
OutputLogic::~OutputLogic                               ()
{
    for( auto output : m_outputs )
    {
        delete output;
    }
}

// *********************************
//
void                OutputLogic::ProcessFrameData       ( NRenderContext * ctx, RenderedChannelsData * rcd )
{
    for( auto output : m_outputs )
    {
        output->ProcessFrameData( ctx, rcd );
    }
}

// *********************************
//
Output *            OutputLogic::GetOutput              ( CustomOutputType outputType )
{
    return m_outputs[ (unsigned int) outputType ];
}

// *********************************
//
void                OutputLogic::AppendOutput           ( Output * output )
{
    m_outputs.push_back( output );
}

// *********************************
//
OutputLogic *       OutputLogic::Create                 ( OutputLogicDesc & desc )
{
    OutputLogic * res = new OutputLogic();

    for( unsigned int i = 0; i < desc.GetNumDescriptors(); ++i )
    {
        auto output = Output::Create( desc.AccessDesc( i ) );
        
        // FIXME: nrl - thikn of a common approach to handling nullptr outputs (or do not allow them at all)
        if ( output )
        {
            res->AppendOutput( output );
        }
    }

    return res;
}

} //nrl
} //bv
