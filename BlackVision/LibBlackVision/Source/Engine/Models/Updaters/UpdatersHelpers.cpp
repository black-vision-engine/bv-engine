#include "stdafx.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "UpdatersHelpers.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"
#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Application/ApplicationContext.h"


#include "Memory/MemoryLeaks.h"



namespace bv
{



// *********************************
//FIXME: this should be implemented via VBOAcessor in one place only (VBO utils or some more generic utils) - right now it is copied here and in BasicNode (as AddVertexDataToVBO)
void    UpdatersHelpers::WriteVertexDataToVBO( char * data, model::IConnectedComponentPtr cc )
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

// *****************************
//
void    UpdatersHelpers::UpdateRenderableBuffer     ( RenderableEntity * renderable, model::IConnectedComponentPtr cc )
{
    // This can copy verticies for other topologies either.
    //assert( renderable->GetType() == RenderableEntity::RenderableType::RT_LINES );

    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = rad->VAO(); 
    VertexBuffer * vb                   = vao->GetVertexBuffer();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL

    //This is update only, so the number of vertices must match
    assert( cc->GetNumVertices() == vao->GetNumVertices( 0 ) );
 
    UpdatersHelpers::WriteVertexDataToVBO( vbData, cc );

    vao->SetNeedsUpdateMemUpload( true );
}

// ***********************
//
void    UpdatersHelpers::RecreateRenderableBuffer    ( RenderableEntity * renderable, model::IConnectedComponentPtr cc )
{
    RenderableArrayDataArraysSingleVertexBuffer * radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = radasvb->VAO();
    VertexBuffer * vb                   = vao->GetVertexBuffer();

    auto channels = cc->GetAttributeChannels();
    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_DYNAMIC;
    int entrySize = 0;
    
    for( auto & desc : channels )
    {
        entrySize += desc->GetDescriptor()->GetEntrySize();
    }

    vb->Reinitialize( cc->GetNumVertices(), entrySize, vbSemantic );
    vao->ResetState();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P

    assert( !cc->GetAttributeChannels().empty() );

    vao->AddCCEntry( cc->GetNumVertices() );
    UpdatersHelpers::WriteVertexDataToVBO( &vbData[ 0 ], cc );
    
    vao->SetNeedsUpdateRecreation( true );
}

// ***********************
//
void         UpdatersHelpers::UpdateCamera                ( Camera * camera, model::CameraModelPtr & cameraModel )
{
    auto position = QueryTypedValue< ValueVec3Ptr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::POSITION ) );
    auto direction = QueryTypedValue< ValueVec3Ptr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::DIRECTION ) );
    auto up = QueryTypedValue< ValueVec3Ptr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::UP_VECTOR ) );
    auto fov = QueryTypedValue< ValueFloatPtr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::FOV ) );
    auto isPerspective = QueryTypedValue< ValueBoolPtr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::IS_PERSPECTIVE ) );
    auto far = QueryTypedValue< ValueFloatPtr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::FAR_CLIPPING_PLANE ) );
    auto near = QueryTypedValue< ValueFloatPtr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::NEAR_CLIPPING_PLANE ) );
    auto size = QueryTypedValue< ValueFloatPtr >( cameraModel->GetValue( model::CameraModel::PARAMETERS::VIEWPORT_SIZE ) )->GetValue();


    auto height = ( float )ApplicationContext::Instance().GetHeight();
    auto width = ( float )ApplicationContext::Instance().GetWidth();
    
    float aspect = width / height;

    if( isPerspective->GetValue() )
        camera->SetPerspective( fov->GetValue(), width, height, near->GetValue(), far->GetValue() );
    else
        camera->SetOrthogonal( aspect * size, size, near->GetValue(), far->GetValue() );

    camera->SetFrame( position->GetValue(), direction->GetValue(), up->GetValue() );
}

}	// bv