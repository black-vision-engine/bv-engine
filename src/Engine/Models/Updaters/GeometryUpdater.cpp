#include "GeometryUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"

#include "Engine/Graphics/Resources/VertexArray.h"


namespace bv {

// *********************************
//
GeometryUpdater::GeometryUpdater     ( RenderableEntity * out, const model::IVertexAttributesChannel * in )
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
void    GeometryUpdater::DoUpdate    ( TimeType t )
{
    //If registered, it means that it is not time invariant (although may not need update in this frame)

    if ( m_in->NeedsAttributesUpdate( t ) )
    {
        assert( !m_in->NeedsTopologyUpdate( t ) );

        UpdatePositions( t );
    
    }
    else if ( m_in->NeedsTopologyUpdate( t ) )
    {
        UpdateTopology( t );
    }
    else
    {
        RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_out->GetRenderableArrayData() );
        VertexArraySingleVertexBuffer * vao = rad->VAO();

        vao->SetNeedsUpdateMemUpload( false );
        vao->SetNeedsUpdateRecreation( false );
    }
}

// *********************************
//FIXME: not positions but something more general (attribute data, contents or something like this which does not involwe recreating the whole thing)
void    GeometryUpdater::UpdatePositions     ( TimeType t )
{
    //FIXME: implement for other types of geometry as well
    assert( m_out->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_out->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = rad->VAO                  (); 
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    auto vaChannel    = m_in;
    auto components     = vaChannel->GetComponents();
    auto geomDesc       = vaChannel->GetDescriptor();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL
    unsigned int currentOffset = 0;

    unsigned int k = 0;

    for( auto cc : components )
    {
        //This is update only, so the number of vertices must match
        assert( cc->GetNumVertices() == vao->GetNumVertices( k ) );
 
        WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();

        ++k;
    }

    vao->SetNeedsUpdateMemUpload( true );
}

// *********************************
//
void    GeometryUpdater::UpdateTopology      ( TimeType t )
{
    //FIXME: implement for other types of geometry as well
    assert( m_out->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    //FIXME: if this is the last update then STATIC semantic should be used but right now it's irrelevant
    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_DYNAMIC;

    auto vaChannel    = m_in;
    auto components     = vaChannel->GetComponents();
    auto geomDesc       = vaChannel->GetDescriptor();

    if( components.empty() )
    {
        assert( false ); //FIXME: at this point empty geometry is not allowed
        return;
    }

    unsigned int totalNumVertivces = vaChannel->TotalNumVertices();
    
    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_out->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = radasvb->VAO              ();
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    vb->Reinitialize( totalNumVertivces, geomDesc->SingleVertexEntrySize(), vbSemantic );
    vao->ResetState();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P
    unsigned int currentOffset = 0;

    for( auto cc : components )
    {
        assert( !cc->GetAttributeChannels().empty() );

        vao->AddCCEntry( cc->GetNumVertices() );

        WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();
    }

    vao->SetNeedsUpdateRecreation( true );
}

// *********************************
//FIXME: this should be implemented via VBOAcessor in one place only (VBO utils or some more generic utils) - right now it is copied here and in BasicNode (as AddVertexDataToVBO)
void    GeometryUpdater::WriteVertexDataToVBO( char * data, model::IConnectedComponent * cc )
{
    unsigned int numVertices = cc->GetNumVertices();
    unsigned int offset = 0;

    for( unsigned int i = 0; i < numVertices; ++i )
    {
        for( auto vach : cc->GetAttributeChannels() )
        {
            assert( vach->GetNumEntries() == numVertices );

            auto eltSize = vach->GetDescriptor()->GetEntrySize(); //FIXME: most probably not required here (can be safely read from other location)
            const char * eltData = vach->GetData();

            memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

            offset += eltSize;
        }
    }
}

} //bv
