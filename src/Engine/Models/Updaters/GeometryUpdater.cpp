#include "GeometryUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Graphics/Resources/RenderableArrayData.h"

namespace bv {

// *********************************
//
GeometryUpdater::GeometryUpdater     ( RenderableEntity * out, const model::IGeometryChannel * in )
    : m_in( in )
    , m_out( out )
{
    assert( in != nullptr );
    assert( out != nullptr );
}

// *********************************
//
GeometryUpdater::~GeometryUpdater    ()
{
    //FIXME: anything here? 
    //FIXME: Yes.
    //FIXME: Really?
}

// *********************************
//
void    GeometryUpdater::Update      ( float t )
{
    //If registered, it means that it is not time invariant (although may not need update in this frame)

    if ( m_in->NeedsPositionsUpdate( t ) )
    {
        assert( !m_in->NeedsTopologyUpdate( t ) );

        UpdatePositions( t );
    
    }
    else if ( m_in->NeedsTopologyUpdate( t ) )
    {
        UpdateTopology( t );
    }
}

// *********************************
//
void    GeometryUpdater::UpdatePositions     ( float t )
{
    const RenderableArrayDataSingleVertexBuffer * rad = m_out->GetRenderableArrayData();

    //VertexBuffer * vertexBuffer = rad-

    //auto geometryChannel = m_in;
    //auto components = geometryChannel->GetComponents();
    //auto geomDesc = geometryChannel->GetDescriptor();

    //VertexBuffer * vertexBuffer         = new VertexBuffer( TotalNumVertices( ccVec ), desc->SingleVertexEntrySize() );
    //VertexDescriptor * vertexDescriptor = CreateVertexDescriptor( desc );

    //VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vertexBuffer, vertexDescriptor );
    //RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    //char * vbData = vertexBuffer->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P

    //unsigned int currentOffset = 0;

    //for( auto cc : ccVec )
    //{
    //    assert( !cc->GetVertexAttributeChannels().empty() );

    //    vao->AddCCEntry( cc->GetNumVertices() );



    //        unsigned int numVertices = cc->GetNumVertices();
    //        unsigned int offset = 0;

    //        for( unsigned int i = 0; i < numVertices; ++i )
    //        {
    //            for( auto vach : cc->GetVertexAttributeChannels() )
    //            {
    //                assert( vach->GetNumEntries() == numVertices );

    //                auto eltSize = vach->GetDescriptor()->GetEntrySize();
    //                const char * eltData = vach->GetData();

    //                memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

    //                offset += eltSize;
    //            }
    //        }




    //    currentOffset += cc->GetNumVertices() * desc->SingleVertexEntrySize();
    //}

    //return rad;

}

// *********************************
//
void    GeometryUpdater::UpdateTopology      ( float t )
{
}

} //bv
