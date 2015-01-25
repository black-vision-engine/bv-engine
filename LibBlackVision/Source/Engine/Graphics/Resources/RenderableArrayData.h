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

//FIXME: this is designed for singe vertexbuffer <-> vertexdescriptor case (it's general enough, the rest is simply abut the Sdata layout)
class RenderableArrayDataSingleVertexBuffer
{
protected:

    VertexBuffer    *   m_vertexBuffer;
    VertexDescriptor *  m_vertexDescriptor;

    RenderableArrayDataEnumKind     m_type;

public:

                                        RenderableArrayDataSingleVertexBuffer   ( RenderableArrayDataEnumKind type, VertexBuffer * vb, VertexDescriptor * vd );
    virtual                             ~RenderableArrayDataSingleVertexBuffer  ();

    RenderableArrayDataEnumKind         Type                                    () const;

    virtual const VertexDescriptor *    GetVertexDescriptor                     () const;
    virtual const VertexBuffer *        GetVertexBuffer                         () const;

    virtual VertexDescriptor *          GetVertexDescriptor                     ();
    virtual VertexBuffer *              GetVertexBuffer                         ();

    virtual unsigned int                GetNumConnectedComponents               () const = 0;
    virtual unsigned int                GetNumVerticesInConnectedComponent      ( unsigned int ccNum ) const = 0;

};

}
