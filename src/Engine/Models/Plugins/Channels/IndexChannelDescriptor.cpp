#include "IndexChannelDescriptor.h"

namespace bv { namespace model
{

const int IndexChannelDescriptor::m_sIdxSizes[ (int) IndexType::IT_TOTAL ] = { sizeof( unsigned char ),  // IT_UBYTE,
                                                                               sizeof( unsigned short ), // IT_USHORT,
                                                                               sizeof( unsigned int )    // IT_UINT    
                                                                             };

// ************************************
//
IndexChannelDescriptor::IndexChannelDescriptor  ( IndexType type )
    : m_indexType( type )
{
}

// ************************************
//
IndexChannelDescriptor::~IndexChannelDescriptor ()
{
}

// ************************************
//
IndexType       IndexChannelDescriptor::GetType                 ()  const
{
    return m_indexType;
}

// ************************************
//
int             IndexChannelDescriptor::GetIndexSize            ()  const
{
    return IndexSize( m_indexType );
}

// ************************************
//
int             IndexChannelDescriptor::IndexSize               ( IndexType type )
{
    return m_sIdxSizes[ (int) type ];
}

} //model
}//bv
