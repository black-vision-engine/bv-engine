#include "stdafx.h"

#include "VertexDescriptor.h"

#include <cassert>
#include <cstdarg>

#include "Engine/Types/Enums.h"


namespace bv {

//FIXME: move closer to the definition
unsigned int VertexDescriptor::m_sComponentSize[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    sizeof( float ),  // AT_FLOAT1
    sizeof( float ),  // AT_FLOAT2
    sizeof( float ),  // AT_FLOAT3
    sizeof( float ),  // AT_FLOAT4
};

unsigned int VertexDescriptor::m_sNumComponents[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    1,  // AT_FLOAT1
    2,  // AT_FLOAT2
    3,  // AT_FLOAT3
    4,  // AT_FLOAT4
};

unsigned int VertexDescriptor::m_sTypeSize[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    1 * sizeof( float ),  // AT_FLOAT1
    2 * sizeof( float ),  // AT_FLOAT2
    3 * sizeof( float ),  // AT_FLOAT3
    4 * sizeof( float ),  // AT_FLOAT4
};


// *********************************
//
VertexDescriptor::VertexDescriptor ( unsigned int numAttrs )
    : m_numAttrs( numAttrs )
    , m_stride(0)
{
    assert( numAttrs > 0 );

    for( unsigned int i = 0; i < numAttrs; ++i )
    {
        m_entries.push_back( Entry() );
    }
}

// *********************************
//
VertexDescriptor::~VertexDescriptor ()
{
}

// *********************************
//    VertexDescriptor * vd = VertexDescriptor::Create(2,
//        0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION,
//        1, AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD );
VertexDescriptor * VertexDescriptor::Create ( unsigned int numAttrs, ... )
{
    VertexDescriptor * vd = new VertexDescriptor( numAttrs );

    va_list args;
    va_start( args, numAttrs );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i < numAttrs; ++i )
    {
        unsigned int channelLoc  = va_arg( args, unsigned int );
        unsigned int type		= va_arg( args, unsigned int );
        unsigned int semantic	= va_arg( args, unsigned int );

        unsigned int semanticIndex = va_arg( args, unsigned int );
    
        { semanticIndex; } // FIXME: suppress unused warning
        vd->SetAttribute( i, channelLoc, offset, (AttributeType)type, (AttributeSemantic)semantic );

        offset += m_sTypeSize[ type ];
    }

    vd->SetStride( (unsigned int) offset );

    va_end( args );

    return vd;
}

// *********************************
//
void VertexDescriptor::SetAttribute ( unsigned int attr, unsigned int channelLocation, unsigned int offset, AttributeType type, AttributeSemantic semantic )
{
    assert( attr >= 0 ); 
    assert( attr < m_numAttrs );
 
    Entry & e = m_entries[ attr ];

    e.channelLocation   = channelLocation;
    e.offset			= offset;
    e.type			    = type;
    e.semantic		    = semantic;
}

// *********************************
//
void VertexDescriptor::SetStride ( unsigned int stride )
{
    assert( stride > 0 );
    
    m_stride = stride;
}

// *********************************
//
unsigned int VertexDescriptor::NumAttrs		()	const
{
    return m_numAttrs;
}

// *********************************
//
unsigned int VertexDescriptor::ChannelLocation ( unsigned int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].channelLocation;
}

// *********************************
//
unsigned int VertexDescriptor::Offset ( unsigned int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].offset;
}

// *********************************
//
AttributeType VertexDescriptor::GetAttributeType ( unsigned int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].type;
}

// *********************************
//
AttributeSemantic VertexDescriptor::GetAttributeSemantic ( unsigned int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].semantic;
}


// *********************************
//
void VertexDescriptor::GetAttribute ( unsigned int attr, unsigned int * channelLocation, unsigned int * offset, AttributeType * type, AttributeSemantic * semantic ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    const Entry & e = m_entries[ attr ];

    *channelLocation    = e.channelLocation;
    *offset			    = e.offset;
    *type			    = e.type;
    *semantic		    = e.semantic;
}

// *********************************
//
unsigned int VertexDescriptor::Stride () const
{
    return m_stride;
}

// *********************************
//
unsigned int	VertexDescriptor::ComponentSize		( unsigned int attr ) const
{
    return ComponentSize( GetAttributeType( attr ) );
}

// *********************************
//
unsigned int	VertexDescriptor::NumComponents		( unsigned int attr ) const
{
    return NumComponents( GetAttributeType( attr ) );
}

// *********************************
//
unsigned int	VertexDescriptor::TypeSize			( unsigned int attr ) const
{
    return TypeSize( GetAttributeType( attr ) );
}


// *********************************
//
unsigned int VertexDescriptor::ComponentSize        ( AttributeType type )
{
    return m_sComponentSize[ (int) type ];
}

// *********************************
//
unsigned int VertexDescriptor::NumComponents        ( AttributeType type )
{
    return m_sNumComponents[ (int) type ];
}

// *********************************
//
unsigned int VertexDescriptor::TypeSize		        ( AttributeType type )
{
    return m_sTypeSize[ (int) type ];
}

} //bv
