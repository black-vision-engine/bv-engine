#pragma once

#include <vector>

namespace bv
{

class VertexBuffer;
class VertexDescriptor;

struct VertexArrayEntry
{

    VertexArrayEntry( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVert, unsigned int vbOffset );

    VertexBuffer *      vertexBuffer;
    VertexDescriptor *  vertexDescriptor;
    unsigned int        numVertices;
    unsigned int        vertexBufferOffset;

};

class VertexArray
{
private:

    std::vector< VertexArrayEntry >     m_vaoEntries;

public:

                            VertexArray         ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset );
                            VertexArray         ();
                            ~VertexArray        ();

    int                     NumEntries          () const;
    void                    AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset );
    VertexArrayEntry        Entry               ( int i ) const;

};

//FIXME: special case when this vao is bound to single vertex buffer with interleaved data described by vertex descriptor -> generic enough
class VertexArraySingleVertexBuffer
{
private:

    VertexBuffer *      m_vertexBuffer;
    VertexDescriptor *  m_vertexDescriptor;
    unsigned int        m_numVertices;
    unsigned int        m_vertexBuferOffset;

public:

                                        VertexArraySingleVertexBuffer   ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset );
                                        ~VertexArraySingleVertexBuffer  ();

            const VertexBuffer *        GetVertexBuffer                 () const;
            const VertexDescriptor *    GetVertexDescriptor             () const;
            unsigned int                GetNumVertices                  () const;
            unsigned int                GetVertexBufferOffset           () const;

};

}
