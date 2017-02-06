#include "stdafx.h"

#include <cassert>

#include "OutputLogicDesc.h"


namespace bv { namespace nrl {

// **************************
//
unsigned int    OutputLogicDesc::GetNumDescriptors   () const
{
    return (unsigned int) m_outputDescriptors.size();
}

// **************************
//
OutputDesc &    OutputLogicDesc::AccessDesc          ( unsigned int i )
{
    assert( i < GetNumDescriptors() );
    
    return m_outputDescriptors[ i ];
}

// **************************
//
void            OutputLogicDesc::AppendDesc          ( const OutputDesc & desc )
{
    m_outputDescriptors.push_back( desc );
}

} //nrl
} //bv
