#include "stdafx.h"

#include "OutputDesc.h"


namespace bv { 

// **************************
//
OutputDesc::OutputDesc                                                      ()
    : m_staticData( 1920, 1080, RenderChannelType::RCT_OUTPUT_1 )
    , m_outputType( CustomOutputType::COT_PREVIEW )
    , m_outputChannelMapping( OutputChannelMapping::OCM_RGBA )
{
}

// **************************
//
OutputDesc::TOutputPropertiesVector &   OutputDesc::AccessOutputProperties  ()
{
    return m_outputProperties;
}

// **************************
//
const OutputDesc::TOutputPropertiesVector & OutputDesc::GetOutputProperties () const
{
    return m_outputProperties;
}

// **************************
//
unsigned int                OutputDesc::GetWidth                            () const
{
    return m_staticData.width;
}

// **************************
//
unsigned int                OutputDesc::GetHeight                           () const
{
    return m_staticData.height;
}

// **************************
//
RenderChannelType           OutputDesc::GetSelectedRenderedChannel          () const
{
    return m_staticData.selectedRenderedChannel;
}
    
// **************************
//
OutputChannelMapping        OutputDesc::GetOutputChannelMapping             () const
{
    return m_outputChannelMapping;
}

// **************************
//
CustomOutputType            OutputDesc::GetRepresentedOutputType            () const
{
    return m_outputType;
}

// **************************
//
bool                        OutputDesc::IsEnabled                           () const
{
    return m_isEnabled;
}

// **************************
//
void                        OutputDesc::SetWidth                            ( unsigned int width )
{
    m_staticData.width = width;
}

// **************************
//
void                        OutputDesc::SetHeight                           ( unsigned int height )
{
    m_staticData.height = height;
}

// **************************
//
void                        OutputDesc::SetSelectedRenderedChannel          ( RenderChannelType rct )
{
    m_staticData.selectedRenderedChannel = rct;
}
    
// **************************
//
void                        OutputDesc::SetOutputChannelMapping             ( OutputChannelMapping ocm )
{
    m_outputChannelMapping = ocm;
}

// **************************
//
void                        OutputDesc::SetRepresentedOutputType            ( CustomOutputType cot )
{
    m_outputType = cot;
}

// **************************
//
void                        OutputDesc::SetEnabled                          ( bool isEnabled )
{
    m_isEnabled = isEnabled;
}


} //bv
