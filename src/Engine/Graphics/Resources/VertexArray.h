#pragma once

#include <vector>

namespace bv
{

class VertexBuffer;
class VertexDescriptor;

struct VertexArrayEntry
{
    VertexArrayEntry( VertexBuffer * vb, VertexDescriptor * vd );

    VertexBuffer *      vertexBuffer;
    VertexDescriptor *  vertexDescriptor;
};

class VertexArray
{
private:

    std::vector< VertexArrayEntry >     m_vaoEntries;

public:

                            VertexArray         ();
                            ~VertexArray        ();

    int                     NumEntries          () const;
    void                    AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd );
    VertexArrayEntry        Entry               ( int i );

};

}