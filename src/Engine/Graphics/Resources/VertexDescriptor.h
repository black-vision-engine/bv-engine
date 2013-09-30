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

    static unsigned int	m_sComponentSize [ (int) AttributeType::AT_TOTAL ];
    static unsigned int	m_sNumComponents [ (int) AttributeType::AT_TOTAL ];
    static unsigned int	m_sTypeSize		 [ (int) AttributeType::AT_TOTAL ];

    unsigned int			m_numAttrs;
    std::vector< Entry >	m_entries;
    unsigned int	        m_stride;

public:

								VertexDescriptor	( unsigned int numAttrs );
    virtual						~VertexDescriptor	();

    static VertexDescriptor *	Create				( unsigned int numAttrs, ... );

    void						SetAttribute		( unsigned int attr, unsigned int channelLocation, unsigned int offset, AttributeType type, AttributeSemantic semantic );
    void						SetStride			( unsigned int stride );

    int							NumAttrs			() const;

    unsigned int				ChannelLocation     ( unsigned int attr ) const;
    unsigned int				Offset				( unsigned int attr ) const;
    AttributeType		        GetAttributeType    ( unsigned int attr ) const;
	AttributeSemantic		    GetAttributeSemantic( unsigned int attr ) const;

	void						GetAttribute		( unsigned int attr, unsigned int * channelLocation, unsigned int * offset, AttributeType * type, AttributeSemantic * semantic ) const;

    unsigned int		        Stride				() const;
    unsigned int			    Index				( AttributeSemantic semantic ) const;

    unsigned int				ComponentSize		( unsigned int attr ) const;
    unsigned int				NumComponents		( unsigned int attr ) const;
    unsigned int				TypeSize			( unsigned int attr ) const;

    static unsigned int			ComponentSize		( AttributeType type );
    static unsigned int			NumComponents		( AttributeType type );
    static unsigned int         TypeSize			( AttributeType type );

};

} //bv
