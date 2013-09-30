#pragma once

#include <vector>
#include "Engine/Types/Enums.h"

namespace bv
{

class VertexDescriptor
{
protected:

    struct Entry
    {
        unsigned int	    channelLocation;
        unsigned int	    offset;
        AttributeType		type;
        AttributeSemantic	semantic;
    };

protected:

    static int	m_sComponentSize [ (int) AttributeType::AT_TOTAL ];
    static int	m_sNumComponents [ (int) AttributeType::AT_TOTAL ];
    static int	m_sTypeSize		 [ (int) AttributeType::AT_TOTAL ];


    int			            m_numAttrs;
    std::vector< Entry >	m_entries;
    int			            m_stride;

public:

								VertexDescriptor	( int numAttrs );
    virtual						~VertexDescriptor	();

    static VertexDescriptor *	Create				( int numAttrs, ... );

    void						SetAttribute		( int attr, unsigned int channelLocation, unsigned int offset, AttributeType type, AttributeSemantic semantic );
    void						SetStride			( int stride );

    int							NumAttrs			() const;

    unsigned int				ChannelLocation     ( int attr ) const;
    unsigned int				Offset				( int attr ) const;
    AttributeType		        GetAttributeType    ( int attr ) const;
	AttributeSemantic		    GetAttributeSemantic( int attr ) const;

	void						GetAttribute		( int attr, unsigned int * channelLocation, unsigned int * offset, AttributeType * type, AttributeSemantic * semantic ) const;

    int							Stride				()	const;
    int							Index				( AttributeSemantic semantic ) const;

    int					        ComponentSize		( int attr ) const;
    int					        NumComponents		( int attr ) const;
    int					        TypeSize			( int attr ) const;

    static int					ComponentSize		( AttributeType type );
    static int					NumComponents		( AttributeType type );
    static int					TypeSize			( AttributeType type );

};

} //bv
