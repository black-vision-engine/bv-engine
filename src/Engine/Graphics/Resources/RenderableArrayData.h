#pragma once

#include <vector>

namespace bv
{

class VertexBuffer;
class VertexDescriptor;

enum class RenderableArrayDataEnumKind : int
{
    RADEK_VAO = 0,
    RADEK_ELEMENTS,

    RADT_TOTAL
};

class RenderableArrayData
{
protected:

    VertexBuffer    *   m_vertexBuffer;
    VertexDescriptor *  m_vertexDescriptor;

    std::vector< unsigned int >     m_numVerticesInCC;
    std::vector< unsigned int >     m_ccOffsets;

    RenderableArrayDataEnumKind     m_type;

public:

                                    RenderableArrayData         ( RenderableArrayDataEnumKind type );
    virtual                         ~RenderableArrayData        () = 0;

    RenderableArrayDataEnumKind     Type                        () const;

    const VertexDescriptor *        GetVertexDecscriptor        () const;
    const VertexBuffer *            GetVertexBuffer             () const;

    unsigned int                    GetCCOffset                 ( int ccNum ) const;
    unsigned int                    GetCCNumVertices            ( int ccNum ) const;

    unsigned int                    GetNumConnectedComponents   () const;

};

}
