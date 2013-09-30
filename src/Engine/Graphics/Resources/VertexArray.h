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

    std::vector< unsigned int >     m_ccVertexNum;
    std::vector< unsigned int >     m_ccVertexBuffOffsets;
    //unsigned int        m_numVertices;
    //unsigned int        m_vertexBuferOffset;

public:

                                        VertexArraySingleVertexBuffer   ( VertexBuffer * vb, VertexDescriptor * vd );
                                        ~VertexArraySingleVertexBuffer  ();

            const VertexBuffer *        GetVertexBuffer                 () const;
            const VertexDescriptor *    GetVertexDescriptor             () const;

            void                        AddCCEntry                      ( unsigned int numVertices, unsigned int vertexBufferOffset );

            unsigned int                GetNumConnectedComponents       () const;

            unsigned int                GetNumVertices                  ( unsigned int ccNum ) const;
            unsigned int                GetVertexBufferOffset           ( unsigned int ccNum ) const;

};

}
