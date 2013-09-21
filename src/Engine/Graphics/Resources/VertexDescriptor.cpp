#include "VertexDescriptor.h"
#include <cassert>
#include <cstdarg>

namespace bv {

//FIXME: move closer to definition
int VertexDescriptor::m_sComponentSize[ AT_TOTAL ] =
{
    0,  // AT_NONE
    4,  // AT_FLOAT1
    4,  // AT_FLOAT2
    4,  // AT_FLOAT3
    4,  // AT_FLOAT4
};

int VertexDescriptor::m_sNumComponents[ AT_TOTAL ] =
{
    0,  // AT_NONE
    1,  // AT_FLOAT1
    2,  // AT_FLOAT2
    3,  // AT_FLOAT3
    4,  // AT_FLOAT4
};

int VertexDescriptor::m_sTypeSize[ AT_TOTAL ] =
{
    0,  // AT_NONE
    4,  // AT_FLOAT1
    8,  // AT_FLOAT2
    12, // AT_FLOAT3
    16, // AT_FLOAT4
};

// *********************************
//
VertexDescriptor::VertexDescriptor ( int numAttrs )
	: m_numAttrs( numAttrs )
    , m_stride(0)
{
    assert( numAttrs > 0 );

    for ( int i = 0; i < VD_MAX_ATTRIBUTES; ++i )
    {
        m_elements[ i ].streamIndex = 0;
        m_elements[ i ].offset		= 0;
        m_elements[ i ].type		= AT_NONE;
        m_elements[ i ].semantic	= AS_NONE;
        m_elements[ i ].usageIndex	= 0;
    }
}

// *********************************
//
VertexDescriptor::~VertexDescriptor ()
{
	//FIXME: call renderer here and unbind all vertex deata associated with this vertex descriptor
}

// *********************************
//    VertexDescriptor * vd = VertexDescriptor::Create(2,
//        VertexFormat::AT_FLOAT3, VertexFormat::AS_POSITION, 0,
//        VertexFormat::AT_FLOAT2, VertexFormat::AS_TEXCOORD, 0);
VertexDescriptor * VertexDescriptor::Create ( int numAttrs, ... )
{
    VertexDescriptor * vd = new VertexDescriptor( numAttrs );

    va_list args;
    va_start( args, numAttrs );

    unsigned int offset = 0;

    for ( int i = 0; i < numAttrs; ++i )
    {
        int type		= va_arg( args, int );
        int semantic	= va_arg( args, int );

        unsigned int semanticIndex = va_arg( args, unsigned int );
    
		vd->SetAttribute( i, 0, offset, (AttrType)type, (AttrSemantic)semantic, semanticIndex );

		offset += m_sTypeSize[ type ];
    }

    vd->SetStride( (int) offset );

    va_end( args );

    return vd;
}

// *********************************
//
void VertexDescriptor::SetAttribute ( int attr, unsigned int streamIndex, unsigned int offset, AttrType type, AttrSemantic semantic, unsigned int usageIndex )
{
	assert( attr >= 0 ); 
	assert( attr < m_numAttrs );
 
    Element & elt = m_elements[ attr ];

    elt.streamIndex		= streamIndex;
    elt.offset			= offset;
    elt.type			= type;
    elt.semantic		= semantic;
    elt.usageIndex		= usageIndex;
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
unsigned int VertexDescriptor::StreamIndex ( int attr ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

	return m_elements[ attr ].streamIndex;
}

// *********************************
//
unsigned int VertexDescriptor::Offset ( int attr ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

    return m_elements[ attr ].offset;
}

// *********************************
//
AttrType VertexDescriptor::AttributeType ( int attr ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

    return m_elements[ attr ].type;
}

// *********************************
//
AttrSemantic VertexDescriptor::AttributeSemantic ( int attr ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

	return m_elements[ attr ].semantic;
}

// *********************************
//
unsigned int VertexDescriptor::UsageIndex ( int attr  ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

	return m_elements[ attr ].usageIndex;
}

// *********************************
//
void VertexDescriptor::GetAttribute ( int attr, unsigned int * streamIndex, unsigned int * offset, AttrType * type, AttrSemantic * semantic, unsigned int * usageIndex ) const
{
	assert( attr >= 0 );
	assert( attr < m_numAttrs );

    const Element& e = m_elements[ attr ];

    *streamIndex	= e.streamIndex;
    *offset			= e.offset;
    *type			= e.type;
    *semantic		= e.semantic;
    *usageIndex		= e.usageIndex;
}

// *********************************
//
int VertexDescriptor::Stride () const
{
    return m_stride;
}

// *********************************
//
int VertexDescriptor::Index ( AttrSemantic semantic, unsigned int usageIndex ) const
{
	for ( int i = 0; i < m_numAttrs; ++i )
    {
        if ( m_elements[ i ].semantic == semantic &&  m_elements[ i ].usageIndex == usageIndex )
        {
            return i;
        }
    }

    return -1;
}

// *********************************
//
int VertexDescriptor::ComponentSize ( AttrType type )
{
    return m_sComponentSize[ type ];
}

// *********************************
//
int VertexDescriptor::NumComponents ( AttrType type )
{
    return m_sNumComponents[ type ];
}

// *********************************
//
int VertexDescriptor::TypeSize		( AttrType type )
{
    return m_sTypeSize[ type ];
}

}


