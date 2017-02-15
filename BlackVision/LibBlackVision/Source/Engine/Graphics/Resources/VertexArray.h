#pragma once

#include <vector>

#include "CoreDEF.h"


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

    SizeType                NumEntries          () const;
    void                    AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset );
    VertexArrayEntry        Entry               ( unsigned int i ) const;

};

//FIXME: special case when this vao is bound to single vertex buffer with interleaved data described by vertex descriptor -> generic enough
class VertexArraySingleVertexBuffer
{
private:

    VertexBuffer *              m_vertexBuffer;
    VertexDescriptor *          m_vertexDescriptor;
    
    unsigned int                m_numTotalVertices;
    std::vector< unsigned int > m_ccVertexNum;

    //unsigned int        m_numVertices;
    //unsigned int        m_vertexBuferOffset;

    mutable bool                m_needsUpdateMemUpload;  // FIXME: Consider using mutable keyword here
    mutable bool                m_needsUpdateRecreation; // FIXME: Consider using mutable keyword here

public:

                                VertexArraySingleVertexBuffer   ( VertexBuffer * vb, VertexDescriptor * vd );
                                ~VertexArraySingleVertexBuffer  ();

    const VertexBuffer *        GetVertexBuffer                 () const;
    const VertexDescriptor *    GetVertexDescriptor             () const;

    VertexBuffer *              GetVertexBuffer                 ();
    VertexDescriptor *          GetVertexDescriptor             ();

    void                        SetVertexDescriptor             ( VertexDescriptor * desc );

    void                        AddCCEntry                      ( unsigned int numVertices );

    SizeType                    GetNumConnectedComponents       () const;

    unsigned int                GetNumVertices                  ( unsigned int ccNum ) const;
    unsigned int                GetNumTotalVertices             () const;

    void                        SetNeedsUpdateMemUpload         ( bool needsUpdateMemUpload ) const;
    void                        SetNeedsUpdateRecreation        ( bool needsUpdateRecreation ) const;

    bool                        NeedsUpdateMemUpload            () const;
    bool                        NeedsUpdateRecreation           () const;

    void                        ResetState                      ();

};

}
