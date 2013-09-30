#include "VertexDescriptor.h"
#include <cassert>
#include <cstdarg>

#include "Engine/Types/Enums.h"

namespace bv {

//FIXME: move closer to definition
int VertexDescriptor::m_sComponentSize[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    sizeof( float ),  // AT_FLOAT1
    sizeof( float ),  // AT_FLOAT2
    sizeof( float ),  // AT_FLOAT3
    sizeof( float ),  // AT_FLOAT4
};

int VertexDescriptor::m_sNumComponents[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    1,  // AT_FLOAT1
    2,  // AT_FLOAT2
    3,  // AT_FLOAT3
    4,  // AT_FLOAT4
};

int VertexDescriptor::m_sTypeSize[ (int) AttributeType::AT_TOTAL ] =
{
    //0,  // AT_NONE
    sizeof( float ),      // AT_FLOAT1
    2 * sizeof( float ),  // AT_FLOAT2
    3 * sizeof( float ),  // AT_FLOAT3
    4 * sizeof( float ),  // AT_FLOAT4
};


// *********************************
//
VertexDescriptor::VertexDescriptor ( int numAttrs )
    : m_numAttrs( numAttrs )
    , m_stride(0)
{
    assert( numAttrs > 0 );

    for( int i = 0; i < numAttrs; ++i )
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
//        0, VertexFormat::AT_FLOAT3, VertexFormat::AS_POSITION,
//        1, VertexFormat::AT_FLOAT2, VertexFormat::AS_TEXCOORD );
VertexDescriptor * VertexDescriptor::Create ( int numAttrs, ... )
{
    VertexDescriptor * vd = new VertexDescriptor( numAttrs );

    va_list args;
    va_start( args, numAttrs );

    unsigned int offset = 0;

    for ( int i = 0; i < numAttrs; ++i )
    {
        int channelLoc  = va_arg( args, int );
        int type		= va_arg( args, int );
        int semantic	= va_arg( args, int );

        unsigned int semanticIndex = va_arg( args, unsigned int );
    
        vd->SetAttribute( i, channelLoc, offset, (AttributeType)type, (AttributeSemantic)semantic );

        offset += m_sTypeSize[ type ];
    }

    vd->SetStride( (int) offset );

    va_end( args );

    return vd;
}

// *********************************
//
void VertexDescriptor::SetAttribute ( int attr, unsigned int channelLocation, unsigned int offset, AttributeType type, AttributeSemantic semantic )
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
void VertexDescriptor::SetStride ( int stride )
{
    assert( stride > 0 );
    
    m_stride = stride;
}

// *********************************
//
int VertexDescriptor::NumAttrs		()	const
{
    return m_numAttrs;
}

// *********************************
//
unsigned int VertexDescriptor::ChannelLocation ( int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].channelLocation;
}

// *********************************
//
unsigned int VertexDescriptor::Offset ( int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].offset;
}

// *********************************
//
AttributeType VertexDescriptor::GetAttributeType ( int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].type;
}

// *********************************
//
AttributeSemantic VertexDescriptor::GetAttributeSemantic ( int attr ) const
{
    assert( attr >= 0 );
    assert( attr < m_numAttrs );

    return m_entries[ attr ].semantic;
}


// *********************************
//
void VertexDescriptor::GetAttribute ( int attr, unsigned int * channelLocation, unsigned int * offset, AttributeType * type, AttributeSemantic * semantic ) const
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
int VertexDescriptor::Stride () const
{
    return m_stride;
}

// *********************************
//
int	VertexDescriptor::ComponentSize		( int attr ) const
{
    return ComponentSize( GetAttributeType( attr ) );
}

// *********************************
//
int	VertexDescriptor::NumComponents		( int attr ) const
{
    return NumComponents( GetAttributeType( attr ) );
}

// *********************************
//
int	VertexDescriptor::TypeSize			( int attr ) const
{
    return TypeSize( GetAttributeType( attr ) );
}


// *********************************
//
int VertexDescriptor::ComponentSize ( AttributeType type )
{
    return m_sComponentSize[ (int) type ];
}

// *********************************
//
int VertexDescriptor::NumComponents ( AttributeType type )
{
    return m_sNumComponents[ (int) type ];
}

// *********************************
//
int VertexDescriptor::TypeSize		( AttributeType type )
{
    return m_sTypeSize[ (int) type ];
}

} //bv
