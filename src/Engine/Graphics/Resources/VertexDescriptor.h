#pragma once

#include "Engine/Types/Enums.h"

namespace bv
{

class VertexDescriptor
{
public:
 
	enum
    {
        // The maximum number of attributes for a vertex format.
        VD_MAX_ATTRIBUTES = 16,

        // The maximum number of texture coordinate units.
        VD_MAX_TCOORD_UNITS = 8,

        // The maximum number of color units.
        VD_MAX_COLOR_UNITS = 2
    };

protected:

    class Element
    {
    public:

        unsigned int	streamIndex;
        unsigned int	offset;
        AttrType		type;
        AttrSemantic	semantic;
        unsigned int	usageIndex;
    };

    int			m_numAttrs;
    Element		m_elements[ VD_MAX_ATTRIBUTES ];
    int			m_stride;

    static int	m_sComponentSize [ AT_TOTAL ];
    static int	m_sNumComponents [ AT_TOTAL ];
    static int	m_sTypeSize		 [ AT_TOTAL ];

public:

								VertexDescriptor	( int numAttrs );
    virtual						~VertexDescriptor	();

    static VertexDescriptor *	Create				( int numAttrs, ... );

    void						SetAttribute		( int attr, unsigned int streamIndex, unsigned int offset, AttrType type, AttrSemantic semantic, unsigned int usageIndex );
    void						SetStride			( int stride );

    int							NumAttrs			() const;

    unsigned int				StreamIndex			( int attr ) const;
    unsigned int				Offset				( int attr ) const;
    AttrType					AttributeType		( int attr ) const;
	AttrSemantic				AttributeSemantic	( int attr ) const;
    unsigned int				UsageIndex			( int attr ) const;

	void						GetAttribute		( int attr, unsigned int * streamIndex, unsigned int * offset, AttrType * type, AttrSemantic * semantic, unsigned int * usageIndex ) const;

    int							Stride				()	const;

    int							Index				( AttrSemantic semantic, unsigned int semanticIndex = 0 ) const;

    static int					ComponentSize		( AttrType type );
    static int					NumComponents		( AttrType type );
    static int					TypeSize			( AttrType type );

};

}
