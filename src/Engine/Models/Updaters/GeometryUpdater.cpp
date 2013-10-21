#include "GeometryUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"

#include "Engine/Graphics/Resources/VertexArray.h"

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
//FIXME: not positions but something more general (attribute data, contents or something like this which does not involwe recreating the whole thing)
void    GeometryUpdater::UpdatePositions     ( float t )
{
    //FIXME: implement for other types of geometry as well
    assert( m_out->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessart
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_out->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = rad->VAO                  (); 
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    auto geomChannel    = m_in;
    auto components     = geomChannel->GetComponents();
    auto geomDesc       = geomChannel->GetDescriptor();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL
    unsigned int currentOffset = 0;

    unsigned int k = 0;

    for( auto cc : components )
    {
        unsigned int numVertices = cc->GetNumVertices();

        assert( numVertices == vao->GetNumVertices( k ) );
    
        char * data = &vbData[ currentOffset ];

        unsigned int offset = 0;

        for( unsigned int i = 0; i < numVertices; ++i )
        {
            for( auto vach : cc->GetVertexAttributeChannels() )
            {
                assert( vach->GetNumEntries() == numVertices );

                auto eltSize = vach->GetDescriptor()->GetEntrySize();
                const char * eltData = vach->GetData();

                memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

                offset += eltSize;
            }
        }

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();

        ++k;
    }

    vao->SetNeedsUpdateMemUpload( true );
}

// *********************************
//
void    GeometryUpdater::UpdateTopology      ( float t )
{
}

} //bv
