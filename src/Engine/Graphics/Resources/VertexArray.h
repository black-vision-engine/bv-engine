#pragma once

#include <vector>

namespace bv
{

class VertexBuffer;
class VertexDescriptor;

struct VertexArrayEntry
{
    VertexArrayEntry( VertexBuffer * vb, VertexDescriptor * vd, unsigned int vboffset );

    VertexBuffer *      vertexBuffer;
    VertexDescriptor *  vertexDescriptor;
    unsigned int        vertexBufferOffset;
};

class VertexArray
{
private:

    std::vector< VertexArrayEntry >     m_vaoEntries;

public:

                            VertexArray         ();
                            ~VertexArray        ();

    int                     NumEntries          () const;
    void                    AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int vertexBufferOffset );
    VertexArrayEntry        Entry               ( int i ) const;

};

}